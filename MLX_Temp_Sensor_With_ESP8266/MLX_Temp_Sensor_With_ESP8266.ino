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
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
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
     Serial.print("T: ");
     Serial.println(String(tempAvg,1));
  }
  
  i++;
  if (i == 8)
    i %= 8;

  delay(500);
}
