# following https://www.datacamp.com/tutorial/face-detection-python-opencv
import cv2
import cv2.data

face_classifier = cv2.CascadeClassifier(
    cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
)


video_capture = cv2.VideoCapture(0)
# video_capture = cv2.VideoCapture(1)

video_capture.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
video_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)


def detect_bounding_box(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    faces = face_classifier.detectMultiScale(
        gray, scaleFactor=1.1, minNeighbors=5, minSize=(40, 40)
    )

    for x, y, w, h in faces:
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 4)

        # oooh, center coords for targets >:>
        center = (int(x + (w / 2)), int(y + (h / 2)))
        cv2.circle(image, center, 10, (0, 0, 160))


while True:
    result, video_frame = video_capture.read()
    if result is False:
        break
    
    faces = detect_bounding_box(video_frame)

    cv2.imshow(
        "cameera", video_frame
    )
    
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

video_capture.release()
cv2.destroyAllWindows()
