#pragma once
#include "Motor.cpp"

class TankDrive {
public:
    Motor frontLeft, frontRight, backLeft, backRight;

    TankDrive(Motor frontLeft, Motor frontRight, Motor backLeft, Motor backRight)
        : frontLeft(frontLeft), frontRight(frontRight), backLeft(backLeft), backRight(backRight) {
    }

    void arcadeDrive(double forward, double rotation) {
        frontLeft.setSpeed(forward + rotation);
        backLeft.setSpeed(forward + rotation);

        frontRight.setSpeed(forward - rotation);
        backRight.setSpeed(forward - rotation);
    }

    void tankDrive(double left, double right) {
        frontLeft.setSpeed(left);
        backLeft.setSpeed(left);

        frontRight.setSpeed(right);
        backRight.setSpeed(right);
    }
};