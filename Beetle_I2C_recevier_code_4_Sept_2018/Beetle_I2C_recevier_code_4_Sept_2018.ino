/*
This is the code for BLUNO beetle recevier
And I2C is to be implemented between beetle and 
NodeMCU to establish Data transfer between 
MCUs  */

#include <Wire.h>
char buf[5] = {'\0'};

 
void setup() {
  Serial.begin(115200);
  Wire.begin(8);                /* join i2c bus with address 8 */

  Wire.onRequest(requestEvent); /* register request event */
}


void loop() {
  //delay(1);
}


void serialEvent()
{
  unsigned char i=0;
  while(Serial.available())
  {
    char inChar=(char)Serial.read();
    buf[i]=inChar;
    i++;
  }
}


// I2C: data requested from master
void requestEvent() {
   for( char t = 0; t <5; t++){
   Wire.write(buf[t]);  /*send string to master on request */
  }
}






// I2C: data received from master
//Not Needed in this project!
//void receiveEvent(int howMany) {
// while (0 <Wire.available()) {
//    Serial.println("I2C received this:");
//    Serial.print(Wire.read());      /* receive byte as a character */
//  }
// Serial.println();             /* to newline */
//}

