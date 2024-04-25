#include <Servo.h>
float x = 55;   //plane coordinates
float xo = 55;  //reference for change in position
float y = 0;
float yo = 0;
float z = 0;
float zo = 0;
float r = 0;
float ro = 0;
float sidea = 150;  //Length of forearm
float sideb = 100;  //Length of shoulder
float sidec = 55;   //initial distance of toolhead from origin
float sidect = 55;  //used in calculating z axis
float angleA = 0.00;  //Angles of respective sides
float angleB = 0.00;
float angleC = 0.00;
float angleY = 0.00;
float angleZ = 0.00;
int servold[] = { 0, 0, 0, 0 }; //reference for last known good coordinates
int servopin[] = { 3, 5, 6, 9 };  //PWM servo pins
int servopos[] = { 0, 0, 0, 90 }; //initial servo positions
int idx = 0;  //index for counting
int xj = 0; //raw values from joysticks
int yj = 0;
int zj = 0;
int rj = 0;
int xs = 0; //mapped value from joysticks
int ys = 0;
int zs = 0;
int rs = 0;
int xadj = 0; //calibration adjustment for joysticks
int yadj = 0;
int zadj = 0;
int radj = 0;
Servo servoA; //servo assignment
Servo servoB;
Servo servoC;
Servo servoY;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); //serial connection for debugging
  servoA.attach(servopin[0]); //servo attachments
  servoC.attach(servopin[1]);
  servoB.attach(servopin[2]);
  servoY.attach(servopin[3]);
  changeX();  //run initial calculations for angles at startup position
  xj = map(analogRead(0), 1, 1024, 0, 500); //read joystick position
  yj = map(analogRead(1), 1, 1024, 0, 500);
  zj = map(analogRead(2), 1, 1024, 0, 500);
  rj = map(analogRead(3), 1, 1024, 0, 500);
  xadj = 250 - xj;  //determine joystick error and assign to adj value
  yadj = 250 - yj;
  zadj = 250 - zj;
  radj = 250 - rj;
}

void loop() {
  // put your main code here, to run repeatedly:

  for (idx = 0; idx < 4; idx++) {   //if servo position is valid, store it last known good, if invalid, load last know good instead
    if (servopos[idx] > 0) { servold[idx] = servopos[idx]; }
    if (servopos[idx] < 1) { servopos[idx] = servold[idx]; }
  }
  servoA.write(servopos[0]);  //write positions to servos
  servoC.write(servopos[1]);
  servoB.write(servopos[2]);
  servoY.write(servopos[3]);

  Serial.print(x);  //print coordinates and servo positions for debugging
  Serial.print("  ");
  Serial.print(y);
  Serial.print("  ");
  Serial.print(z);
  Serial.print("  ");
  Serial.print(servopos[0]);
  Serial.print("  ");
  Serial.print(servopos[1]);
  Serial.print("  ");
  Serial.print(servopos[2]);
  Serial.print("  ");
  Serial.println(servopos[3]);

  if (x != xo || y != yo || z != zo || r != ro) {   //if any coordinate has changed, run position calculations
    changeX();
  }

  xj = map(analogRead(0), 1, 1024, 0, 500); //read joystick locations
  yj = map(analogRead(1), 1, 1024, 0, 500);
  zj = map(analogRead(2), 1, 1024, 0, 500);
  rj = map(analogRead(3), 1, 1024, 0, 500);
  xs = map((xj + xadj - 250), -250, 250, 10, -10);  //map joystick locations to a value to apply to respective coordinate
  ys = map((yj + yadj - 250), -250, 250, -10, 10);
  zs = map((zj + zadj - 250), -250, 250, 10, -10);
  rs = map((rj + radj - 250), -250, 250, -10, 10);

  x = x + xs / 5;   //add joystick value to coordinate, divided by five to provide dead zone and more reliable funciton
  y = y + ys / 5;
  z = z + zs / 5;
  r = r + rs / 5;

  delay(10);  //delay for servo movement
}

void changeX() {  //function to calculate angles from desired coordinate
  sidect = abs(sqrt(sq(x) + sq(y)));  //calculate side c in respect to x,y axis
  sidec = abs(sqrt(sq(sidect) + sq(z)));  //calculate side c in respect to z axis and x,y calculation
  angleA = constrain((180 / PI) * (acos((sideb * sideb + sidec * sidec - sidea * sidea) / (2 * sideb * sidec))), 10, 170);  //calculate angles from x,y,z and c length
  angleC = constrain((180 / PI) * (acos((sq(sidea) + sq(sideb) - sq(sidec)) / (2 * sidea * sideb))), 10, 170);
  angleB = constrain((180 / PI) * (acos((sq(sidea) + sq(sidec) - sq(sideb)) / (2 * sidea * sidec))), 10, 170);
  servopos[0] = constrain(angleA + ((180 / PI) * (asin(z / sidec))), 10, 170);  //assign values to servo positions
  servopos[1] = angleC;
  servopos[2] = angleB;
  servopos[3] = r + constrain(90 + ((180 / PI) * (asin(y / sidec))), 10, 170);
  xo = x;   //clear coordinate change marker
  yo = y;
  zo = z;
  ro = r;
}


