#!/usr/bin/env python3

import serial
import uvicorn
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel


class MovementPacket(BaseModel):
    x: float
    y: float
    rotation: float


def arduino_send(input: str):
    arduino.write(f"{input}\n".encode())


def arduino_send_arr(input_arr: list[str]):
    for input in input_arr:
        arduino_send(input)


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
    # I think the pi is gonna directly be communicating with the arm via its pins, so I'll
    # probably have to mess with that to get this part working

    return 0


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # very security :)
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.mount("/", StaticFiles(directory="./webui", html=True), name="static")

arduino = serial.Serial(port="/dev/ttyUSB0", baudrate=115200, timeout=0.5)

if __name__ == "__main__":
    uvicorn.run("server:app", reload=True, host="0.0.0.0", port=8000)
