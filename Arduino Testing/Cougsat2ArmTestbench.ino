#include <Servo.h>
#include <elapsedMillis.h>

#define SHOULDER_SERVO_PIN 10
#define ELBOW_SERVO_PIN 11
#define ROTATION_SERVO_PIN 12
#define JOYSTICK_X_PIN A2
elapsedMillis timer;

Servo shoulderServo;
Servo elbowServo;
Servo rotationServo;
int xvalue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //zero
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  shoulderServo.writeMicroseconds(1500);
  elbowServo.attach(ELBOW_SERVO_PIN);
  elbowServo.writeMicroseconds(1500);
  rotationServo.attach(ROTATION_SERVO_PIN);
  rotationServo.writeMicroseconds(1500);

  
}

void loop() {
  //1000 = 360 degrees 1500 == 180 degrees 2000 = 0 degrees
  xvalue = analogRead(JOYSTICK_X_PIN);
  xvalue = map(xvalue, 0, 1023, 0, 180);
  if(timer<=1000){
    shoulderServo.write(xvalue);
    elbowServo.write(xvalue);
    rotationServo.write(xvalue);
  }
  else if (timer <= 2000){
    shoulderServo.write(xvalue);
    elbowServo.write(xvalue);
    rotationServo.write(xvalue);
  }
  else{
    timer = 0;
  }
  //elbowServo.writeMicroseconds(elbowAngle);
}
