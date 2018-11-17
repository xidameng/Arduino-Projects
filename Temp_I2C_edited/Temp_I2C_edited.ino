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

#include <Wire.h>


void setup() {
  
  Serial.begin(115200);

  //12C comm setup
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onRequest(requestEvent); /* register request event */
}



void loop() {

//  if(Serial.available()>0)
//  {
//    float incoming_msg = Serial.read();
//    Serial.print("I received: ");
//    Serial.println(incoming_msg , 1);
//  }
//  delay(500);
}



// Handler that executes whenever data is requested from master
void requestEvent() {
 Wire.write("Hi ESP, request received");  /*send string on request */
}


