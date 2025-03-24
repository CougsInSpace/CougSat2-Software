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

  
  Serial.println();
   Serial.println("-X=");
  Serial.println(readLightSensor(0));
  Serial.println("Z=");
  Serial.println(readLightSensor(1));
  Serial.println("Y=");
  Serial.println(readLightSensor(2));
  Serial.println("X=");
  Serial.println(readLightSensor(3));
 
  
for(int j=0;j<4;j++){
    int rawData = readLightSensor(j);
    bool highestFlag = false;
    if (rawData>(highestReading+5)){//If the new reading is notably highest
      highestReading=rawData;
      highestFlag = true;
    }
    switch (j){
      case(0):
        if(highestFlag){
           Serial.println("\n-X is highest\n");
           Serial.println("\n Roll right\n");
            writeLedBrightness(0,0);
          writeLedBrightness(0,1);
          writeLedBrightness(0,2);
          writeLedBrightness(0,3);
        }
       
      break;
      case(1):
        if(highestFlag){
          Serial.println("\nZ is highest\n"); 
          Serial.println("\n At desired diode\n");
          //Celebrate
          writeLedBrightness(255,0);
          writeLedBrightness(255,1);
          writeLedBrightness(255,2);
          writeLedBrightness(255,3);
        }
        
      break;
      case(2):
        if(highestFlag){
          Serial.println("\nY is highest\n");
          Serial.println("\n Pitch down\n");
          writeLedBrightness(0,0);
          writeLedBrightness(0,1);
          writeLedBrightness(0,2);
          writeLedBrightness(0,3); 
        }
        
      break;
      case(3):
        if(highestFlag){
          Serial.println("\nX is highest\n");
          Serial.println("\n Roll Left\n");
          writeLedBrightness(0,0);
          writeLedBrightness(0,1);
          writeLedBrightness(0,2);
          writeLedBrightness(0,3);
        }
        
      break;
      case(4):
        if(highestFlag){
          Serial.println("\n-Y is highest\n");
          Serial.println("\n Pitch Up\n");
          writeLedBrightness(0,0);
          writeLedBrightness(0,1);
          writeLedBrightness(0,2);
          writeLedBrightness(0,3);
        }
        
      break;
      case(5):
        if(highestFlag){
          Serial.println("\n-Z is highest\n");
          Serial.println("\n Roll Right/Left OR Pitch Up/Down\n");
          writeLedBrightness(0,0);
          writeLedBrightness(0,1);
          writeLedBrightness(0,2);
          writeLedBrightness(0,3);
        } 
        
      break;
    }
    
  }
  //Get desired diode
  int desiredRaw = readLightSensor(desiredDiode);
  


  //Needs some form of debounce
  delay(500);
}
