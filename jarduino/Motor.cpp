// This class is for L298N and similar motor drivers.
// However, it lowers the PWM frequency to have better torque at low speeds.
// Additionally, it uses the controller's ability to have a brake and coast mode.

// References:
// https://www.dprg.org/l298n-motor-driver-board-drive-modes/ for brake and coast mode
// https://learn.adafruit.com/improve-brushed-dc-motor-performance/choosing-decay-mode-and-pwm-frequency for PWM frequency and how it affects motors
// https://domoticx.com/arduino-pwm-frequency-and-timers/ for setting PWM frequency on an Arduino Uno (nano is identical)

// Note that setting frequency registers is microcontroller-specific.
// This code assumes an ATmega8, 168, or 328P -- used on the Arduino Uno and Nano.
#pragma once

#include "Arduino.h"
// #include <avr/iom328p.h>
// #include <avr/io.h>


class Motor {
public:
    enum BrakeMode { COAST, BRAKE };

    int enPin, in1Pin, in2Pin;
    BrakeMode brakeMode;
    bool reversed;
    double deadzone;

    /// @brief Constructor
    /// @param enPin: PWM pin
    /// @param in1Pin: Digital pin
    /// @param in2Pin: Digital pin
    /// @param brakeMode: COAST or BRAKE
    /// @param reversed: If true, spinning direction is reversed
    /// @param deadzone: Speed below this value is considered 0. Helps avoid the buzzing noise.
    Motor(int enPin, int in1Pin, int in2Pin, BrakeMode brakeMode = BRAKE, bool reversed = false, double deadzone = 0) :
        enPin(enPin), in1Pin(in1Pin), in2Pin(in2Pin), brakeMode(brakeMode), reversed(reversed), deadzone(deadzone) {
        pinMode(enPin, OUTPUT);
        pinMode(in1Pin, OUTPUT);
        pinMode(in2Pin, OUTPUT);

        setSpeed(0);

        setPWMFrequencyToLowest(enPin);
    };

    /// @brief Sets the speed of the motor.
    /// @param speed: -1.0 to 1.0
    void setSpeed(double speed) {
        // Restrict speed to -1.0 to 1.0 in the event it came out of bounds
        speed = constrain(speed, -1.0, 1.0);

        // Apply deadzone
        if (abs(speed) < deadzone) {
            speed = 0;
        }

        // Apply reversal
        if (reversed) {
            speed = -speed;
        }

        // Set direction
        if (speed > 0) {
            digitalWrite(in1Pin, HIGH);
            digitalWrite(in2Pin, LOW);
        }
        else if (speed < 0) {
            digitalWrite(in1Pin, LOW);
            digitalWrite(in2Pin, HIGH);
        }
        else {
            if (brakeMode == BRAKE) {
                digitalWrite(in1Pin, HIGH);
                digitalWrite(in2Pin, HIGH);
            }
            else {
                digitalWrite(in1Pin, LOW);
                digitalWrite(in2Pin, LOW);
            }
        }

        // Convert 0 to 1.0 to 0 to 255, and write speed to PWM pin
        analogWrite(enPin, abs(speed * 255));
    }

    void setPWMFrequencyToLowest(int pwmPin) {
//         // FIXME: this doesn't COMPILE if you don't do the #define hack lol
//         // the timer register definitions are in magical arduino land in <avr/iom328p.h>
//         // and you can't include them twice... i just copy-pasted the defines from that ^ file
#define TCCR0B _SFR_IO8(0x25)
#define TCCR1B _SFR_MEM8(0x81)
#define TCCR2B _SFR_MEM8(0xB1)
        // ALSO, modifying timer 0 affects millis() and delay, which breaks WAY too much lol
        // It may not be that big of a deal :)

//         // source: https://domoticx.com/arduino-pwm-frequency-and-timers/

        switch (pwmPin) {
        case 3:
        case 11:
            TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
            break;
        case 5:
        case 6:
//             // NOTE: Changing this timer 0 affects millis() and delay!
//             TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz
            break;
        case 9:
        case 10:
            TCCR1B = TCCR1B & B11111000 | B00000101;    // set timer 1 divisor to  1024 for PWM frequency of    30.64 Hz
            break;
        default:
            break;
        }
    }
};