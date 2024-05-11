// When uploading on Josh's Nano boards, make sure to select ATMega328P (Old Bootloader) in the Tools > Processor menu.

#include "Motor.cpp"
#include "SquareDrive.cpp"

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

        setSpeed.setSpeed(x, y, rotation);
        // Uncomment the following 2 lines if you're just trying to test over serial monitor
        // and are just typing in inputs!
        // delay(5000);
        // setSpeed.setSpeed(0, 0, 0);
    }

    // Serial.println("Enter your name to test all the motors: ");

    //  block until user hits Enter:
    // while (Serial.available() == 0) {}
    // Serial.readString();
    // sorry yeah irdk how else the serial lib wants me to do that LOL

    // Serial.println("right.setSpeed(0.4);");
    // delay(1000);
    // right.setSpeed(0.4);
    // delay(1000);
    // right.setSpeed(0.0);
    // delay(1000);
    // Serial.println("right complete");
    // delay(1000);

    // Serial.println("left.setSpeed(0.4);");
    // delay(1000);
    // left.setSpeed(0.4);
    // delay(1000);
    // left.setSpeed(0.0);
    // delay(1000);
    // Serial.println("left complete");
    // delay(1000);

    // Serial.println("back.setSpeed(0.4);");
    // delay(1000);
    // back.setSpeed(0.4);
    // delay(1000);
    // back.setSpeed(0.0);
    // delay(1000);
    // Serial.println("back complete");
    // delay(1000);

    // Serial.println("front.setSpeed(0.4);");
    // delay(1000);
    // front.setSpeed(0.4);
    // delay(1000);
    // front.setSpeed(0.0);
    // delay(1000);
    // Serial.println("front complete");
    // delay(1000);


    delay(50);
}