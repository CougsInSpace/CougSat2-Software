
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
  double convertedV =5;
  double r =10000;
  // put your main code here, to run repeatedly:
  double analogV = analogRead(A4);
  int resist4 = 10000; //10kOhms
  int inputV = 5; //Arduino is 5v RP2040 is 3.3v;
  double totalResist = -1;
  if (analogV < 1024){
    convertedV = analogV/204.80;
    resist4 = resist4*convertedV*-.1;
    r  = resist4/(convertedV-inputV);


  }
 
  //||IMPORTANT||
  //STEINHART HART EQUATIONS CONSTANTS
  double a = (2.108508173*pow(10,-3));
  double b= (0.7979204727*pow(10,-4));
  double c = (6.535076315 * pow(10,-7));
  
  double tempK = 1/(a+(b*log(r)+(c*pow(log(r),3))));
  double tempC = tempK-273.15;

  //float voltTemp = resist4/(resist4+resist1) * temp
  //voltage from 0-1024 needs to be converted into resistance
  Serial.println(tempC);
  // Serial.println(convertedV);
  // Serial.println(r);
  //Serial.println(totalResist);
  delay(100);
}
