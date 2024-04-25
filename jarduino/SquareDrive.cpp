#pragma once
#include "Motor.cpp"

class SquareDrive {
public:
    Motor front, right, left, back;

    SquareDrive(Motor front, Motor right, Motor left, Motor back)
        : front(front), right(right), left(left), back(back) {
    }

    void drive(double x, double y, double rotation) {
        // IT'S ALL MECANUM???
        left.setSpeed(y + x + rotation);
        right.setSpeed(y - x - rotation);
        front.setSpeed(y - x + rotation);
        back.setSpeed(y + x - rotation);
        // It always has been :,)

        //     left.setSpeed(y);
        //     right.setSpeed(y);
        //     front.setSpeed(x);
        //     back.setSpeed(x);
    }

    // Field-oriented drive is possible... TODO...? Lol, never!
};