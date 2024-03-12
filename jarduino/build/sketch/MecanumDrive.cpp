#line 1 "C:\\Users\\joshu\\OneDrive\\Documents\\GitHub\\chessbot-pi-code\\jarduino\\MecanumDrive.cpp"
#pragma once
#include "Motor.cpp"

// Source: Method 1 in https://www.youtube.com/watch?v=gnSW2QpkGXQ

class MecanumDrive {
public:
    Motor frontLeft, frontRight, backLeft, backRight;

    MecanumDrive(Motor frontLeft, Motor frontRight, Motor backLeft, Motor backRight)
        : frontLeft(frontLeft), frontRight(frontRight), backLeft(backLeft), backRight(backRight) {
    }

    void setSpeed(double x, double y, double rotation) {
        frontLeft.setSpeed(y + x + rotation);
        frontRight.setSpeed(y - x - rotation);
        backLeft.setSpeed(y - x + rotation);
        backRight.setSpeed(y + x - rotation);
    }
};