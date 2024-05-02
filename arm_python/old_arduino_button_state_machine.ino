// Button press -> change state machine -> move servo to one of a list of saved positions
#include "Servo.h"

// TODO: remove hardcoded 4 and 5...
int all_presets[][5] = {
  // Each array represents a preset.
  // { base, shoulder, elbow, wristPitch, gripper }
  { 45, 70, 70, 70, 120 },      //  home open
  { 45, 90, 90, 90, 80 },       //  home closed
  { 135, 100, 100, 100, 120 },  //  dropoff closed
  { 135, 120, 120, 120, 80 }    //  dropoff open
};

const int num_total_states = sizeof(all_presets) / sizeof(all_presets[0]);

int cur_state = 0;
const int BUTTON_PIN = 0;

Servo base;
Servo shoulder;
Servo elbow;
Servo wristPitch;
Servo gripper;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  base.attach(1);
  shoulder.attach(2);
  elbow.attach(3);
  wristPitch.attach(4);
  gripper.attach(5);

  moveTo(90, 90, 90, 90, 90);
}

void loop() {
  int button_pressed = digitalRead(BUTTON_PIN);

  if (button_pressed == HIGH) {
    // Blink the LED to show that the button has been pressed
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);

    // Depending on state, go to preset
    moveToArray(all_presets[cur_state]);

    Serial.print("We go to new state: ");
    Serial.println(cur_state);

    delay(1000);

    // Cycle state machine
    cur_state++;
    cur_state %= num_total_states;
  }


  delay(20);
}

void moveTo(int baseAngle, int shoulderAngle, int elbowAngle, int wristPitchAngle, int gripperAngle) {
  base.write(baseAngle);
  shoulder.write(shoulderAngle);
  elbow.write(elbowAngle);
  wristPitch.write(wristPitchAngle);
  gripper.write(gripperAngle);
}

// An alternate version of moveTo() that accepts an array is useful.
void moveToArray(int angles[]) {
  // Remember, arrays start indexing at 0.
  moveTo(angles[0], angles[1], angles[2], angles[3], angles[4]);
}
