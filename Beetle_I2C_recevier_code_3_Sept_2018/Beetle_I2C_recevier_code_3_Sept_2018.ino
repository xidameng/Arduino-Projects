/*
This is the code for BLUNO beetle recevier (Peripheral)
And I2C is to be implemented between beetle and 
NodeMCU to establish Data transfer between 
MCUs  */

#include <Wire.h>
char buf[5];
bool stringComplete = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(8);       /* join i2c bus as Slave with address 8 */
  Wire.onRequest(requestEvent); /* register request event */
   Wire.onReceive(receiveEvent); /* register receive event */
}

void loop() {
//Loop infinitely without delay to ensure serial buffer is read instanteously

  unsigned char c=0;
  while(Serial.available() && c<5)
  {
    char inChar=(char)Serial.read();
    buf[c]=inChar;
    c++;
    if(buf[c] == '\0'){
      stringComplete = true;
      Serial.println("String is Complete");
    }
    else{Serial.println("null 0 is not received");}
  }

//delay(50); //delay 50ms to slow down serial event from changing the status of stringComplete
}

//
//void serialEvent(){
//  unsigned char c=0;
//  while(Serial.available() && c<5)
//  {
//    char inChar=(char)Serial.read();
//    buf[c]=inChar;
//    c++;
//    if(buf[c] == '\0'){
//      stringComplete = true;
//      Serial.println("String is Complete");
//    }
//    else{Serial.println("null 0 is not received");}
//  }
//}

// I2C: data requested from master
void requestEvent() 
{
//  while(stringComplete == true)
//  {
    Serial.println("NodeMCU is requesting data");
    char i = 0;
    while(i <5)
    {
      Wire.write(buf[i]);  /*send string to master on request */
      Serial.println("1 byte of data are via I2C transmitted");
      i++;
    }
    //stringComplete = false;
  //}
}

void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.print(c);           /* print the character */
  }
 Serial.println();             /* to newline */
}
