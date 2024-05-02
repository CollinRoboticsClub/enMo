from cscore import CameraServer
import cv2
import numpy as np


CameraServer.enableLogging()

camera = CameraServer.startAutomaticCapture()
camera.setResolution(width, height)

sink = cs.getVideo()

while True:
    time, input_img = cvSink.grabFrame(input_img)

    if time == 0:  # There is an error
        continue
