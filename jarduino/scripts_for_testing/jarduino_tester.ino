// When uploading on Josh's Nano boards, make sure to select ATMega328P (Old Bootloader) in the Tools > Processor menu.

// ---
// Feel free to copy-paste this testing script back into the main directory as required!
// ---

#include "drives/Motor.cpp"
#include "drives/SquareDrive.cpp"

// refer: ./sqaure_drive_pins_from_aubrey_discord.png
Motor front(3, 2, 4, true, Motor::BRAKE, 0.1); // m4
Motor back(11, 12, 13, true, Motor::BRAKE, 0.1); // m3
Motor left(9, 8, 10, true, Motor::BRAKE, 0.1); // m2
Motor right(5, 6, 7, true, Motor::BRAKE, 0.1); // m1

SquareDrive setSpeed(front, right, left, back);

// double left = 0, right = 0;

void setup() {
    delay(5000);
    Serial.begin(9600);
    while (Serial.available() == 0) {}
}

void loop() {
    Serial.println("Enter your name to test all the motors: ");

    // block until user hits Enter :
    while (Serial.available() == 0) {}
    Serial.readString();
    // sorry yeah irdk how else the serial lib wants me to do that LOL

    Serial.println("right.setSpeed(0.4);");
    delay(1000);
    right.setSpeed(0.4);
    delay(1000);
    right.setSpeed(0.0);
    delay(1000);
    Serial.println("right complete");
    delay(1000);

    Serial.println("left.setSpeed(0.4);");
    delay(1000);
    left.setSpeed(0.4);
    delay(1000);
    left.setSpeed(0.0);
    delay(1000);
    Serial.println("left complete");
    delay(1000);

    Serial.println("back.setSpeed(0.4);");
    delay(1000);
    back.setSpeed(0.4);
    delay(1000);
    back.setSpeed(0.0);
    delay(1000);
    Serial.println("back complete");
    delay(1000);

    Serial.println("front.setSpeed(0.4);");
    delay(1000);
    front.setSpeed(0.4);
    delay(1000);
    front.setSpeed(0.0);
    delay(1000);
    Serial.println("front complete");
    delay(1000);

    delay(50);
}