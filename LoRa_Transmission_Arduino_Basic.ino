#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //TX, RX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    
  if(Serial.available() > 0){//Read from serial monitor and send over LoRa wireless module
    String input = Serial.readString();
    mySerial.println(input);    
  }
  
  delay(20);
}
