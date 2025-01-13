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
#ifndef ENCODERS_H
#define ENCODERS_H

#include <astm.h>

// Function declarations for encoding messages
inline void encodeMessageHeader(MessageHeader *header, uint8_t messageType, uint8_t protocolVersion);
inline void encodeBasicIDMessage(BasicIDMessage *msg, idTypes idType, uaTypes uaType, const char *uasID);
inline void encodeLocationMessage(LocationMessage *msg, uint8_t status, uint8_t trackDirection, uint8_t speed, int8_t verticalSpeed, float latitude, float longitude, float pressureAltitude, float geodeticAltitude, int16_t height, uint32_t horAccuracy, uint32_t vertAccuracy, uint32_t speedAccuracy, uint16_t timestamp);
inline void encodeAuthenticationMessage(AuthenticationMessage *msg, uint8_t authType, const uint8_t *authData);
inline void encodeSelfIDMessage(SelfIDMessage *msg, uint8_t descriptionType, const char *description);
inline void encodeSystemMessage(SystemMessage *msg, uint8_t operatorLocationType, int32_t operatorLatitude, int32_t operatorLongitude, uint16_t areaCount, uint16_t areaRadius, int16_t areaCeiling, int16_t areaFloor, uint8_t classificationType, uint8_t uaCategory, uint8_t uaClass, int16_t operatorAltitude, uint32_t timestamp);
inline void encodeOperatorIDMessage(OperatorIDMessage *msg, uint8_t operatorIDType, const char *operatorID);
inline void encodeVendorSpecificTag(VendorSpecificTag *vendorTag, MessagePack *messagePack);
inline uint8_t encodeDirection(uint16_t direction, uint8_t *directionSegment);
inline uint8_t encodeSpeed(float speed, uint8_t *speedMultiplier);
inline int32_t convertLatLonFormat(float value);
inline int8_t encodeVerticalSpeed(float verticalSpeed);
inline uint16_t encodeAltitude(float altitude);
inline uint16_t encodeTimestamp(float timestamp);

inline uint8_t convertHorizontalAccuracy(uint32_t accuracy) {
    if (accuracy >= 18520000) return 0;
    if (accuracy >= 7408000) return 1;
    if (accuracy >= 3704000) return 2;
    if (accuracy >= 1852000) return 3;
    if (accuracy >= 926000) return 4;
    if (accuracy >= 555600) return 5;
    if (accuracy >= 185200) return 6;
    if (accuracy >= 92600) return 7;
    if (accuracy >= 30000) return 8;
    if (accuracy >= 10000) return 9;
    if (accuracy >= 3000) return 10;
    if (accuracy >= 1000) return 11;
    if (accuracy >= 100) return 12;
    return 13;
}

inline uint8_t convertVerticalAccuracy(uint32_t accuracy) {
    if (accuracy >= 15000) return 0;
    if (accuracy >= 4500) return 1;
    if (accuracy >= 2500) return 2;
    if (accuracy >= 1000) return 3;
    if (accuracy >= 300) return 4;
    if (accuracy >= 100) return 5;
    return 6;
}

inline uint8_t convertSpeedAccuracy(uint32_t accuracy) {
    if (accuracy >= 10000) return 0;
    if (accuracy >= 3000) return 1;
    if (accuracy >= 1000) return 2;
    if (accuracy >= 300) return 3;
    return 4;
}

inline uint8_t encodeTrackDirection(float course) {
    if (course < 0 || course > 359) {
        return 0; // Default value if the course is out of range
    }
    uint8_t direction = static_cast<uint8_t>(course);
    if (direction > 179) {
        direction -= 180;
    }
    return direction;
}


// Function definitions
inline void encodeMessageHeader(MessageHeader *header, uint8_t messageType, uint8_t protocolVersion) {
    header->messageType_protocolVersion = (messageType << 4) | (protocolVersion & 0x0F);
}

inline void encodeBasicIDMessage(BasicIDMessage *msg, idTypes idType, uaTypes uaType, const char *uasID) {
    encodeMessageHeader(&msg->header, msBasicID, 0x01);
    msg->idType_uaType = (idType << 4) | (uaType & 0x0F);
    memset(msg->uasID, 0, sizeof(msg->uasID));
    strncpy((char *)msg->uasID, uasID, sizeof(msg->uasID) - 1);
    msg->uasID[sizeof(msg->uasID) - 1] = '\0';
    memset(msg->reserved, 0, sizeof(msg->reserved));
}

inline void encodeLocationMessage(LocationMessage *msg, uint8_t status, uint8_t trackDirection, uint8_t speed, int8_t verticalSpeed, float latitude, float longitude, float pressureAltitude, float geodeticAltitude, int16_t height, uint32_t horAccuracy, uint32_t vertAccuracy, uint32_t speedAccuracy, uint16_t timestamp) {
    encodeMessageHeader(&msg->header, msLocation, 0x01);
    msg->status = (status << 4);
    msg->trackDirection = encodeTrackDirection(trackDirection);
    msg->speed = speed;
    msg->verticalSpeed = verticalSpeed;
    msg->latitude = static_cast<int32_t>(latitude * 1e7); // Convert degrees to degrees * 10^7
    msg->longitude = static_cast<int32_t>(longitude * 1e7); // Convert degrees to degrees * 10^7
    msg->pressureAltitude = encodeAltitude(pressureAltitude);
    msg->geodeticAltitude = encodeAltitude(geodeticAltitude);
    msg->height = height;
    msg->horVertAccuracy = (convertVerticalAccuracy(vertAccuracy) << 4) | (convertHorizontalAccuracy(horAccuracy) & 0x0F);
    msg->baroSpeedAccuracy = (convertSpeedAccuracy(speedAccuracy) << 4) | (convertSpeedAccuracy(speedAccuracy) & 0x0F);
    msg->timestamp = timestamp;
    msg->reservedTimestampAccuracy = 0x00;
    msg->reserved = 0x00;
}

