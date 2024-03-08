// Include Libraries
#include "DCMDriverL298.h"

// Pin Definitions
#define MOTORDRIVER_1_PIN_INT1	2
#define MOTORDRIVER_1_PIN_ENB 	5
#define MOTORDRIVER_1_PIN_INT2	4
#define MOTORDRIVER_1_PIN_ENA 	3
#define MOTORDRIVER_1_PIN_INT3	7
#define MOTORDRIVER_1_PIN_INT4	8

#define MOTORDRIVER_2_PIN_INT1	10
#define MOTORDRIVER_2_PIN_ENB 	9
#define MOTORDRIVER_2_PIN_INT2	11
#define MOTORDRIVER_2_PIN_ENA 	6
#define MOTORDRIVER_2_PIN_INT3	12
#define MOTORDRIVER_2_PIN_INT4	13

DCMDriverL298 frontMotorDriver(
	MOTORDRIVER_1_PIN_ENA,
	MOTORDRIVER_1_PIN_INT1,
	MOTORDRIVER_1_PIN_INT2,
	MOTORDRIVER_1_PIN_ENB,
	MOTORDRIVER_1_PIN_INT3,
	MOTORDRIVER_1_PIN_INT4
);
DCMDriverL298 rearMotorDriver(
	MOTORDRIVER_2_PIN_ENA,
	MOTORDRIVER_2_PIN_INT1,
	MOTORDRIVER_2_PIN_INT2,
	MOTORDRIVER_2_PIN_ENB,
	MOTORDRIVER_2_PIN_INT3,
	MOTORDRIVER_2_PIN_INT4
);

// TODO: make code below actually use these constants
// motor constants
const int MAX_SPEED = 0;
const int NORMAL_SPEED = 0;
const int OFF_SPEED = 0;
const int FORWARD = 0; // TODO: figure out if this is correct
const int BACKWARD = 1; // TODO: figure out if this is correct

// define vars for testing menu
const int TIMEOUT = 10000;
char menuOption = 0;
long time0;

// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu() {
	Serial.println(F("\nWhich component would you like to test?"));
	Serial.println(F("(1) L298N Motor Driver with Dual Hobby DC motors #1"));
	Serial.println(F("(2) L298N Motor Driver with Dual Hobby DC motors #2"));
	Serial.println(F("(menu) send anything else or press on board reset button\n"));
	while (!Serial.available());

	// Read data from serial monitor if received
	while (Serial.available()) {
		char c = Serial.read();
		if (isAlphaNumeric(c)) {
			if(c == '1') {
				Serial.println(F("Now Testing L298N Motor Driver with Dual Hobby DC motors #1"));
			} else if(c == '2') {
				Serial.println(F("Now Testing L298N Motor Driver with Dual Hobby DC motors #2"));
			} else {
				Serial.println(F("illegal input!"));
				return 0;
			}
			time0 = millis();
			return c;
		}
	}
}

void setup() {
	// Setup Serial which is useful for debugging
	//Serial.begin(9600);
	//while (!Serial) ; // wait for serial port to connect. Needed for native USB

	//Serial.println("start");
	//menuOption = menu();
}

void loop() {
	//if(menuOption == '1') {
		// Start both motors. note that rotation direction is determined by the motors connection to the driver.
		// You can change the speed by setting a value between 0-255, and set the direction by changing between 1 and 0.
		frontMotorDriver.setMotorA(200,1);
		frontMotorDriver.setMotorB(200,0);
		delay(2000);

		//Stop both motors
		frontMotorDriver.stopMotors();
		delay(2000);

	//} else if(menuOption == '2') {
		rearMotorDriver.setMotorA(200,1);
		rearMotorDriver.setMotorB(200,0);
		delay(2000);

		//Stop both motors
		rearMotorDriver.stopMotors();
		delay(2000);
		//
	//}

	//if (millis() - time0 > TIMEOUT) {
		//menuOption = menu();
	//}
}