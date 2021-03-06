/*
This is the code for BLUNO beetle recevier
And I2C is to be implemented between beetle and 
NodeMCU to establish Data transfer between 
MCUs  */

#include <Wire.h>
char buf[4];

 
void setup() {
  Serial.begin(115200);
  Wire.begin(8);                /* join i2c bus with address 8 */

  Wire.onRequest(requestEvent); /* register request event */
}


void loop() {
  //central to peripheral data rate@2K/second, 4bytes would take 2ms 
  delay(15);//give it some time to load data to serial buffer
}


void serialEvent()
{
  unsigned char i=0;
  while(Serial.available()&&i<4)
  {
    char inChar=(char)Serial.read();
    buf[i]=inChar;
    i++;
  }
  Serial.print("Beetle MCU receive:");
  Serial.println(buf);
}


// I2C: data requested from master
void requestEvent() {
   for( char t = 0; t <4; t++){
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

