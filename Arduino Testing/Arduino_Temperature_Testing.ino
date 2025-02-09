
#include <Wire.h> 
#include <TLC59116.h>
#include <Serial.h>
#include <math.h>

byte read(int i){
  Wire.beginTransmission(0x68);
  Wire.write(i);
  Wire.endTransmission();
  Wire.requestFrom(0x68,1);
  byte value = Wire.read();


  return(value);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  float initialTemp = analogRead(A4);//0-1024
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  Wire.begin();
  
  

  Wire.beginTransmission(0x68);  
  Wire.write(0x14);
  Wire.write(0b10101010);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);  
  Wire.write(0x15);
  Wire.write(0b10101010);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);  
  Wire.write(0x16);
  Wire.write(0b10101010);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);  
  Wire.write(0x17);
  Wire.write(0b10101010);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x12);
  Wire.write(0);
  Wire.endTransmission();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  double temperatureV = analogRead(A4);
  int resist4 = 10000; //10kOhms
  int inputV = 5; //Arduino is 5v RP2040 is 3.3v;
  double totalResist = -1;
  if (temperatureV != 5){
      totalResist = (-resist4*temperatureV)/(temperatureV-5);
  }
 
  //||IMPORTANT||
  //STEINHART HART EQUATIONS CONSTANTS
  double steinA=0.001135590635;
  double steinB=0.0002341911173;
  double steinC=0.00000007879679525;
  totalResist = 10000;
  //Calc temp in C using formula
  //1/A+B*ln(R)+C*(ln(R))^3
  double tempC = 1 / (0.0011355906347799041 + 0.0002341911172955573 * 9.21034037198 + 7.879679525327714e-8 * 9.21034037198 * 9.21034037198 * 9.21034037198);
 

  //float voltTemp = resist4/(resist4+resist1) * temp
  //voltage from 0-1024 needs to be converted into resistance
  Serial.println(tempC);
  //Serial.println(totalResist);
  delay(100);
}
