#include <Wire.h>

char string[6]="100100";
//int i = 0;

void setup() {

  Serial.begin(115200);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {


//  while(Serial.available()&&i<4)
//  {
//    Serial.println("Data available at Serial port!");
//    i = 0;
//    string[i] = (char)Serial.read();
//    i++;
//    Serial.println(i);
//    
//    if(i == 4)
//    {
//      Serial.print("Received a string from Serial:");
//      Serial.println(string);
//    }
//  }
}



// I2C: data requested from master
void requestEvent() {
   for( char t = 0; t <6; t++){
   Wire.write(string[t]);  /*send string to master on request */
  }
  Serial.println(string);
}

//end
