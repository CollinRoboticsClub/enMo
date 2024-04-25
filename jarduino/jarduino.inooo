// When uploading on Josh's Nano boards, make sure to select ATMega328P (Old Bootloader) in the Tools > Processor menu.

#include "Motor.cpp"
#include "TankDrive.cpp"

// If you ever change these: the EN pins must be PWM pins.
const int FRONT_ENA = 3, FRONT_IN1 = 2, FRONT_IN2 = 4;
const int FRONT_ENB = 5, FRONT_IN3 = 7, FRONT_IN4 = 8;
const int BACK_ENB = 9, BACK_IN1 = 10, BACK_IN2 = 11;
const int BACK_ENA = 6, BACK_IN3 = 12, BACK_IN4 = 13;

Motor frontLeft(FRONT_ENA, FRONT_IN1, FRONT_IN2, true);
// Motor frontRight(FRONT_ENB, FRONT_IN3, FRONT_IN4, false);
// Motor backLeft(BACK_ENA, BACK_IN1, BACK_IN2, true);
// Motor backRight(BACK_ENB, BACK_IN3, BACK_IN4, false);


Motor frontRight(BACK_ENB, BACK_IN3, BACK_IN4, false);
Motor backLeft(FRONT_ENB, FRONT_IN3, FRONT_IN4, true);
Motor backRight(BACK_ENA, BACK_IN1, BACK_IN2, false);

TankDrive drive(frontLeft, frontRight, backLeft, backRight);

double left = 0, right = 0;


void setup() {
    // Serial.begin(9600);
}

void loop() {
    // while (true) {
    //     Serial.println("Hello, world!");
    //     delay(1000);
    // }
    // if (Serial.available() > 0) {
    //     double left = Serial.parseDouble();
    //     double right = Serial.parseDouble();

    //     Serial.print("left: ");
    //     Serial.print(left);
    //     Serial.print("right: ");
    //     Serial.println(right);


    //     // frontLeft.setSpeed(y);
    //     // frontRight.setSpeed(y);
    //     // backLeft.setSpeed(y);
    //     // backRight.setSpeed(y);

    //     tankdrive.tankDrive(left, right);
    //     // delay(20000);
    //     // drive.setSpeed(0, 0, 0);
    // }
    // frontLeft.setSpeed(0.4);
    // delay(500);

    // frontRight.setSpeed(0.4);
    // delay(500);

    // backLeft.setSpeed(0.4);
    // delay(500);

    // backRight.setSpeed(0.4);
    // delay(500);

    drive.tankDrive(left, right);
    left += 0.05;
    right += 0.05;

    delay(10);
}