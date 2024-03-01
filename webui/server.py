#!/usr/bin/env python3

from pydantic import BaseModel
import uvicorn
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

class MovementPacket(BaseModel):
    left: bool
    right: bool
    up: bool
    down: bool
    speed: float # speed factor, [0,1]

@app.post("/move/")
async def move(movement_packet: MovementPacket):
    # TODO:
    if False:
        return 1

    return 0


origins = [
    "*", # security :)
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
