void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);

  
}

int i=1;

void loop() {
  // put your main code here, to run repeatedly:

  switch(i){
    case 1:
      digitalWrite(D1,HIGH);
      delay(500);
      digitalWrite(D1,LOW);
      delay(500);
      i++;
      break;
    case 2:
      digitalWrite(D2,HIGH);
      delay(500);
      digitalWrite(D2,LOW);
      delay(500);
      i++;
      break;
    case 3:
      digitalWrite(D3,HIGH);
      delay(500);
      digitalWrite(D3,LOW);
      delay(500);
      i++;
      break;
    case 4:
      digitalWrite(D4,HIGH);
      delay(500);
      digitalWrite(D4,LOW);
      delay(500);
      i=1;
      break;
  }
}
