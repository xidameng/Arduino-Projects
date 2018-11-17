#include <Wire.h>

char string[]="7899";
int i = 0;

void setup() {

  Serial.begin(115200);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {

  while(Serial.available()>0)
  {
    Serial.print("Available bytes at serial buffer are:");
    Serial.println(Serial.available());
    string[i] = (char)Serial.read();
    i=i+1;
    Serial.println(i);
    delay(10);
    if(i == 4)
    {
      i = 0;
      Serial.println(string);
      break;
    }
  }
}



// I2C: data requested from master
void requestEvent() {
   for( char t = 0; t <4; t++){
   Wire.write(string[t]);  /*send string to master on request */
  }
  Serial.print("I2C received:");
  Serial.println(string);
}

//end


