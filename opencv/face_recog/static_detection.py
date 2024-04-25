# following https://www.datacamp.com/tutorial/face-detection-python-opencv
import cv2
import cv2.data

img_path = "./lowres Fresh Prince.jpg"

img = cv2.imread(img_path)
print(f"shape: {img.shape}")

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
print(f"shape: {gray.shape}")

face_classifier = cv2.CascadeClassifier(
    cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
)

face = face_classifier.detectMultiScale(
    gray, scaleFactor=1.1, minNeighbors=8, minSize=(100, 100) # my tune for will smiff
    # gray, scaleFactor=1.5, minNeighbors=1, minSize=(100, 100) # super sensitive
    # gray, scaleFactor=1.1, minNeighbors=5, minSize=(40,40) # stock tune from guide
)

for x, y, w, h in face:
    cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 4)

    # oooh, center coords for targets >:>
    center = (int(x + (w / 2)), int(y + (h / 2)))
    cv2.circle(img, center, 10, (0, 0, 160))


img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

# cv2.imshow("he so freshh", img_rgb)
cv2.imshow("he so freshh", img)
cv2.waitKey(0)
cv2.destroyAllWindows()
