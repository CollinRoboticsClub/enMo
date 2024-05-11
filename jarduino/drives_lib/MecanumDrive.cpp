#pragma once
#include "Motor.cpp"

// Source: Method 1 in https://www.youtube.com/watch?v=gnSW2QpkGXQ
// BY THE WAY, this works for drivetrains where the Omni wheels are on the *corners*!
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