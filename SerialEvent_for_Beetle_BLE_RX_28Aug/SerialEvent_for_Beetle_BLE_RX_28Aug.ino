char buf[5];

void setup() {
  Serial.begin(115200);
}

void loop() {
  char i=0;
  while(i<5){
  Serial.print(buf[i]);
  }
  delay(500);
}



//A serial Interrupt
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
