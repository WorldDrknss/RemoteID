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
//display.cpp
#include "HT_st7735.h"
#include "init.h"
#include "gps.h"     // Include the header file where ggaData is declared
#include <string.h>  // Include string.h for strlen and strcmp functions
#include <time.h>

extern TaskHandle_t wifiBeaconTaskHandle;  // Declare the task handle as external

HT_st7735 tft;
extern Config config;

// Variables to store previous values
int prevNumSatellites = -1;
int prevNumSatellitesLength = strlen("PNDNG");
double prevLatitude = -1;
int prevLatitudeLength = strlen("PNDNG");
double prevLongitude = -1;
int prevLongitudeLength = strlen("PNDNG");
char prevTime[20] = "PNDNG";
int prevTimeLength = strlen(prevTime);

// Variables to store previous WiFi status
char prevWifiStatus[10] = "PNDNG";
int prevWifiStatusLength = strlen(prevWifiStatus);

#define ST7735_PURPLE 0x780F  // This is an example for purple

int timezoneStringToOffset(const String& timezone) {
  if (timezone == "PST") return -8;
  if (timezone == "PDT") return -7;
  if (timezone == "EST") return -5;
  if (timezone == "EDT") return -4;
  if (timezone == "CST") return -6;
  if (timezone == "CDT") return -5;
  // Add other timezones as needed
  return 0;  // Default to UTC if the timezone is not recognized
}

void displaySetup(const char* ssid) {
  int col1 = 0;
  int col2 = 30;

  // Initialize Screen and Fill Black
  tft.st7735_init();
  tft.st7735_fill_screen(ST7735_BLACK);

  // Beacon and FC Status
  tft.st7735_write_str(col1, 0, "BTX:", Font_7x10, ST7735_PURPLE, ST7735_BLACK);
  tft.st7735_write_str(col2, 0, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);
  tft.st7735_write_str(85, 0, "FC:", Font_7x10, ST7735_PURPLE, ST7735_BLACK);
  tft.st7735_write_str(110, 0, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);

  // Satellites
  tft.st7735_write_str(col1, 13, "SAT:", Font_7x10, ST7735_PURPLE, ST7735_BLACK);
  tft.st7735_write_str(col2, 13, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);
  tft.st7735_write_str(85, 13, "TM:", Font_7x10, ST7735_PURPLE, ST7735_BLACK);
  tft.st7735_write_str(110, 13, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);

  // Lat
  tft.st7735_write_str(col1, 26, "LAT:", Font_7x10, ST7735_PURPLE, ST7735_BLACK);
  tft.st7735_write_str(col2, 26, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);

  // Lon
  tft.st7735_write_str(col1, 39, "LNG:", Font_7x10, ST7735_PURPLE, ST7735_BLACK);
  tft.st7735_write_str(col2, 39, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);

  // Aircraft Serial
  tft.st7735_write_str(col1, 52, "MFR:", Font_7x10, ST7735_PURPLE, ST7735_BLACK);
  tft.st7735_write_str(col2, 52, "RRDI", Font_7x10, ST7735_RED, ST7735_BLACK);  // Show RRDI

  tft.st7735_write_str(col1, 65, "S/N:", Font_7x10, ST7735_PURPLE, ST7735_BLACK);
  tft.st7735_write_str(col2, 65, ssid + 4, Font_7x10, ST7735_RED, ST7735_BLACK);  // Show the remaining of SSID
}

String formatTime(int value) {
  if (value < 10) return "0" + String(value);
  return String(value);
}

