#include <Wire.h> 
#include <TLC59116.h>

int readLightSensor(int diodeNum){
 
  if(diodeNum==0){
    //Serial.println("Diode 0");
    return analogRead(A0);
  }
  if(diodeNum==1){
    //Serial.println("Diode 1");
    return analogRead(A1);
  }
  if(diodeNum==2){
    //Serial.println("Diode 2");
    return analogRead(A2);
  }
  if(diodeNum==3){
    //Serial.println("Diode 3");
    return analogRead(A3);
  }
}
void writeLedBrightness(int brightness, int j){
  int range = 6+((j-1)*4);
  int rangeEnd = range+4;
  for(range; range<rangeEnd;range++){
    //Serial.println(range);
    Wire.beginTransmission(0x68);   
    Wire.write(range);
    Wire.write(brightness);
    Wire.endTransmission();
  }
}
byte read(int i){
  Wire.beginTransmission(0x68);
  Wire.write(i);
  Wire.endTransmission();
  Wire.requestFrom(0x68,1);
  byte value = Wire.read();


  return(value);
}
//TLC59116 board1(0);

void setup() {
  Serial.begin(9600);
  float initialTemp = analogRead(A4);//0-1024
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  Wire.begin();

  
  //board1.begin();



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
  digitalWrite(13,HIGH);

  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.write(0b00000001);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);  
  Wire.write(0x01);
  Wire.write(0b00000000);
  Wire.endTransmission();
  int highestReading =0;
  int desiredDiode=1;
  for(int j=0;j<4;j++){
    int rawData = readLightSensor(j);
    rawData = constrain(rawData, 0,100);
    //Serial.println(rawData);
    byte processedData= map(rawData,0,100,0,255);//turn from 5v reading to human readable data
    //Serial.println(processedData);
    if (processedData>highestReading){
      highestReading=processedData;
    }
  }
  //Get desired diode
  int desiredRaw = readLightSensor(desiredDiode);
  desiredRaw = constrain(desiredRaw, 0,100);
  byte desiredProcessed= map(desiredRaw,0,100,0,255);

  if(desiredProcessed>= (highestReading-20)){
      writeLedBrightness(255,1);
      writeLedBrightness(255,2);
      writeLedBrightness(255,3);
      writeLedBrightness(255,4);
  }
  else{
    writeLedBrightness(0,1);
    writeLedBrightness(0,2);
    writeLedBrightness(0,3);
    writeLedBrightness(0,4);
  }
  Serial.println("Highest + Desired");
  Serial.println(highestReading);
  Serial.println(desiredProcessed);
  //Needs some form of debounce
  delay(500);
}
