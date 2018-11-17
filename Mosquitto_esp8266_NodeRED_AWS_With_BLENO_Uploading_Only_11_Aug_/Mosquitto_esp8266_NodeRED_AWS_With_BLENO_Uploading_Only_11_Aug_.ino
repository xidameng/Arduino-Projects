/*****
 
 All the resources for this project:
 http://randomnerdtutorials.com/
 
 THIS IS THE FIRST WORKING PROJECT CREATED BY XI MENG
 for real-time uploading sensor data to the cloud
 
*****/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "SimonMiix320";
const char* password = "82517802";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqttServer = "13.229.143.144";  //127.0.0.1 is the localhost
const int mqttPort = 1883;
//const char* mqttServer = "m10.cloudmqtt.com";
//const int mqttPort = 18647;
//const char* mqttUser = "xjerpjsc";          //use to access the mqtt account
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


// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
void setup() {
  Serial.begin(115200);
  setup_wifi();
  initialise_MQTT();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT); // Setup for Analog Reading from pin A0 
}


double ecg;

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  
  if(!client.loop())
    client.connect("ESP8266Client");

  if(Serial.available()>0){
    ecg = Serial.read();
    Serial.println(ecg);
  }
  else{
    Serial.println("waiting for ecg data...");
    //blink once and wait for 0.1 second to check for serial buffer
    digitalWrite(LED_BUILTIN,HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);
    delay(100);
  }
  
  
  char ecgbuffer[7];
  dtostrf(ecg,6,0,ecgbuffer);

  // Publishes ECG value
  //PUBLISHING!!
  client.publish("wearable/ecg", ecgbuffer);



  Serial.print("T: ");
  Serial.println(String(tempAvg,1));//tempAvg is the value(float), the 1 behind mean 1 decimal place

  char tempbuffer[7];//A buffer that store the location of each character in the String
  dtostrf(tempAvg, 6, 1, tempbuffer);//Convert Float to String, tempAvg store data, 6 is the width(how many bytes), 1 is precision(how many decimal place), tempbuffer is a pointer
  
  // Publishes Temperature value
  client.publish("wearable/temp", tempbuffer);


} 
