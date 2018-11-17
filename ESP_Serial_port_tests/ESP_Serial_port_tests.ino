int incomingByte = 0;   // for incoming serial data

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps 
        pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

        // send data only when you receive data:
        if (Serial.available() > 0) {
          incomingByte = Serial.read();
            if (incomingByte == 1){
          
                digitalWrite(LED_BUILTIN, HIGH);
                delay(200);
                digitalWrite(LED_BUILTIN, LOW);
                delay(100);
            }
             else{
                digitalWrite(LED_BUILTIN, HIGH);
            }
        }
        else{
          digitalWrite(LED_BUILTIN, HIGH);       
          }
}
