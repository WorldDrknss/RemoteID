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
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "init.h"

Config config;

void loadConfig() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }

    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile) {
        Serial.println("Failed to open config file");
        return;
    }

    size_t size = configFile.size();
    if (size > 1024) {
        Serial.println("Config file size is too large");
        return;
    }

    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);

    DynamicJsonDocument doc(1024);
    auto error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.print(F("Failed to parse config file: "));
        Serial.println(error.c_str());
        return;
    }

    config.enableBT = doc["config"]["enableBT"];
    config.enableWiFi = doc["config"]["enableWiFi"];
    config.operatorInfo.OperatorID = doc["operator"]["OperatorID"].as<String>();
    config.operatorInfo.OperatorLatitude = doc["operator"]["OperatorLatitude"];
    config.operatorInfo.OperatorLongitude = doc["operator"]["OperatorLongitude"];
    config.operatorInfo.OperatorTimezone = doc["operator"]["OperatorTimezone"].as<String>();

    Serial.println("Config loaded successfully");
    Serial.println("enableBT: " + String(config.enableBT));
    Serial.println("enableWiFi: " + String(config.enableWiFi));
    Serial.println("OperatorID: " + config.operatorInfo.OperatorID);
    Serial.println("OperatorLatitude: " + String(config.operatorInfo.OperatorLatitude));
    Serial.println("OperatorLongitude: " + String(config.operatorInfo.OperatorLongitude));
    Serial.println("OperatorTimezone: " + config.operatorInfo.OperatorTimezone);
}
