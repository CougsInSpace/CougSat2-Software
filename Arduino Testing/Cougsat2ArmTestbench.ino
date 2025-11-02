#include <Servo.h>
#include <elapsedMillis.h>

#define SHOULDER_SERVO_PIN 10
#define ELBOW_SERVO_PIN 11

elapsedMillis timer;

Servo shoulderServo;
Servo elbowServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //zero
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  shoulderServo.writeMicroseconds(1500);
  elbowServo.attach(ELBOW_SERVO_PIN);
  elbowServo.writeMicroseconds(1500);

  //1000 = 360 degrees 1500 == 180 degrees 2000 = 0 degrees
  float shoulderAngle = 1500; //180 degrees
  float elbowAngle = 1500;
}

void loop() {
  // put your main code here, to run repeatedly:
  shoulderServo.writeMicroseconds(shoulderAngle);
  elbowServo.writeMicroseconds(elbowAngle);
}
