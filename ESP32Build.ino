/*
 ___         _                ___               _             
|  _ \      ( )_             |  _ \            (_ )           
| (_) )  _  |  _)  _   _ __  | (_) )  __    _ _ | |  ___ ___  
|    / / _ \| |  / _ \(  __) |    / / __ \/ _  )| |/  _   _  \
| |\ \( (_) ) |_( (_) ) |    | |\ \(  ___/ (_| || || ( ) ( ) |
(_) (_)\___/ \__)\___/(_)    (_) (_)\____)\__ _)___)_) (_) (_)
                                                                                                                         
 * This file is part of Rotor Realm RemoteID
 *
 * Copyright (c) 2024 Rotor Realm
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/
#include "Arduino.h"
#include "gps.h"
#include "display.h"
#include "wifiBeacon.h"
#include "init.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define VGNSS_CTRL 3

String ssidString;
const char* ssid;
TaskHandle_t wifiBeaconTaskHandle = NULL;  // Declare the task handle globally

void setup() {
  ssidString = generateUASID();
  ssid = ssidString.c_str();

  pinMode(VGNSS_CTRL, OUTPUT);
  digitalWrite(VGNSS_CTRL, HIGH);
  Serial.begin(115200);
  
  // Order of Operations
  loadConfig();
  gpsSetup();
  displaySetup(ssid);
  wifiBeaconSetup(ssid);
  

  Serial.println("Creating GPS Task...");
  if (xTaskCreate(
        gpsTask,     // Function to be called
        "GPS Task",  // Name of the task
        8192,        // Stack size (increased size)
        NULL,        // Task input parameter
        1,           // Priority of the task
        NULL         // Task handle
        )
      != pdPASS) {
    Serial.println("Failed to create GPS Task");
  } else {
    Serial.println("GPS Task created successfully");
  }

  Serial.println("Creating WiFi Beacon Task...");
  if (xTaskCreatePinnedToCore(
        wifiBeaconTask,         // Function to be called
        "WiFi Beacon Task",     // Name of the task
        8192,                   // Stack size (increased size)
        (void*)ssid,            // Task input parameter
        2,                      // Priority of the task
        &wifiBeaconTaskHandle,  // Task handle
        0                       // Core number (0 or 1 for ESP32)
        )
      != pdPASS) {
    Serial.println("Failed to create WiFi Beacon Task");
  } else {
    Serial.println("WiFi Beacon Task created successfully");
    vTaskSuspend(wifiBeaconTaskHandle);  // Suspend the task immediately
  }

  Serial.println("Creating Display Task...");
  if (xTaskCreate(
        displayTask,     // Function to be called
        "Display Task",  // Name of the task
        8192,            // Stack size (increased size)
        NULL,            // Task input parameter
        3,               // Priority of the task
        NULL             // Task handle
        )
      != pdPASS) {
    Serial.println("Failed to create Display Task");
  } else {
    Serial.println("Display Task created successfully");
  }
}

void loop() {
  if (ggaData.numSatellites > 1) {
    vTaskResume(wifiBeaconTaskHandle);  // Resume the task if the condition is met
  } else {
    vTaskSuspend(wifiBeaconTaskHandle);  // Suspend the task if the condition is not met
  }

  vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay to prevent rapid suspend/resume calls
}

String generateUASID() {
  String prefix = "RRDIF";
  uint64_t chipId = ESP.getEfuseMac();
  String serialNumber = String(chipId, HEX);
  serialNumber.toUpperCase();
  while (serialNumber.length() < 7) {
    serialNumber = "0" + serialNumber;
  }
  String uasID = prefix + serialNumber;
  while (uasID.length() < 19) {
    uasID += "0";
  }
  return uasID;
}