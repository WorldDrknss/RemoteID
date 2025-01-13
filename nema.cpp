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
#include "nema.h"

double convertToDecimalDegrees(double coordinate, char direction) {
  int degrees = int(coordinate / 100);
  double minutes = coordinate - (degrees * 100);
  double decimalDegrees = degrees + (minutes / 60.0);

  if (direction == 'S' || direction == 'W') {
    decimalDegrees = -decimalDegrees;
  }

  return decimalDegrees;
}

double parseDouble(String value) {
  return value.toFloat();
}

int parseInt(String value) {
  return value.toInt();
}

void parseGGA(String gga, GGAData &ggaData) {
  int idx = 0;
  int fieldIndex = 0;
  String field;

  while (idx < gga.length()) {
    int endIdx = gga.indexOf(',', idx);
    if (endIdx == -1) endIdx = gga.length();

    field = gga.substring(idx, endIdx);

    switch (fieldIndex) {
      case 1:  // UTC Time
        ggaData.time = field;
        break;
      case 2:  // Latitude
        ggaData.latitude = parseDouble(field);
        break;
      case 3:  // Latitude Direction
        ggaData.latDir = field.charAt(0);
        break;
      case 4:  // Longitude
        ggaData.longitude = parseDouble(field);
        break;
      case 5:  // Longitude Direction
        ggaData.lonDir = field.charAt(0);
        break;
      case 6:  // Fix Quality
        ggaData.fixQuality = parseInt(field);
        break;
      case 7:  // Number of Satellites
        ggaData.numSatellites = parseInt(field);
        break;
      case 8:  // HDOP
        ggaData.hdop = parseDouble(field);
        break;
      case 9:  // Altitude
        ggaData.altitude = parseDouble(field);
        break;
      case 10:  // Altitude Unit
        ggaData.altUnit = field.charAt(0);
        break;
      case 11:  // Geoid Height
        ggaData.geoidHeight = parseDouble(field);
        break;
      case 12:  // Geoid Unit
        ggaData.geoUnit = field.charAt(0);
        break;
      case 13:  // DGPS Age
        ggaData.dgpsAge = parseDouble(field);
        break;
      case 14:  // DGPS Station ID
        ggaData.dgpsStationId = parseInt(field);
        break;
    }

    idx = endIdx + 1;
    fieldIndex++;
  }

  // Convert latitude and longitude to proper format
  ggaData.latitude = convertToDecimalDegrees(ggaData.latitude, ggaData.latDir);
  ggaData.longitude = convertToDecimalDegrees(ggaData.longitude, ggaData.lonDir);
}

void parseRMC(String rmc, RMCData &rmcData) {
  int idx = 0;
  int fieldIndex = 0;
  String field;

  while (idx < rmc.length()) {
    int endIdx = rmc.indexOf(',', idx);
    if (endIdx == -1) endIdx = rmc.length();

    field = rmc.substring(idx, endIdx);

    switch (fieldIndex) {
      case 1:  // UTC Time
        rmcData.time = field;
        break;
      case 2:  // Status
        rmcData.status = field.charAt(0);
        break;
      case 3:  // Latitude
        rmcData.latitude = parseDouble(field);
        break;
      case 4:  // Latitude Direction
        rmcData.latDir = field.charAt(0);
        break;
      case 5:  // Longitude
        rmcData.longitude = parseDouble(field);
        break;
      case 6:  // Longitude Direction
        rmcData.lonDir = field.charAt(0);
        break;
      case 7:  // Speed
        rmcData.speed = parseDouble(field);
        break;
      case 8:  // Course
        rmcData.course = parseDouble(field);
        break;
      case 9:  // Date
        rmcData.date = field;
        break;
      case 10:  // Magnetic Variation
        rmcData.magVar = parseDouble(field);
        break;
      case 11:  // Magnetic Variation Direction
        rmcData.magVarDir = field.charAt(0);
        break;
      case 12:  // Mode
        rmcData.mode = field.charAt(0);
        break;
    }

    idx = endIdx + 1;
    fieldIndex++;
  }

  // Convert latitude and longitude to proper format
  rmcData.latitude = convertToDecimalDegrees(rmcData.latitude, rmcData.latDir);
  rmcData.longitude = convertToDecimalDegrees(rmcData.longitude, rmcData.lonDir);
}


