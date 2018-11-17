void setup() 
{
        Serial.begin(115200);     // opens serial port, sets data rate to 9600 bps
}

void loop() 
{
            if(Serial.available())
            {
                Serial.println("Transmission started");
                
                while(Serial.available())
                {
                  Serial.println(Serial.read());
                }
                
                Serial.println("Transmission ended");
            }
}
