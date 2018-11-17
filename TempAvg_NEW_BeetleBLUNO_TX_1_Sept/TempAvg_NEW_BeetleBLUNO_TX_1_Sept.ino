/*************************************************************************
 * A minimal Arduino library for
 * Melexis MLX90615 Infra-red Temperature Sensor
 * Copyright (C) 2017 Sergey Kiselev
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *************************************************************************/

// T1 TX: 0x38D269FDB29F
// T1 RX: 0x506583874199//old one
// T2 TX: 0x506583874376
// T2 RX: 0x506583874343
 
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



void setup() {
  Serial.begin(115200);
  //Serial.println("Melexis MLX90615 infra-red temperature sensor test");
  mlx.begin();

//  Serial.print("Sensor ID number = ");
//  Serial.println(mlx.get_id(), HEX);
}


char tempbuffer[5];

void loop() {
  //Serial.print("Ambient = ");
  //Serial.print(String(mlx.get_ambient_temp(),1)); 
  //Serial.print("T: ");
  //Serial.print(String(mlx.get_object_temp(),2));
  //Serial.println(" *C");

  tempArray[i] = mlx.get_object_temp();
  //Serial.println(tempArray[0]);
  //Serial.println(tempArray[1]);
  //Serial.println(tempArray[2]);
  //Serial.println(tempArray[3]);
  //Serial.println(tempArray[4]);
  //Serial.println(tempArray[5]);
  //Serial.println(tempArray[6]);
  //Serial.println(tempArray[7]);
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
        //Serial.println(String(largestDiff,2));
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
     //Serial.print("T: ");
     dtostrf(tempAvg,5,1,tempbuffer);
     tempbuffer[4] = '\0';
     Serial.write(tempbuffer);


//     calibratedTemp = tempAvg + 0.5;
//     Serial.print("Calibrated Temperature = ");
//     Serial.println(String(calibratedTemp,1));
  }
  
  i++;
  if (i == 8)
    i %= 8;

  //Serial.println();
  delay(500);
}
