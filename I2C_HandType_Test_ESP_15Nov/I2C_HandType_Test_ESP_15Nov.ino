#include <Wire.h>

void setup() {
 Serial.begin(115200); /* begin serial for debug */
 Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
 
}

void loop() {

//Simulating temp sensor
 Wire.requestFrom(9, 4); /* request & read data of size 13 from slave */
 while(Wire.available()){
    char c = Wire.read();
  Serial.print(c);
 }
 Serial.println();
 delay(1000);
  
 
//Simulating ECG sensor
  Wire.requestFrom(10, 3); /* request & read data of size 13 from slave */
 while(Wire.available()){
    char e = Wire.read();
  Serial.print(e);
 }
 Serial.println();
 delay(1000);

 
//Simulating HR and SpO2 sensor
  Wire.requestFrom(8, 4); /* request & read data of size 13 from slave */
 while(Wire.available()){
    char d = Wire.read();
  Serial.print(d);
 }
 Serial.println();
 delay(1000);
}
