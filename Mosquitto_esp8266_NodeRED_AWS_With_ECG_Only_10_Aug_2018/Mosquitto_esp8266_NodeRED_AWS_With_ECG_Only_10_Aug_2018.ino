/*****
 
 All the resources for this project:
 http://randomnerdtutorials.com/
 
*****/

/*THIS IS THE FIRST WORKING PROJECT CREATED BY XI MENG that real-time upload sensor data to the cloud*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <mlx90615.h>


MLX90615 mlx = MLX90615();
int i = 0;
float tempAvg = 0.0;
float calibratedTemp = 0.0;
float tempSum = 0.0;
float tempArray[8] = {};
float arrayDiff = 0.0;
float largestDiff = 0.0;


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

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
//WiFiClient XDMespClient;
//PubSubClient client(XDMespClient);



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

//test whether callback works when free!!
//void callback(String topic, byte* message, unsigned int length) {
//  Serial.print("Message arrived on topic: ");
//  Serial.print(topic);
//  Serial.print(". Message: ");
//  String messageTemp;
//  
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)message[i]);
//    messageTemp += (char)message[i];
//  }
//  Serial.println();
//
//  // Feel free to add more if statements to control more GPIOs with MQTT
//
//  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
//  if(topic=="room/lamp"){
//      Serial.print("Changing Room lamp to ");
//      if(messageTemp == "on"){
//        digitalWrite(lamp, HIGH);
//        Serial.print("On");
//      }
//      else if(messageTemp == "off"){
//        digitalWrite(lamp, LOW);
//        Serial.print("Off");
//      }
//  }
//  Serial.println();
//}



// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  client.setServer(mqttServer, mqttPort);
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
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
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  Serial.begin(115200);
  setup_wifi();
  initialise_MQTT();
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
  pinMode(D5, INPUT); // Setup for leads off detection LO +
  pinMode(D6, INPUT); // Setup for leads off detection LO -
  pinMode(A0, INPUT); // Setup for Analog Reading from pin A0
    
  //client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}


double ecg;

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  
  if(!client.loop())
    client.connect("ESP8266Client");


//
//   //ECG Sensor reading data from Analog 1 pin
//  if((digitalRead(5) == 1)||(digitalRead(6) == 1)){
//    Serial.println('!');
//  }
//  else{
//    ecg = analogRead(A0);
//    // send the value of analog input A0:
//      Serial.println(analogRead(ecg));
//  }

  ecg = analogRead(A0);
  //send the value of analog input A0:
  Serial.println(ecg);
  
  char ecgbuffer[7];
  dtostrf(ecg,6,0,ecgbuffer);

  // Publishes ECG value
  //PUBLISHING!!
  client.publish("wearable/ecg", ecgbuffer);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN,LOW);
  delay(50);

//  tempArray[i] = mlx.get_object_temp();
//  tempSum = 0.0;
//  tempAvg = 0.0;
//  calibratedTemp = 0.0;
//  largestDiff = 0.0;
//  
//       
//  for (int x = 0; x < 8; x++)
//  {
//    for (int y = x + 1; y < 8; y++)
//    {
//      arrayDiff = abs(tempArray[x] - tempArray[y]);
//      if (arrayDiff > largestDiff)
//      {
//        largestDiff = arrayDiff;
//      }
//    }
//  }
//  
//  if (largestDiff <= 0.2)
//  {
//    for (int k = 0; k < 8; k++)
//    {
//      tempSum += tempArray[k];
//    }
//
//     tempAvg = tempSum / 8;
//     Serial.print("T: ");
//     Serial.println(String(tempAvg,1));//tempAvg is the value(float), the 1 behind mean 1 decimal place
//  }
//
//    char tempbuffer[7];//A buffer that store the location of each character in the String
//    dtostrf(tempAvg, 6, 1, tempbuffer);//Convert Float to String, tempAvg store data, 6 is the width(how many bytes), 1 is precision(how many decimal place), tempbuffer is a pointer
//  
//    i++;
//    if (i == 8)
//    i %= 8;
//
//    digitalWrite(LED_BUILTIN,HIGH);
//    delay(50);
//    digitalWrite(LED_BUILTIN,LOW);
//    delay(50);
//
//    // Publishes Temperature value
//    //PUBLISHING!!
//    client.publish("wearable/temp", tempbuffer);
} 
