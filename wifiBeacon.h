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
#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <enc.h>
#include <astm.h>

// Define the WiFi channel and output power
extern const int wifiChannel;
extern const int wifiOutputPower;
extern const int ledPin;

// Define UA Information
#define uasIDType 1  // Serial
#define uasType 2    // Multirotor

// Define Location Information
// #define uasStatus 2
// #define uasLatitude 336970783     // UAS Latitude
// #define uasLongitude -1171853531  // UAS Longitude
// #define uasAltitude 0             // UAS Altitude in meters
// #define uasHorizontalSpeed 60     // Horizontal Speed in cm/s
// #define uasVerticalSpeed 30       // Vertical Speed in cm/s
// #define uasTrack 180              // Track direction in degrees

// Define Operator Information
// #define operatorID "FC:WorldDrknss"
// #define operatorLatitude 336970783
// #define operatorLongitude -1171853531

// Define Self ID Information
#define selfIdType 0
#define selfIdDescription "Recreational"

void wifiBeaconSetup(const char* ssid);
void wifiBeaconTask(void* parameters);
void printRawBytes(uint8_t* data, size_t length);
void constructBeaconFrame(const char* ssid, int channel, int beaconInterval);
void encodeVendorSpecificElement(const char* ssid);

#endif // WIFI_H