inline void encodeAuthenticationMessage(AuthenticationMessage *msg, uint8_t authType, const uint8_t *authData) {
    encodeMessageHeader(&msg->header, msAuthentication, 0x1);
    msg->authType = authType;
    memcpy(msg->authData, authData, sizeof(msg->authData));
}

inline void encodeSelfIDMessage(SelfIDMessage *msg, uint8_t descriptionType, const char *description) {
    encodeMessageHeader(&msg->header, msSelfID, 0x1);
    msg->descriptionType = descriptionType;
    strncpy(msg->description, description, sizeof(msg->description));
    if (strlen(description) < sizeof(msg->description)) {
        memset(msg->description + strlen(description), 0, sizeof(msg->description) - strlen(description));
    }
}

inline void encodeSystemMessage(SystemMessage *msg, uint8_t operatorLocationType, int32_t operatorLatitude, int32_t operatorLongitude, uint16_t areaCount, uint16_t areaRadius, int16_t areaCeiling, int16_t areaFloor, uint8_t classificationType, uint8_t uaCategory, uint8_t uaClass, int16_t operatorAltitude, uint32_t timestamp) {
    encodeMessageHeader(&msg->header, msSystem, 0x1);
    msg->flags = (classificationType << 4) | operatorLocationType;
    msg->operatorLatitude = operatorLatitude;
    msg->operatorLongitude = operatorLongitude;
    msg->areaCount = areaCount;
    msg->areaRadius = areaRadius;
    msg->areaCeiling = areaCeiling;
    msg->areaFloor = areaFloor;
    msg->classification = (uaCategory << 4) | uaClass;
    msg->operatorAltitude = operatorAltitude;
    msg->timestamp = timestamp;
    msg->reserved = 0;
}

inline void encodeOperatorIDMessage(OperatorIDMessage *msg, uint8_t operatorIDType, const char *operatorID) {
    encodeMessageHeader(&msg->header, msOperatorID, 0x1);
    msg->operatorIDType = operatorIDType;
    strncpy(msg->operatorID, operatorID, sizeof(msg->operatorID));
    if (strlen(operatorID) < sizeof(msg->operatorID)) {
        memset(msg->operatorID + strlen(operatorID), 0, sizeof(msg->operatorID) - strlen(operatorID));
    }
    memset(msg->reserved, 0, sizeof(msg->reserved));
}

inline void encodeVendorSpecificTag(VendorSpecificTag *vendorTag, MessagePack *messagePack) {
    vendorTag->length = 3 + 1 + 1 + (messagePack->numMessages * messagePack->messageSize);
    vendorTag->oui[0] = 0xFA;
    vendorTag->oui[1] = 0x0B;
    vendorTag->oui[2] = 0xBC;
    vendorTag->vendType = 0x0D;
    memcpy(vendorTag->advData, messagePack, sizeof(MessageHeader) + 1 + 1 + (messagePack->numMessages * messagePack->messageSize));
}

inline uint8_t encodeDirection(uint16_t direction, uint8_t *directionSegment) {
    uint8_t encodedValue;
    if (direction < 180) {
        encodedValue = direction;
        *directionSegment = 0;
    } else {
        encodedValue = direction - 180;
        *directionSegment = 1;
    }
    return encodedValue;
}

inline uint8_t encodeSpeed(float speed, uint8_t *speedMultiplier) {
    uint8_t encodedValue;
    if (speed <= 255 * 0.25) {
        encodedValue = speed / 0.25;
        *speedMultiplier = 0;
    } else if (speed > 255 * 0.25 && speed < 254.25) {
        encodedValue = (speed - (255 * 0.25)) / 0.75;
        *speedMultiplier = 1;
    } else {
        encodedValue = 254;
        *speedMultiplier = 1;
    }
    return encodedValue;
}

inline int32_t convertLatLonFormat(float value) {
    // Check if the value is already in the format of degrees * 10^7
    if (value > 1000000.0 || value < -1000000.0) {
        return static_cast<int32_t>(value);
    } else if (value > 360.0 || value < -360.0) {
        // If value is in decimal format (e.g., 12345.6789), convert it to degrees first
        int degrees = static_cast<int>(value / 100);
        float minutes = value - (degrees * 100);
        float decimalDegrees = degrees + (minutes / 60.0);
        return static_cast<int32_t>(decimalDegrees * 1e7);
    } else {
        // Convert degrees to degrees * 10^7
        return static_cast<int32_t>(value * 1e7);
    }
}


inline int8_t encodeVerticalSpeed(float verticalSpeed) {
    if (verticalSpeed > 62.0) {
        verticalSpeed = 62.0;
    } else if (verticalSpeed < -62.0) {
        verticalSpeed = -62.0;
    }
    float dividedValue = verticalSpeed / 0.5;
    int8_t encodedValue = static_cast<int8_t>(dividedValue);
    return encodedValue;
}

inline uint16_t encodeAltitude(float altitude) {
    if (altitude == -1000) {
        return 0;
    }
    return (altitude + 1000) / 0.5;
}

inline uint16_t encodeTimestamp(float timestamp) {
    return timestamp * 10;
}

#endif // ENCODERS_H