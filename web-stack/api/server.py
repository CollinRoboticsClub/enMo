#!/usr/bin/env python3

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel

from arduino_serial_utils import ArduinoSerialConnection


class MovementPacket(BaseModel):
    x: float
    y: float
    rotation: float


# globals
arduino_serial = ArduinoSerialConnection("/dev/ttyUSB0")
app = FastAPI(root_path="/api")


@app.post("/move-wheels")
async def move_wheels(movement_packet: MovementPacket):
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
async def move_arm(movement_packet: MovementPacket):
    x = movement_packet.x
    y = movement_packet.y

    # TODO: implement this

    return 0


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # very security :)
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.mount("/", StaticFiles(directory="./webui", html=True), name="static")