void displayTask(void* parameters) {
  Serial.println("Display Task started");

  for (;;) {
    // Update the display with ggaData only if values have changed
    char buffer[20];

    // Update number of satellites if it has changed
    if (ggaData.numSatellites > 0 && ggaData.numSatellites != prevNumSatellites) {
      snprintf(buffer, sizeof(buffer), "%d", ggaData.numSatellites);
      int newLength = strlen(buffer);
      tft.st7735_write_str(30, 13, buffer, Font_7x10, ST7735_RED, ST7735_BLACK);
      // Clear any extra characters
      for (int i = newLength; i < prevNumSatellitesLength; i++) {
        tft.st7735_write_str(30 + i * 7, 13, " ", Font_7x10, ST7735_BLACK, ST7735_BLACK);
      }
      prevNumSatellites = ggaData.numSatellites;
      prevNumSatellitesLength = newLength;
    } else if (ggaData.numSatellites == 0 && prevNumSatellites != 0) {
      tft.st7735_write_str(30, 13, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);
      prevNumSatellites = 0;
      prevNumSatellitesLength = strlen("PNDNG");
    }

    // Update latitude if it has changed
    if (ggaData.latitude != prevLatitude) {
      snprintf(buffer, sizeof(buffer), "%.5f", ggaData.latitude);
      int newLength = strlen(buffer);
      tft.st7735_write_str(30, 26, buffer, Font_7x10, ST7735_RED, ST7735_BLACK);
      // Clear any extra characters
      for (int i = newLength; i < prevLatitudeLength; i++) {
        tft.st7735_write_str(30 + i * 7, 26, " ", Font_7x10, ST7735_BLACK, ST7735_BLACK);
      }
      prevLatitude = ggaData.latitude;
      prevLatitudeLength = newLength;
    } else if (ggaData.latitude == 0.0 && prevLatitude != 0.0) {
      tft.st7735_write_str(30, 26, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);
      prevLatitude = 0.0;
      prevLatitudeLength = strlen("PNDNG");
    }

    // Update longitude if it has changed
    if (ggaData.longitude != prevLongitude) {
      snprintf(buffer, sizeof(buffer), "%.5f", ggaData.longitude);
      int newLength = strlen(buffer);
      tft.st7735_write_str(30, 39, buffer, Font_7x10, ST7735_RED, ST7735_BLACK);
      // Clear any extra characters
      for (int i = newLength; i < prevLongitudeLength; i++) {
        tft.st7735_write_str(30 + i * 7, 39, " ", Font_7x10, ST7735_BLACK, ST7735_BLACK);
      }
      prevLongitude = ggaData.longitude;
      prevLongitudeLength = newLength;
    } else if (ggaData.longitude == 0.0 && prevLongitude != 0.0) {
      tft.st7735_write_str(30, 39, "PNDNG", Font_7x10, ST7735_RED, ST7735_BLACK);
      prevLongitude = 0.0;
      prevLongitudeLength = strlen("PNDNG");
    }

    // Get the timezone offset from config
    int timezoneOffset = timezoneStringToOffset(config.operatorInfo.OperatorTimezone);

    // Convert GPS time to int
    int hours = ggaData.time.substring(0, 2).toInt();
    int minutes = ggaData.time.substring(2, 4).toInt();

    // Apply the timezone offset
    hours = (hours + timezoneOffset + 24) % 24; // Ensure the hours wrap around correctly

    // Format the adjusted time back to a string
    String formattedTime = formatTime(hours) + ":" + formatTime(minutes);

    // Update GPS time if it has changed
    if (strcmp(formattedTime.c_str(), prevTime) != 0) {
      snprintf(buffer, sizeof(buffer), "%s", formattedTime.c_str());
      int newLength = strlen(buffer);
      tft.st7735_write_str(110, 13, buffer, Font_7x10, ST7735_RED, ST7735_BLACK);
      // Clear any extra characters
      for (int i = newLength; i < prevTimeLength; i++) {
        tft.st7735_write_str(110 + i * 7, 13, " ", Font_7x10, ST7735_BLACK, ST7735_BLACK);
      }
      strcpy(prevTime, formattedTime.c_str());
      prevTimeLength = newLength;
    }

    // Update WiFi Beacon Task status if it has changed
    const char* wifiStatus = (eTaskGetState(wifiBeaconTaskHandle) != eSuspended) ? "TRSMTNG" : "PNDNG";
    if (strcmp(wifiStatus, prevWifiStatus) != 0) {
      int newLength = strlen(wifiStatus);
      tft.st7735_write_str(30, 0, wifiStatus, Font_7x10, ST7735_RED, ST7735_BLACK);
      // Clear any extra characters
      for (int i = newLength; i < prevWifiStatusLength; i++) {
        tft.st7735_write_str(30 + i * 7, 0, " ", Font_7x10, ST7735_BLACK, ST7735_BLACK);
      }
      strcpy(prevWifiStatus, wifiStatus);
      prevWifiStatusLength = newLength;
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay to yield control
  }
}
