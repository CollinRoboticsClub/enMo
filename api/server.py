#!/usr/bin/env python3
import asyncio

import cv2
import cv2.data
import serial
import uvicorn
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
from websockets.exceptions import ConnectionClosed


class MovementPacket(BaseModel):
    x: float
    y: float
    rotation: float


def arduino_send(input_str: str):
    arduino.write(f"{input_str}\n".encode())


def arduino_send_arr(input_arr: list[str]):
    for input_str in input_arr:
        arduino_send(input_str)


app = FastAPI(root_path="/api")


@app.post("/move-wheels")
async def move_wheels(movement_packet: MovementPacket):
    # TODO:
    # - figure out how to: if no movement packet received within last X seconds, send basically
    #   a packet of all zeroes to make the robot stop moving if the client abruptly disconnects.

    x = movement_packet.x
    y = movement_packet.y
    rotation = movement_packet.rotation
    arduino_send(f"{x} {y} {rotation}")

    return 0


@app.post("/move-arm")
async def move_arm(movement_packet: MovementPacket):
    x = movement_packet.x
    y = movement_packet.y

    # TODO: implement this

    return 0


@app.websocket("/video")
async def video_feed(websocket: WebSocket):
    await websocket.accept()
    try:
        while True:
            if video_capture is None:
                break

            # read frame
            result, video_frame = video_capture.read()
            if result is False:
                break

            await detect_bounding_box(video_frame)

            # encode frame as WebP
            _, encoded_frame = cv2.imencode(".webp", video_frame)

            # send it
            encoded_frame_bytes = encoded_frame.tobytes()
            await websocket.send_bytes(encoded_frame_bytes)

            # to avoid hogging all the CPU time
            await asyncio.sleep(0)
    except (WebSocketDisconnect, ConnectionClosed):
        print("Client disconnected")


async def detect_bounding_box(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    faces = face_classifier.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(40, 40))
    for x, y, w, h in faces:
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 4)

        # oooh, center coords for targets >:>
        center = (int(x + (w / 2)), int(y + (h / 2)))
        cv2.circle(image, center, 10, (0, 0, 160))


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # very security :)
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.mount("/", StaticFiles(directory="./webui", html=True), name="static")

arduino = serial.Serial(port="/dev/ttyUSB0", baudrate=9600, timeout=0.5)

face_classifier = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_frontalface_default.xml")
video_capture = cv2.VideoCapture(0)
video_capture.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
video_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

if __name__ == "__main__":
    uvicorn.run("server:app", reload=True, host="0.0.0.0", port=8000)

    video_capture.release()
    cv2.destroyAllWindows()
