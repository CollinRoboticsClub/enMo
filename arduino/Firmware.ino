// Include Libraries
#include "Arduino.h"
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


DCMDriverL298 frontMotorDriver(MOTORDRIVERL298_1_PIN_ENA,MOTORDRIVERL298_1_PIN_INT1,MOTORDRIVERL298_1_PIN_INT2,MOTORDRIVERL298_1_PIN_ENB,MOTORDRIVERL298_1_PIN_INT3,MOTORDRIVERL298_1_PIN_INT4);
DCMDriverL298 rearMotorDriver(MOTORDRIVERL298_2_PIN_ENA,MOTORDRIVERL298_2_PIN_INT1,MOTORDRIVERL298_2_PIN_INT2,MOTORDRIVERL298_2_PIN_ENB,MOTORDRIVERL298_2_PIN_INT3,MOTORDRIVERL298_2_PIN_INT4);

// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

void setup() {
	// Setup Serial which is useful for debugging
	// Use the Serial Monitor to view printed messages
	//Serial.begin(9600);
	//while (!Serial) ; // wait for serial port to connect. Needed for native USB
	//Serial.println("start");
	//menuOption = menu();
}

void loop() {
	//if(menuOption == '1') {
		// L298N Motor Driver with Dual Hobby DC motors #1 - Test Code
		// Start both motors. note that rotation direction is determined by the motors connection to the driver.
		// You can change the speed by setting a value between 0-255, and set the direction by changing between 1 and 0.
		frontMotorDriver.setMotorA(200,1);
		frontMotorDriver.setMotorB(200,0);
		delay(2000);
		
		//Stop both motors
		frontMotorDriver.stopMotors();
		delay(2000);
		
	//} else if(menuOption == '2') {
		// L298N Motor Driver with Dual Hobby DC motors #2 - Test Code
		// Start both motors. note that rotation direction is determined by the motors connection to the driver.
		// You can change the speed by setting a value between 0-255, and set the direction by changing between 1 and 0.
		rearMotorDriver.setMotorA(200,1);
		rearMotorDriver.setMotorB(200,0);
		delay(2000);

		//Stop both motors
		rearMotorDriver.stopMotors();
		delay(2000);
		//
	//}

	//if (millis() - time0 > timeout) {
		//menuOption = menu();
	//}

}

// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
//char menu() {
//
//    Serial.println(F("\nWhich component would you like to test?"));
//    Serial.println(F("(1) L298N Motor Driver with Dual Hobby DC motors #1"));
//    Serial.println(F("(2) L298N Motor Driver with Dual Hobby DC motors #2"));
//    Serial.println(F("(menu) send anything else or press on board reset button\n"));
//    while (!Serial.available());
//
//    // Read data from serial monitor if received
//    while (Serial.available()) 
//    {
//        char c = Serial.read();
//        if (isAlphaNumeric(c)) 
//        {
//
//            if(c == '1') 
//    			Serial.println(F("Now Testing L298N Motor Driver with Dual Hobby DC motors #1"));
//    		else if(c == '2') 
//    			Serial.println(F("Now Testing L298N Motor Driver with Dual Hobby DC motors #2"));
//            else
//            {
//                Serial.println(F("illegal input!"));
//                return 0;
//            }
//            time0 = millis();
//            return c;
//        }
//    }
//}
