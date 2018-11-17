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
const char* mqttServer = "13.229.141.7";  //This is the IP address of the AWS server
const int mqttPort = 1883;                  //port 1883 was specifically allocated to mosquitto for MQTT use
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


// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
void setup() {
  Serial.begin(115200);
  setup_wifi();
  initialise_MQTT();
  pinMode(LED_BUILTIN, OUTPUT);
  
}


double temp_ble;

void loop() {
    
  //Indicate the status of WIFI module
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  delay(100);
    
  //Checking for connection status
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");



  //Checking if there is any data in UART buffer
  if(Serial.available()>0){
    temp_ble = Serial.read();
    Serial.println(temp_ble);
  }
  else{
    Serial.println("waiting for temp_ble data...");
   }
  
  char temp_blebuffer[11];
  dtostrf(temp_ble,10,2,temp_blebuffer);

  // Publishes temp_ble value
  //PUBLISHING!!
  client.publish("wearable/temp", temp_blebuffer);

} 
