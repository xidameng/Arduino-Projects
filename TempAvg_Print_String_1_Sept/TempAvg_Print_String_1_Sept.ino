/*************************************************************************
 * A minimal Arduino library for
 * Melexis MLX90615 Infra-red Temperature Sensor
 * Copyright (C) 2017 Sergey Kiselev
 *************************************************************************/

// T1 TX: 0x38D269FDB29F
// T1 RX: 0x506583874199
// T2 TX: 0x506583874376
// T2 RX: 0x506583874343
 
#include <Wire.h>
#include <mlx90615.h>

MLX90615 mlx = MLX90615();
int i = 0;
double tempAvg = 0.0;
float calibratedTemp = 0.0;
float tempSum = 0.0;
float tempArray[8] = {};
float arrayDiff = 0.0;
float largestDiff = 0.0;

char tempbuf[5];


void setup() {
  Serial.begin(115200);
  //Serial.println("Melexis MLX90615 infra-red temperature sensor test");
  mlx.begin();
}

void loop() {

  tempArray[i] = mlx.get_object_temp();

  tempSum = 0.0;
  tempAvg = 0.0;
  calibratedTemp = 0.0;
  largestDiff = 0.0;
  
  for (int x = 0; x < 8; x++)
  {
    for (int y = x + 1; y < 8; y++)
    {
      arrayDiff = abs(tempArray[x] - tempArray[y]);
      if (arrayDiff > largestDiff)
      {
        largestDiff = arrayDiff;
      }
    }
  }
  
  if (largestDiff <= 0.2)
  {
    for (int k = 0; k < 8; k++)
    {
      tempSum += tempArray[k];
    }

     tempAvg = tempSum / 8;
//   Serial.print("T: ");

     //xx.x takes 5 bytes space
     dtostrf(tempAvg,5,1,tempbuf);
     Serial.write(tempbuf);//Writing binary values rather than ASCII human readable values(Serial.print)

//   Uncomment the line below will bring float value instead of string
//     Serial.println(tempAvg);
  }
  
  i++;
  if (i == 8)
    i %= 8;

  delay(250);
}
