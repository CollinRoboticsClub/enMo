#!/usr/bin/env python3

from pydantic import BaseModel
import uvicorn
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
import serial

class MovementPacket(BaseModel):
    left: bool
    right: bool
    up: bool
    down: bool
    speed: float # speed factor, [0,1]


app = FastAPI(root_path="/api")

@app.post("/move")
async def move(movement_packet: MovementPacket):
    left = movement_packet.left
    right = movement_packet.right
    up = movement_packet.up
    down = movement_packet.down
    speed = movement_packet.speed

    # TODO: call serial com functions here
    if False:
        return 1

    return 0

def setup_fastapi():
    app.add_middleware(
        CORSMiddleware,
        allow_origins=["*"], # very security :)
        allow_credentials=True,
        allow_methods=["*"],
        allow_headers=["*"],
    )

    app.mount("/", StaticFiles(directory="../webui", html=True), name="static")

if __name__ == "__main__":
    setup_fastapi()
    arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.5)
    uvicorn.run(
        "server:app",
        reload=True,
        host="0.0.0.0",
        port=8000
    )
