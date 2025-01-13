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
#include "gps.h"
#include "nema.h"

HardwareSerial GPSSerial(1);

GGAData ggaData;
RMCData rmcData;
GSAData gsaData;
GSVData gsvData;
NAVACCData navaccData;

#define VGNSS_CTRL 3

void gpsSetup() {
  pinMode(VGNSS_CTRL, OUTPUT);
  digitalWrite(VGNSS_CTRL, HIGH);
  GPSSerial.begin(115200, SERIAL_8N1, 33, 34);  // RX, TX
  Serial.println("GPS setup complete");

  // Enable NAVACC and VTG
  GPSSerial.print("$CFGMSG,0,5,1\r\n"); // Enable VTG
  delay(1000);  // Wait for the GPS to respond
  GPSSerial.print("$CFGMSG,1,3,1\r\n");  // Enable NAVACC
  delay(1000);  // Wait for the GPS to respond
}

void gpsTask(void* parameters) {
  Serial.println("GPS Task started");
  for (;;) {
    while (GPSSerial.available()) {
      String nmea = GPSSerial.readStringUntil('\n');
      parseNMEA(nmea, ggaData, rmcData, gsaData, gsvData, navaccData);

      // Print all NMEA messages for debugging
      // Serial.println(nmea);

      if (nmea.startsWith("$GPGGA") || nmea.startsWith("$GBGGA") || nmea.startsWith("$GAGGA") || nmea.startsWith("$GLGGA") || nmea.startsWith("$GNGGA")) {
        // Serial.print("Time: ");
        // Serial.println(ggaData.time);
        // Serial.print("GGA Latitude: ");
        // Serial.println(ggaData.latitude);
        // Serial.print("GGA Longitude: ");
        // Serial.println(ggaData.longitude);
        // Serial.println(ggaData.numSatellites);
      } else if (nmea.startsWith("$GPRMC") || nmea.startsWith("$GBRMC") || nmea.startsWith("$GARMC") || nmea.startsWith("$GLRMC") || nmea.startsWith("$GNRMC")) {
        // Serial.print("Time: ");
        // Serial.println(rmcData.time);
        // Serial.print("Status: ");
        // Serial.println(rmcData.status);
        // Serial.print("RMC Latitude: ");
        // Serial.println(rmcData.latitude);
        // Serial.print("RMC Longitude: ");
        // Serial.println(rmcData.longitude);
      } else if (nmea.startsWith("$GPGSA") || nmea.startsWith("$GBGSA") || nmea.startsWith("$GAGSA") || nmea.startsWith("$GLGSA") || nmea.startsWith("$GNGSA")) {
        // Serial.print("Mode1: ");
        // Serial.println(gsaData.mode1);
        // Serial.print("Mode2: ");
        // Serial.println(gsaData.mode2);
        // Serial.print("PDOP: ");
        // Serial.println(gsaData.pdop);
        // Serial.print("HDOP: ");
        // Serial.println(gsaData.hdop);
        // Serial.print("VDOP: ");
        // Serial.println(gsaData.vdop);
      } else if (nmea.startsWith("$GPGSV") || nmea.startsWith("$GBGSV") || nmea.startsWith("$GAGSV") || nmea.startsWith("$GLGSV") || nmea.startsWith("$GNGSV")) {
        // Serial.print("Number of Messages: ");
        // Serial.println(gsvData.numMessages);
        // Serial.print("Message Number: ");
        // Serial.println(gsvData.messageNum);
        // Serial.print("Number of SVs: ");
        // Serial.println(gsvData.numSVs);
        // for (int i = 0; i < 4; i++) {
        //   Serial.print("SV ID: ");
        //   Serial.println(gsvData.svInfo[i].svID);
        //   Serial.print("Elevation: ");
        //   Serial.println(gsvData.svInfo[i].elevation);
        //   Serial.print("Azimuth: ");
        //   Serial.println(gsvData.svInfo[i].azimuth);
        //   Serial.print("SNR: ");
        //   Serial.println(gsvData.svInfo[i].snr);
        // }
      } else if (nmea.startsWith("$NAVACC")) {
        // Serial.print("NAVACC Time: ");
        // Serial.println(navaccData.time);
        // Serial.print("NAVACC Status: ");
        // Serial.println(navaccData.status);
        // Serial.print("Horizontal Accuracy: ");
        // Serial.println(navaccData.pAcc);
        // Serial.print("Vertical Accuracy: ");
        // Serial.println(navaccData.vAcc);
        // Serial.print("Ground Course Accuracy: ");
        // Serial.println(navaccData.cAcc);
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay to yield control
  }
}
