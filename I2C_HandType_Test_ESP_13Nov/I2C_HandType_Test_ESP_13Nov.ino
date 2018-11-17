#include <Wire.h>

void setup() {
 Serial.begin(115200); /* begin serial for debug */
 Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
 
// Wire.beginTransmission(9); /* begin with device address 8 */
// Wire.write("Hello Arduino");  /* sends hello string */
// Wire.endTransmission();    /* stop transmitting */
}

void loop() {

 Wire.requestFrom(9, 4); /* request & read data of size 13 from slave */
 while(Wire.available()){
    char c = Wire.read();
  Serial.print(c);
 }
 //Serial.println();
 delay(1000);
}
