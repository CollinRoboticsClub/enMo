#include <Servo.h>

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

Vector3 planeCoordinates = { 55, 0, 0 };
Vector3 referenceCoordinates = { 55, 0, 0 };

// float x = 55;   //plane coordinates
// float xo = 55;  //reference for change in position
// float y = 0;
// float yo = 0;
// float z = 0;
// float zo = 0;
// float r = 0;
// float ro = 0;
float sidea = 150;    //Length of forearm
float sideb = 100;    //Length of shoulder
float sidec = 30;     //initial distance of toolhead from origin
float sidect = 30;    //used in calculating z axis
float angleA = 0.00;  //Angles of respective sides
float angleB = 0.00;
float angleC = 0.00;
float angleY = 0.00;
float angleZ = 0.00;
int servold[] = { 0, 0, 0, 0 };     //reference for last known good coordinates
int servopin[] = { 3, 5, 6, 9 };    //PWM servo pins
int servopos[] = { 0, 0, 90, 90 };  //initial servo positions
int xj = 0;                         //raw values from joysticks
int yj = 0;
int zj = 0;
int rj = 0;
int xs = 0;  //mapped value from joysticks
int ys = 0;
int zs = 0;
int rs = 0;
int xadj = 0;  //calibration adjustment for joysticks
int yadj = 0;
int zadj = 0;
int radj = 0;
float P = 2;  //PID multiplicants
float I = .05;
float D = .001;
int del = 10; //delay length (needed for PID calculations)
int xe = 0;   //x, y, z, and r variables for PID control
int xp = 0;
int xi = 0;
int xt = 0;
int xd = 0;
int xso = 0;
int ye = 0;
int yp = 0;
int yi = 0;
int yt = 0;
int yd = 0;
int yso = 0;
int ze = 0;
int zp = 0;
int zi = 0;
int zt = 0;
int zd = 0;
int zso = 0;
int re = 0;
int rp = 0;
int ri = 0;
int rt = 0;
int rd = 0;
int rso = 0;
Servo servoA;  //servo assignment
Servo servoB;
Servo servoC;
Servo servoY;

void setup() {
    // put your setup code here, to run once:

    Serial.begin(9600);          //serial connection for debugging
    servoA.attach(servopin[0]);  //servo attachments
    servoC.attach(servopin[1]);
    servoB.attach(servopin[2]);
    servoY.attach(servopin[3]);
    changeX();           //run initial calculations for angles at startup position
    xj = analogRead(0);  //read joystick position
    yj = analogRead(1);
    zj = analogRead(2);
    rj = analogRead(3);
    xadj = 500 - xj;  //determine joystick error and assign to adj value
    yadj = 500 - yj;
    zadj = 500 - zj;
    radj = 500 - rj;
    pinMode(12, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);
}

void loop() {
    // put your main code here, to run repeatedly:

    for (int idx = 0; idx < 4; idx++) {  //if servo position is valid, store to last known good, if invalid, load last known good instead
        if (0 < servopos[idx] && servopos[idx] < 181) { servold[idx] = servopos[idx]; }
        if (servopos[idx] < 1 || servopos[idx] > 180) { servopos[idx] = servold[idx]; }
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

    if (x != xo || y != yo || z != zo || r != ro) {  //if any coordinate has changed, run position calculations
        changeX();
    }

    if (digitalRead(12) == 0 && digitalRead(11) == 0) {  //if both stick buttons are pressed, return to home position
        x = 55;
        xo = 55;
        y = 0;
        yo = 0;
        z = 0;
        zo = 0;
        r = 0;
        ro = 0;
        changeX();
    }
    xj = analogRead(0);  //read joystick locations
    yj = analogRead(1);
    zj = analogRead(2);
    rj = analogRead(3);
    xs = map((xj + xadj - 500), -500, 500, -12, 12);  //map joystick locations to a value to apply to respective coordinate
    ys = map((yj + yadj - 500), -500, 500, 12, -12);
    zs = map((zj + zadj - 500), -500, 500, -12, 12);
    rs = map((rj + radj - 500), -500, 500, 12, -12);

    xe = 0 - (xs / 3);    //PID calculations
    xp = xe;
    xi = xi + (xe * (del / 1000));
    xd = (xe - xso) / (0.01);
    xso = xe;
    ye = 0 - (ys / 3);
    yp = ye;
    yi = yi + (ye * (del / 1000));
    yd = (ye - yso) / (0.01);
    yso = ye;
    ze = 0 - (zs / 3);
    zp = ze;
    zi = zi + (ze * (del / 1000));
    zd = (ze - zso) / (0.01);
    zso = ze;
    re = 0 - (rs / 3);
    rp = re;
    ri = ri + (re * (del / 1000));
    rd = (re - rso) / (0.01);
    rso = re;
    x = x + (xp * P) + (xi * I) + (xd * D);  //add PID calculations to axis
    y = y + (yp * P) + (yi * I) + (yd * D);
    z = z + (zp * P) + (zi * I) + (zd * D);
    r = r + (rp * P) + (ri * I) + (rd * D);
    delay(del);  //delay for servo movement
}

void changeX() {                                                                                                           //function to calculate angles from desired coordinate
    sidect = abs(sqrt(sq(x) + sq(y)));                                                                                       //calculate side c in respect to x,y axis
    sidec = abs(sqrt(sq(sidect) + sq(z)));                                                                                   //calculate side c in respect to z axis and x,y calculation
    angleA = constrain((180 / PI) * (acos((sideb * sideb + sidec * sidec - sidea * sidea) / (2 * sideb * sidec))), 1, 180);  //calculate angles from x,y,z and c length
    angleC = constrain((180 / PI) * (acos((sq(sidea) + sq(sideb) - sq(sidec)) / (2 * sidea * sideb))), 1, 180);
    angleB = constrain((180 / PI) * (acos((sq(sidea) + sq(sidec) - sq(sideb)) / (2 * sidea * sidec))), 1, 180);
    servopos[0] = constrain(angleA + (angleA / 30) + ((180 / PI) * (asin(z / sidec))), 1, 180);  //assign values to servo positions
    servopos[1] = map(angleC, 1, 180, 180, 1);
    servopos[2] = angleB - ((180 / PI) * (asin(z / sidec)));
    servopos[3] = r + constrain(90 + ((180 / PI) * (asin(y / sidec))), 1, 180);
    xo = x;  //clear coordinate change marker
    yo = y;
    zo = z;
    ro = r;
}
