#pragma once
#include "Motor.cpp"

class SquareDrive {
public:
    Motor front, right, left, back;

    SquareDrive(Motor front, Motor right, Motor left, Motor back)
        : front(front), right(right), left(left), back(back) {
    }

    void setSpeed(double x, double y, double rotation) {
        left.setSpeed(y + rotation);
        right.setSpeed(y - rotation);
        front.setSpeed(x + rotation);
        back.setSpeed(x - rotation);
    }

    // TODO: Field-oriented drive is possible... TODO...? Lol, never!
};