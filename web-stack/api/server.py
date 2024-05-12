#!/usr/bin/env python3
from contextlib import asynccontextmanager

from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
from websockets.exceptions import ConnectionClosed

from arduino_serial_utils import ArduinoSerialConnection
from arm_servos import Arm


class WheelMovementPacket(BaseModel):
    x: float
    y: float
    rotation: float


class ArmMovementPacket(BaseModel):
    base_angle: int
    shoulder_angle: int
    elbow_angle: int
    wrist_angle: int
    gripper_angle: int


# globals
global arm, arduino_serial


@asynccontextmanager
async def lifespan(app: FastAPI):
    # Steps that will be performed on startup events only once.
    print("Starting up...")
    global arm, arduino_serial
    arm = Arm()
    arduino_serial = ArduinoSerialConnection("/dev/ttyUSB0")
    yield

    # Steps that will happen on shutdown event
    del arduino_serial  # closes serial connection
    print("Shutting down...")


app = FastAPI(root_path="/api", lifespan=lifespan)


@app.websocket("/wheels/move")
async def move_wheels(websocket: WebSocket):
    await websocket.accept()
    print("client connected to wheels websocket")
    try:
        while True:
            movement_packet = await websocket.receive_json()

            x = movement_packet["x"]
            y = movement_packet["y"]
            rotation = movement_packet["rotation"]

            arduino_serial.send(f"{x} {y} {rotation}")
    except (WebSocketDisconnect, ConnectionClosed):
        arduino_serial.send(f"0 0 0")  # stop motors
        print("client disconnected from wheels websocket")


@app.websocket("/arm/move")
async def move_arm(websocket: WebSocket):
    await websocket.accept()
    print("client connected to arm websocket")
    try:
        while True:
            movement_packet = await websocket.receive_json()

            arm_base_rotation = movement_packet["base_angle"]
            arm_shoulder_rotation = movement_packet["shoulder_angle"]
            arm_elbow_rotation = movement_packet["elbow_angle"]
            arm_wrist_rotation = movement_packet["wrist_angle"]
            arm_gripper_rotation = movement_packet["gripper_angle"]

            arm.move_base(arm_base_rotation)
            arm.move_shoulder(arm_shoulder_rotation)
            arm.move_elbow(arm_elbow_rotation)
            arm.move_wrist(arm_wrist_rotation)
            arm.move_gripper(arm_gripper_rotation)
    except (WebSocketDisconnect, ConnectionClosed):
        print("client disconnected from arm websocket")


@app.post("/arm/preset/default")
async def move_arm_preset():
    # arm.set_base_angle(0)
    arm.set_shoulder_angle(90)
    arm.set_elbow_angle(135)
    arm.set_wrist_angle(135)
    # arm.set_gripper_angle(0)

    return 0


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # very security :)
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.mount("/", StaticFiles(directory="./webui", html=True), name="static")
