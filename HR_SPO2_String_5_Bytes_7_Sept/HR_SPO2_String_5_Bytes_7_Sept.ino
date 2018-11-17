/*
  Optical SP02 Detection (SPK Algorithm) using the MAX30105 Breakout
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 19th, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  This demo shows heart rate and SPO2 levels.

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/

// HR1 TX: 0x50658399739E
// HR1 RX: 0x38D269FDC0FE
// HR2 TX: 0x38D269FDB614
// HR2 RX: 0x38D269FDC9E5

#include <Wire.h>
#include "MAX30105.h"
#include "final_hr_spo2_algorithm.h"

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

byte pulseLED = 11; //Must be on PWM pin
byte readLED = 13; //Blinks with each data read

char hrbuf[3];
char Null[1];
char spo2buf[3];


void setup()
{
  Serial.begin(115200); // initialize serial communication at 115200 bits per second:

  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30102 was not found. Please check wiring/power."));
    while (1);
  }

  //Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));
  //while (Serial.available() == 0) ; //wait until user presses a key
  Serial.read();

  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void loop()
{
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    Serial.print(F("red="));
    Serial.print(redBuffer[i], DEC);
    irBuffer[i] = particleSensor.getIR();
    Serial.print(F(", ir="));
    Serial.println(irBuffer[i], DEC);
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
  }

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  max_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every data read

      redBuffer[i] = particleSensor.getRed();
      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      irBuffer[i] = particleSensor.getIR();
      Serial.print(F(", ir="));
      Serial.println(irBuffer[i], DEC);
      particleSensor.nextSample(); //We're finished with this sample so move to next sample
    }
      //After gathering 25 new samples recalculate HR and SP02
      max_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
      
      //send samples and calculation result to terminal program through UART



//      hrbuf = String(heartRate);
//      spo2buf = String(spo2);
//      Serial.write(hrbuf);//should be 2 bytes of data
//      Serial.write(Null); //1 byte of null
//      Serial.write(spo2buf);//another 2 bytes here
    }
}

