// When uploading on Josh's Nano boards, make sure to select ATMega328P (Old Bootloader) in the Tools > Processor menu.

#include "Motor.cpp"
#include "SquareDrive.cpp"

// The pin orders are ENA, ENB, and IN, for each motor
// FIXME Adjust it!
Motor front(3, 2, 4, false, Motor::BRAKE, 0.2);
Motor back(9, 12, 13, true, Motor::BRAKE, 0.2);
Motor left(6, 10, 11, false, Motor::BRAKE, 0.2);
Motor right(5, 7, 8, true, Motor::BRAKE, 0.2);

SquareDrive drive(front, right, left, right);

double left = 0, right = 0;

void setup() {
    Serial.begin(9600);
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


    //     // front.setSpeed(y);
    //     // right.setSpeed(y);
    //     // left.setSpeed(y);
    //     // right.setSpeed(y);

    //     tankdrive.tankDrive(left, right);
    //     // delay(20000);
    //     // drive.setSpeed(0, 0, 0);
    // }
    // front.setSpeed(0.4);
    // delay(500);

    // right.setSpeed(0.4);
    // delay(500);

    // left.setSpeed(0.4);
    // delay(500);

    // right.setSpeed(0.4);
    // delay(500);

    // drive.tankDrive(left, right);
    // left += 0.05;
    // right += 0.05;

    delay(20);
}