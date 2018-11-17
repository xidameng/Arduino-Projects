/*****
 
 All the resources for this project:
 http://randomnerdtutorials.com/
 
 THIS IS THE FIRST WORKING PROJECT CREATED BY XI MENG
 for real-time uploading sensor data to the cloud
 
*****/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


//Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "SimonMiix320";
const char* password = "82517802";

//Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqttServer = "13.250.98.113";   //This is the IP address of the AWS server_Jessica's one
const int mqttPort = 1883;                  //port 1883 was specifically allocated to mosquitto for MQTT use

//Using CloudMQTT
//const char* mqttServer = "m10.cloudmqtt.com";
//const int mqttPort = 18647;
//const char* mqttUser = "xjerpjsc";          // For cloutMQTT(disabled)
//const char* mqttPassword = "Fg5cFpOtwf2F";  //^^^^^^^^^^^^^^^^^^^^^^^^^^^

WiFiClient espClient;
PubSubClient client(espClient);



// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


//In "Callback" function, ESP listening for command via WIFI
// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something

void initialise_MQTT() {

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      //strip.setPixelColor(0, 0, 255, 0); // Green, means connected to MQTT server
      //strip.show();
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      //strip.setPixelColor(0, 255, 255, 0); // Yellow means MQTT connection issue
      //strip.show();
      delay(2000);
    }
  }
}



// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  client.setServer(mqttServer, mqttPort);
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("room/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



char ecg_ble[3];

//I2C address 10, 3 bytes requested
void ECG()
{
  Serial.println("Retriving ECG data now...");
  
  //typical data sample will be like 359, 378, 401
  Wire.requestFrom(10, 3); /* request data of size 4 from beetle at address 10 */

  //delay(1); // to fix the missing bytes issue
  
  //Checking if there is any data in UART buffer
  if(Wire.available()){
    
    int ecgcount = 0;
    while(Wire.available() && ecgcount <3){ //check for 5 bytes of data
     
      ecg_ble[ecgcount] = Wire.read();
      Serial.print(ecg_ble[ecgcount]);// changed from println to print
      ecgcount++;
    }
    Serial.println();
  }
  
  else{
    Serial.println("waiting for ECG data...");
      
    //Indicate that No Data Received
    digitalWrite(LED_BUILTIN,HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);//LOW actually leave the LED On.
    delay(100);
   }

  // Publishes temp_ble value
  //PUBLISHING!!
  client.publish("wearable/ecg", ecg_ble);
}



char temp_ble[4];

//I2C address 9 , 4 bytes requested
void TEMP(){
  
  Serial.println("Retriving temp data now...");
  
///////////////////////
/*  For temp ble RX  */
///////////////////////

  //Request Data from TEMP beetle bluno Receiver
  Wire.requestFrom(9, 4); /* request data of size 4 from beetle at address 8 */
  //used to be (8,7)
  //wait for 1ms for I2C buffer to load data
  //delay(50);// 10ms for each byte//not needed
  
  //Checking if there is any data in UART buffer
  if(Wire.available()){
    
    int tcount = 0;
    while(Wire.available() && tcount <4){
     
      temp_ble[tcount] = Wire.read();
      Serial.print(temp_ble[tcount]);// changed from println to print
      tcount++;
      //delay(1);//new delay
    }
    Serial.println();
  }
  
  else{
    Serial.println("waiting for temp data...");
      
    //Indicate that No Data Received
    digitalWrite(LED_BUILTIN,HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);//LOW actually leave the LED On.
    delay(100);
   }

//  These code is only needed if float values are to be received
//  char temp_blebuffer[11];
//  dtostrf(temp_ble,10,2,temp_blebuffer);

  // Publishes temp_ble value
  //PUBLISHING!!
  client.publish("wearable/temp", temp_ble);

  //Wait for 0.5 second after upload, too fast uploading would result
  //in NodeRed dashboard breakdown.
  
}



char hr_ble[4];

//I2C address 8, 4 bytes requested
void HRSPO2(){

  Serial.println("Retriving hrspo2 data now...");
  
//////////////////////////
/*  For HR&SpO2 ble RX  */
//////////////////////////

  Wire.requestFrom(8, 4); /* request data of size 10 from beetle at address 9, bcos the data is of form"-999[ENTER]-999[ENTER]" which are 10 bytes in total */
  //5 bytes being 2 bytes for HR and 1 byte for /n and another 2 bytes for spo2

  //delay(1); // to fix the missing bytes issue
  
  //Checking if there is any data in UART buffer
  if(Wire.available()){
    
    int hcount = 0;
    while(Wire.available() && hcount <4){ //check for 5 bytes of data
     
      hr_ble[hcount] = Wire.read();
      Serial.print(hr_ble[hcount]);// changed from println to print
      hcount++;
    }
    Serial.println();
  }
  
  else{
    Serial.println("waiting for HR and spo2 data...");
      
    //Indicate that No Data Received
    digitalWrite(LED_BUILTIN,HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);//LOW actually leave the LED On.
    delay(100);
   }

  // Publishes temp_ble value
  //PUBLISHING!!
  client.publish("wearable/hrspo2", hr_ble);
}




// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
void setup() {
  Serial.begin(115200);
  Wire.begin(D1, D2); /* join i2c bus as MASTER, with SDA=D1 and SCL=D2*/
  setup_wifi();
  initialise_MQTT();
  pinMode(LED_BUILTIN, OUTPUT);
}



void loop() {
   
  //Checking for connection status
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop()){
    client.connect("ESP8266Client");
  }

//////////////////////////////////////////////////////////////////////////
/*  Every second take 5 ECG samples, 1 temp sample and 1 hrspo2 sample  */
//////////////////////////////////////////////////////////////////////////
  
  ECG();
  delay(1000); //
  
  TEMP();
  delay(1000); //
  
  HRSPO2();
  delay(1000); //1000ms
}
//END
