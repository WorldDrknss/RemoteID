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
#include "wifiBeacon.h"
#include "init.h"  // Include init.h to access the config variable
#include "gps.h"   // Include gps.h to access the GPS data
#include <WiFi.h>
#include <esp_wifi.h>

// Define the WiFi channel and output power
const int wifiChannel = 6;       // Set WiFi Channel
const int wifiOutputPower = 20;  // dBm
// const int ledPin = LED_BUILTIN;

// WiFi packet buffer
uint8_t beaconPacket[256];
uint8_t packetIndex = 0;
static uint8_t beaconCounter = 0;  // Static variable to keep track of how many times the beacon has been sent

extern Config config;          // Access the global config variable
extern GGAData ggaData;        // Access the GPS GGA data
extern RMCData rmcData;
extern NAVACCData navaccData;  // Access the GPS NAVACC data

void wifiBeaconSetup(const char* ssid) {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(wifiChannel, WIFI_SECOND_CHAN_NONE);
  int channel = wifiChannel;
  int beaconInterval = 100;
  constructBeaconFrame(ssid, channel, beaconInterval);
}

void wifiBeaconTask(void* parameters) {
  for (;;) {
    esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, packetIndex, false);
    beaconCounter++;
    digitalWrite(LED, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    constructBeaconFrame((const char*)parameters, wifiChannel, 100);
    digitalWrite(LED, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void constructBeaconFrame(const char* ssid, int channel, int beaconInterval) {
  packetIndex = 0;
  beaconPacket[packetIndex++] = 0x80;
  beaconPacket[packetIndex++] = 0x00;
  beaconPacket[packetIndex++] = 0x00;
  beaconPacket[packetIndex++] = 0x00;
  memset(&beaconPacket[packetIndex], 0xFF, 6);
  packetIndex += 6;
  uint8_t sourceAddress[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD };
  memcpy(&beaconPacket[packetIndex], sourceAddress, 6);
  packetIndex += 6;
  memcpy(&beaconPacket[packetIndex], sourceAddress, 6);
  packetIndex += 6;
  beaconPacket[packetIndex++] = 0x00;
  beaconPacket[packetIndex++] = 0x00;
  memset(&beaconPacket[packetIndex], 0x00, 8);
  packetIndex += 8;
  beaconPacket[packetIndex++] = beaconInterval & 0xFF;
  beaconPacket[packetIndex++] = (beaconInterval >> 8) & 0xFF;
  beaconPacket[packetIndex++] = 0x01;
  beaconPacket[packetIndex++] = 0x00;
  beaconPacket[packetIndex++] = 0x00;
  uint8_t ssidLength = strlen(ssid);
  beaconPacket[packetIndex++] = ssidLength;
  memcpy(&beaconPacket[packetIndex], ssid, ssidLength);
  packetIndex += ssidLength;
  beaconPacket[packetIndex++] = 0x01;
  beaconPacket[packetIndex++] = 0x08;
  beaconPacket[packetIndex++] = 0x82;
  beaconPacket[packetIndex++] = 0x84;
  beaconPacket[packetIndex++] = 0x8B;
  beaconPacket[packetIndex++] = 0x96;
  beaconPacket[packetIndex++] = 0x0C;
  beaconPacket[packetIndex++] = 0x12;
  beaconPacket[packetIndex++] = 0x18;
  beaconPacket[packetIndex++] = 0x24;
  beaconPacket[packetIndex++] = 0x03;
  beaconPacket[packetIndex++] = 0x01;
  beaconPacket[packetIndex++] = channel;
  encodeVendorSpecificElement(ssid);
}

void encodeVendorSpecificElement(const char* ssid) {
  VendorSpecificTag vendorTag;
  MessagePack messagePack;
  messagePack.messageSize = 25;
  messagePack.numMessages = 5;

  // Basic ID Message
  BasicIDMessage basicIDMessage;
  encodeBasicIDMessage(
    &basicIDMessage,  // Basic ID message struct
    idSerial,         // UAS ID Type
    uaMultirotor,     // UAS Type
    ssid              // UAS ID (SSID)
  );
  memcpy(messagePack.messages[0], &basicIDMessage, sizeof(BasicIDMessage));

  // Location Message
  LocationMessage locationMessage;
  encodeLocationMessage(&locationMessage,
        osAirborne,                         // Status
        static_cast<uint8_t>(rmcData.course),       // Track Direction
        static_cast<uint8_t>(rmcData.speed),        // Speed
        0,                                          // Vertical Speed (can be replaced with actual value if available)
        ggaData.latitude,                           // Latitude
        ggaData.longitude,                          // Longitude
        ggaData.altitude,                           // Pressure Altitude (float)
        ggaData.geoidHeight,                        // Geodetic Altitude (float)
        static_cast<int16_t>(ggaData.altitude),     // Height
        navaccData.pAcc,                            // Horizontal Accuracy
        navaccData.vAcc,                            // Vertical Accuracy
        navaccData.cAcc,                            // Speed/Baro Accuracy
        ggaData.time.toInt()                        // Timestamp
    );                                                                // Timestamp
  memcpy(messagePack.messages[1], &locationMessage, sizeof(LocationMessage));

  // Self ID Message
  SelfIDMessage selfIDMessage;
  encodeSelfIDMessage(
    &selfIDMessage,    // Self ID message struct
    selfIdType,        // Self ID Type
    selfIdDescription  // Self ID Description
  );
  memcpy(messagePack.messages[2], &selfIDMessage, sizeof(SelfIDMessage));

  // System Message
  SystemMessage systemMessage;
  uint8_t uaClass = 0;
  encodeSystemMessage(
    &systemMessage,                         // System message struct
    0,                                      // Operator Location Type
    config.operatorInfo.OperatorLatitude,   // Operator Latitude
    config.operatorInfo.OperatorLongitude,  // Operator Longitude
    1,                                      // Area Count
    0,                                      // Area Radius
    0,                                      // Area Ceiling
    0,                                      // Area Floor
    cfUndeclared,                           // Classification Type
    uaMultirotor,                           // UA Category
    uaClass,                                // UA Class
    0,                                      // Operator Altitude
    0                                       // Timestamp
  );
  memcpy(messagePack.messages[3], &systemMessage, sizeof(SystemMessage));

  // Operator ID Message
  OperatorIDMessage operatorIDMessage;
  encodeOperatorIDMessage(
    &operatorIDMessage,                     // Operator ID message struct
    0,                                      // Operator ID Type
    config.operatorInfo.OperatorID.c_str()  // Operator ID
  );
  memcpy(messagePack.messages[4], &operatorIDMessage, sizeof(OperatorIDMessage));

  encodeMessageHeader(&messagePack.header, msMessagePack, 0x01);

  encodeVendorSpecificTag(&vendorTag, &messagePack);

  beaconPacket[packetIndex++] = 0xDD;
  beaconPacket[packetIndex++] = vendorTag.length;
  memcpy(&beaconPacket[packetIndex], vendorTag.oui, 3);
  packetIndex += 3;
  beaconPacket[packetIndex++] = vendorTag.vendType;
  beaconPacket[packetIndex++] = vendorTag.msgCounter;
  memcpy(&beaconPacket[packetIndex], vendorTag.advData, vendorTag.length - 5);
  packetIndex += vendorTag.length - 5;
}
