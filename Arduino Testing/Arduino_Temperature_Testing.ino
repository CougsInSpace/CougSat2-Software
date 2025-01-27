#include <Wire.h> 
#include <TLC59116.h>
#include <Serial.h>

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
  float temperatureC = analogRead(A4);
  float voltTemp = resist4/(resist4+resist1) * temp
  //voltage from 0-1024 needs to be converted into resistance
  Serial.println(temperatureC);
  
}
