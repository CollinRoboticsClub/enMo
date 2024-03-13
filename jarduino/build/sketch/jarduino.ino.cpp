#include <Arduino.h>
#line 1 "C:\\Users\\joshu\\OneDrive\\Documents\\GitHub\\chessbot-pi-code\\jarduino\\jarduino.ino"
// When uploading on Josh's Nano boards, make sure to select ATMega328P (Old Bootloader) in the Tools > Processor menu.

#include "Motor.cpp"
#include "MecanumDrive.cpp"

// If you ever change these: the EN pins must be PWM pins.
const int FRONT_ENA = 3, FRONT_IN1 = 2, FRONT_IN2 = 4;
const int FRONT_ENB = 5, FRONT_IN3 = 7, FRONT_IN4 = 8;
const int BACK_ENB = 9, BACK_IN1 = 10, BACK_IN2 = 11;
const int BACK_ENA = 6, BACK_IN3 = 12, BACK_IN4 = 13;

Motor frontLeft(FRONT_ENA, FRONT_IN1, FRONT_IN2);
Motor frontRight(FRONT_ENB, FRONT_IN3, FRONT_IN4);
Motor backLeft(BACK_ENA, BACK_IN1, BACK_IN2);
Motor backRight(BACK_ENB, BACK_IN3, BACK_IN4);

MecanumDrive drive(frontLeft, frontRight, backLeft, backRight);


#line 20 "C:\\Users\\joshu\\OneDrive\\Documents\\GitHub\\chessbot-pi-code\\jarduino\\jarduino.ino"
void setup();
#line 24 "C:\\Users\\joshu\\OneDrive\\Documents\\GitHub\\chessbot-pi-code\\jarduino\\jarduino.ino"
void loop();
#line 20 "C:\\Users\\joshu\\OneDrive\\Documents\\GitHub\\chessbot-pi-code\\jarduino\\jarduino.ino"
void setup() {
    Serial.begin(9600);
}

void loop() {
    // while (true) {
    //     Serial.println("Hello, world!");
    //     delay(1000);
    // }
    if (Serial.available() > 0) {
        float x = Serial.parseFloat();
        float y = Serial.parseFloat();
        float rotation = Serial.parseFloat();

        Serial.print("x: ");
        Serial.print(x);
        Serial.print(", y: ");
        Serial.print(y);
        Serial.print(", rotation: ");
        Serial.println(rotation);


        frontLeft.setSpeed(y);
        frontRight.setSpeed(y);
        backLeft.setSpeed(y);
        backRight.setSpeed(y);

        // drive.setSpeed(x, y, rotation);
        delay(20000);
        drive.setSpeed(0, 0, 0);
    }
}
