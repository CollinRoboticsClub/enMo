import asyncio
import concurrent.futures
import functools
import multiprocessing as mp
from contextlib import asynccontextmanager

import cv2
import cv2.data
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse


def detect_bounding_box(image, cascade_classifier):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    faces = cascade_classifier.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(40, 40))
    for x, y, w, h in faces:
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 4)

        # oooh, center coords for targets >:>
        center = (int(x + (w / 2)), int(y + (h / 2)))
        cv2.circle(image, center, 10, (0, 0, 160))


def queue_camera_frames(
        input_video_capture: cv2.VideoCapture,
        output_frames_queue: mp.Queue
):
    while True:
        # read frame
        result, video_frame = input_video_capture.read()
        if result is False:
            continue

        output_frames_queue.put(video_frame)

        # print("queued camera frame")


def encode_processed_frames(
        input_frames_queue: mp.Queue,
        output_frames_queue: mp.Queue
):
    while True:
        current_frame = input_frames_queue.get()

        # encode frame
        _, encoded_frame = cv2.imencode(".jpg", current_frame)

        # convert frame to bytes and add to queue
        output_frames_queue.put(encoded_frame.tobytes())

        # print("queued encoded frame bytes")


def process_camera_frames(
        input_frames_queue: mp.Queue,
        output_frames_queue: mp.Queue,
        cascade_classifier: cv2.CascadeClassifier
):
    while True:
        current_frame = input_frames_queue.get()

        # this modifies the parameter in-place
        detect_bounding_box(current_frame, cascade_classifier)

        output_frames_queue.put(current_frame)

        # print("queued processed camera frame")


# constants
FACE_CLASSIFIER = cv2.CascadeClassifier(f"{cv2.data.haarcascades}/haarcascade_frontalface_default.xml")
QUEUE_MAX_SIZE = 15

# globals
global_encoded_byte_frame_queue = mp.Queue(maxsize=QUEUE_MAX_SIZE)
# noinspection PyShadowingBuiltins
print = functools.partial(print, flush=True)  # shadowing of builtin is intentional


@asynccontextmanager
async def lifespan(app: FastAPI):
    # Run at startup
    print("Starting up...")

    # NOTE:
    #  there was a bug when running this on the Pi where if this code ran too early after the program's
    #  initial execution it would fail for some reason. If this fails, try adding an `asyncio.sleep(1)`
    #  or something like that.
    video_capture = cv2.VideoCapture(0)
    video_capture.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    video_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

    camera_frames_queue = mp.Queue(maxsize=QUEUE_MAX_SIZE)
    opencv_processed_queue = mp.Queue(maxsize=QUEUE_MAX_SIZE)

    print("about to start camera threads")
    executor = concurrent.futures.ThreadPoolExecutor(max_workers=3)
    executor.submit(
        queue_camera_frames,
        input_video_capture=video_capture,
        output_frames_queue=camera_frames_queue
    )
    executor.submit(
        process_camera_frames,
        input_frames_queue=camera_frames_queue,
        output_frames_queue=opencv_processed_queue,
        cascade_classifier=FACE_CLASSIFIER,
    )
    executor.submit(
        encode_processed_frames,
        input_frames_queue=opencv_processed_queue,
        output_frames_queue=global_encoded_byte_frame_queue,
    )
    print("done starting camera threads")

    yield

    # Run on shutdown
    print("Shutting down...")
    video_capture.release()
    executor.shutdown()


app = FastAPI(root_path="/api", lifespan=lifespan)


@app.get("/video")
async def video_feed():
    async def stream_video():
        while True:
            # yield to the event loop, so we don't hoard all the CPU time
            await asyncio.sleep(0)

            frame = global_encoded_byte_frame_queue.get()
            if frame is None:
                break

            # print("sending frame!!")

            yield b"--frame\r\n" b"Content-Type: image/jpeg\r\n\r\n" + frame + b"\r\n"

    return StreamingResponse(stream_video(), media_type="multipart/x-mixed-replace;boundary=frame")


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # very security :)
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
