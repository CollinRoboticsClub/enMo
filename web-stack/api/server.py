#!/usr/bin/env python3

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel

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
arduino_serial = ArduinoSerialConnection("/dev/ttyUSB0")
arm = Arm()
app = FastAPI(root_path="/api")


@app.post("/move-wheels")
async def move_wheels(movement_packet: WheelMovementPacket):
    # TODO: handle abrupt disconnect (pseudocode below)
    # currently, if the client abruptly disconnects and the last packet sent was
    # contained a non-zero value, the robot would continue that movement until
    # it eventually received something else once someone connected again.
    #
    #   if (no movement packet received within last X seconds):
    #       send "0 0 0" to make the robot stop moving
    #
    # Best way to handle this may be yet another producer-consumer where the producer
    # are the API endpoints and the consumer is an async queue that sends
    # the serial events as they're added. Because then it can have some sort of simple
    # global timer handling maybe. or I could do that here but that seems a bit messier.

    x = movement_packet.x
    y = movement_packet.y
    rotation = movement_packet.rotation
    arduino_serial.send(f"{x} {y} {rotation}")

    return 0


@app.post("/move-arm")
async def move_arm(movement_packet: ArmMovementPacket):
    arm_base_rotation = movement_packet.base_angle
    arm_shoulder_rotation = movement_packet.shoulder_angle
    arm_elbow_rotation = movement_packet.elbow_angle
    arm_wrist_rotation = movement_packet.wrist_angle
    arm_gripper_rotation = movement_packet.gripper_angle

    arm.move_base(arm_base_rotation)
    arm.move_shoulder(arm_shoulder_rotation)
    arm.move_elbow(arm_elbow_rotation)
    arm.move_wrist(arm_wrist_rotation)
    arm.move_gripper(arm_gripper_rotation)

    return 0


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # very security :)
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.mount("/", StaticFiles(directory="./webui", html=True), name="static")