void parseGSA(String gsa, GSAData &gsaData) {
  int idx = 0;
  int endIdx = gsa.indexOf(',', idx);
  gsaData.mode1 = gsa.charAt(idx);

  idx = endIdx + 1;
  endIdx = gsa.indexOf(',', idx);
  gsaData.mode2 = parseInt(gsa.substring(idx, endIdx));

  for (int i = 0; i < 12; i++) {
    idx = endIdx + 1;
    endIdx = gsa.indexOf(',', idx);
    gsaData.sv[i] = parseInt(gsa.substring(idx, endIdx));
  }

  idx = endIdx + 1;
  endIdx = gsa.indexOf(',', idx);
  gsaData.pdop = parseDouble(gsa.substring(idx, endIdx));

  idx = endIdx + 1;
  endIdx = gsa.indexOf(',', idx);
  gsaData.hdop = parseDouble(gsa.substring(idx, endIdx));

  idx = endIdx + 1;
  gsaData.vdop = parseDouble(gsa.substring(idx));
}

void parseGSV(String gsv, GSVData &gsvData) {
  int idx = 0;
  int endIdx = gsv.indexOf(',', idx);
  gsvData.numMessages = parseInt(gsv.substring(idx, endIdx));

  idx = endIdx + 1;
  endIdx = gsv.indexOf(',', idx);
  gsvData.messageNum = parseInt(gsv.substring(idx, endIdx));

  idx = endIdx + 1;
  endIdx = gsv.indexOf(',', idx);
  gsvData.numSVs = parseInt(gsv.substring(idx, endIdx));

  for (int i = 0; i < 4; i++) {
    idx = endIdx + 1;
    endIdx = gsv.indexOf(',', idx);
    gsvData.svInfo[i].svID = parseInt(gsv.substring(idx, endIdx));

    idx = endIdx + 1;
    endIdx = gsv.indexOf(',', idx);
    gsvData.svInfo[i].elevation = parseInt(gsv.substring(idx, endIdx));

    idx = endIdx + 1;
    endIdx = gsv.indexOf(',', idx);
    gsvData.svInfo[i].azimuth = parseInt(gsv.substring(idx, endIdx));

    idx = endIdx + 1;
    endIdx = gsv.indexOf(',', idx);
    gsvData.svInfo[i].snr = parseInt(gsv.substring(idx, endIdx));
  }
}

void parseNAVACC(String nmea, NAVACCData &navaccData) {
    int idx = nmea.indexOf(',');
    int endIdx = nmea.indexOf(',', idx + 1);
    navaccData.pAcc = nmea.substring(idx + 1, endIdx).toInt();

    idx = endIdx;
    endIdx = nmea.indexOf(',', idx + 1);
    navaccData.vAcc = nmea.substring(idx + 1, endIdx).toInt();

    idx = endIdx;
    endIdx = nmea.indexOf(',', idx + 1);
    navaccData.cAcc = nmea.substring(idx + 1, endIdx).toInt();
}


void parseNMEA(String nmea, GGAData &ggaData, RMCData &rmcData, GSAData &gsaData, GSVData &gsvData, NAVACCData &navaccData) {
  if (nmea.startsWith("$GPGGA") || nmea.startsWith("$GBGGA") || nmea.startsWith("$GAGGA") || nmea.startsWith("$GLGGA") || nmea.startsWith("$GNGGA")) {
    parseGGA(nmea, ggaData);
  } else if (nmea.startsWith("$GPRMC") || nmea.startsWith("$GBRMC") || nmea.startsWith("$GARMC") || nmea.startsWith("$GLRMC") || nmea.startsWith("$GNRMC")) {
    parseRMC(nmea, rmcData);
  } else if (nmea.startsWith("$GPGSA") || nmea.startsWith("$GBGSA") || nmea.startsWith("$GAGSA") || nmea.startsWith("$GLGSA") || nmea.startsWith("$GNGSA")) {
    parseGSA(nmea, gsaData);
  } else if (nmea.startsWith("$GPGSV") || nmea.startsWith("$GBGSV") || nmea.startsWith("$GAGSV") || nmea.startsWith("$GLGSV") || nmea.startsWith("$GNGSV")) {
    parseGSV(nmea, gsvData);
  } else if (nmea.startsWith("$NAVACC")) {
    parseNAVACC(nmea, navaccData);
  }
}
