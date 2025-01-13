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
#ifndef NEMA_H
#define NEMA_H

#include <Arduino.h>

struct GGAData {
    String time;
    double latitude;
    char latDir;
    double longitude;
    char lonDir;
    int fixQuality;
    int numSatellites;
    double hdop;
    double altitude;
    char altUnit;
    double geoidHeight;
    char geoUnit;
    double dgpsAge;
    int dgpsStationId;
};

struct RMCData {
    String time;
    char status;
    double latitude;
    char latDir;
    double longitude;
    char lonDir;
    double speed;
    double course;
    String date;
    double magVar;
    char magVarDir;
    char mode;
};

struct GSAData {
    char mode1;
    int mode2;
    int sv[12];
    double pdop;
    double hdop;
    double vdop;
};

struct GSVData {
    int numMessages;
    int messageNum;
    int numSVs;
    struct {
        int svID;
        int elevation;
        int azimuth;
        int snr;
    } svInfo[4];
};

struct NAVACCData {
    String time;
    char status;
    uint32_t pAcc; // Horizontal Accuracy
    uint32_t vAcc; // Vertical Accuracy
    uint32_t cAcc; // Speed Accuracy
};

void parseGGA(String nmea, GGAData &ggaData);
void parseRMC(String nmea, RMCData &rmcData);
void parseGSA(String nmea, GSAData &gsaData);
void parseGSV(String nmea, GSVData &gsvData);
void parseNAVACC(String nmea, NAVACCData &navaccData);
void parseNMEA(String nmea, GGAData &ggaData, RMCData &rmcData, GSAData &gsaData, GSVData &gsvData, NAVACCData &navaccData);

double convertToDecimalDegrees(double coordinate, char direction);
double parseDouble(String value);
int parseInt(String value);

#endif // NEMA_H
