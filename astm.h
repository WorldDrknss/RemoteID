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
// Definitions and structures required by ASTM F3411-22a standard
#ifndef ASTM_F3411_22A_H
#define ASTM_F3411_22A_H

// Enumeration for message types in ASTM F3411-22a standard
typedef enum Messages {
  msBasicID = 0x0,         // Basic ID message type (0x0)
  msLocation = 0x1,        // Location/Vector message type (0x1)
  msAuthentication = 0x2,  // Authentication message type (0x2)
  msSelfID = 0x3,          // Self ID message type (0x3)
  msSystem = 0x4,          // System message type (0x4)
  msOperatorID = 0x5,      // Operator ID message type (0x5)
  msMessagePack = 0xF      // Message Pack message type (0xF)
} msType;

// Enumeration for different types of IDs in ASTM F3411-22a standard
typedef enum ID {
  idNone = 0,    // No ID type (0)
  idSerial = 1,  // Serial number ID type (1)
  idCAA = 2,     // CAA (Civil Aviation Authority) compliant ID type (2)
  idUTM = 3,     // UTM (Unmanned Traffic Management) compliant ID type (3)
  idSession = 4  // Session ID type (4)
} idTypes;

// Enumeration for different types of Unmanned Aircraft (UA) in ASTM F3411-22a standard
typedef enum UA {
  uaUndefined = 0,       // Undefined UA type (0)
  uaAeroplane = 1,       // Aeroplane UA type (1)
  uaMultirotor = 2,      // Multirotor UA type (2)
  uaGyroplane = 3,       // Gyroplane UA type (3)
  uaFixedWing = 4,       // Fixed-wing UA type (4)
  uaOrnithopter = 5,     // Ornithopter UA type (5)
  uaGlider = 6,          // Glider UA type (6)
  uaKite = 7,            // Kite UA type (7)
  uaFreeBalloon = 8,     // Free balloon UA type (8)
  uaCaptiveBalloon = 9,  // Captive balloon UA type (9)
  uaAirship = 10,        // Airship UA type (10)
  uaFreeFall = 11,       // Free-fall UA type (11)
  uaRocket = 12,         // Rocket UA type (12)
  uaTethered = 13,       // Tethered UA type (13)
  uaGround = 14,         // Ground UA type (14)
  uaOther = 15           // Other UA type (15)
} uaTypes;

// Enumeration for different types of Operational Status in ASTM F3411-22a standard
typedef enum OperationalStatus {
  osUndeclared = 0,    // Undeclared operational status (0)
  osGround = 1,        // Ground operational status (1)
  osAirborne = 2,      // Airborne operational status (2)
  osEmergency = 3,     // Emergency operational status (3)
  osSystemFailure = 4  // System failure operational status (4)
} osTypes;

// Enumeration for different types of Operator Location in ASTM F3411-22a standard
typedef enum OperatorLocation {
  olTakeoff = 0,  // Operator location at the takeoff point (0)
  olDynamic = 1,  // Dynamic operator location (moving) (1)
  olFixed = 2     // Fixed operator location (stationary) (2)
} olTypes;

// Enumeration for different classification types in ASTM F3411-22a standard
// Enumeration for different types of Classification in ASTM F3411-22a standard
typedef enum Classification {
  cfUndeclared = 0,     // Undeclared classification (0)
  cfEuropeanUnion = 1,  // European Union classification (1)
                        // 2 - 7 Reserved for future use
} cfTypes;

// Enumeration for different types of Categories in ASTM F3411-22a standard
typedef enum Categories {
  euUndefined = 0,  // Undefined category (0)
  euOpen = 1,       // Open category (1)
  euSpecific = 2,   // Specific category (2)
  euCertified = 3   // Certified category (3)
                    // 4 - 15 Reserved for future use
} euTypes;

// Enumeration for different types of Classes in ASTM F3411-22a standard
typedef enum Classes {
  clUndefined = 0,  // Undefined class (0)
  clClass0 = 1,     // Class 0 (1)
  clClass1 = 2,     // Class 1 (2)
  clClass2 = 3,     // Class 2 (3)
  clClass3 = 4,     // Class 3 (4)
  clClass4 = 5,     // Class 4 (5)
  clClass5 = 6      // Class 5 (6)
                    // 7 - 15 Reserved for future use
} clTypes;


// Structure for Message Header as defined in ASTM F3411-22a standard
typedef struct MessageHeader {
  uint8_t messageType_protocolVersion;  // Bits 7..4: Message Type, Bits 3..0: Protocol Version
} MessageHeader;

// Structure for Basic ID Message as defined in ASTM F3411-22a standard
typedef struct BasicIDMessage {
  MessageHeader header;   // 1 byte
  uint8_t idType_uaType;  // ID Type (upper 4 bits) and UA Type (lower 4 bits) - 1 byte
  uint8_t uasID[20];      // UAS ID (20 bytes)
  uint8_t reserved[3];    // Reserved bytes (3 bytes)
} BasicIDMessage;


// Structure for Location/Vector Message as defined in ASTM F3411-22a standard
typedef struct LocationMessage {
  MessageHeader header;               // 1 byte
  uint8_t status;                     // 1 byte
  uint8_t trackDirection;             // 1 byte
  uint8_t speed;                      // 1 byte
  int8_t verticalSpeed;               // 1 byte
  int32_t latitude;                   // 4 bytes
  int32_t longitude;                  // 4 bytes
  int16_t pressureAltitude;           // 2 bytes
  int16_t geodeticAltitude;           // 2 bytes
  int16_t height;                     // 2 bytes
  uint8_t horVertAccuracy;            // 1 byte (Combined Horizontal and Vertical Accuracy)
  uint8_t baroSpeedAccuracy;          // 1 byte (Combined Baro Altitude Accuracy and Speed Accuracy)
  uint16_t timestamp;                 // 2 bytes
  uint8_t reservedTimestampAccuracy;  // 1 byte (Combined Reserved and Timestamp Accuracy)
  uint8_t reserved;                   // 1 byte
} __attribute__((packed)) LocationMessage;


// Structure for Authentication Message as defined in ASTM F3411-22a standard
typedef struct AuthenticationMessage {
  MessageHeader header;  // Message header
  uint8_t authType;      // Authentication Type
  uint8_t authData[23];  // Authentication Data
} AuthenticationMessage;

// Structure for Self-ID Message as defined in ASTM F3411-22a standard
typedef struct SelfIDMessage {
  MessageHeader header;     // Message header
  uint8_t descriptionType;  // Description Type
  char description[23];     // ASCII Text Description (padded with nulls)
} SelfIDMessage;

// Structure for System Message as defined in ASTM F3411-22a standard
typedef struct SystemMessage {
  MessageHeader header;       // Message header
  uint8_t flags;              // Flags (1 byte)
  int32_t operatorLatitude;   // Latitude of Remote Pilot (4 bytes)
  int32_t operatorLongitude;  // Longitude of Remote Pilot (4 bytes)
  uint16_t areaCount;         // Number of aircraft in Area (2 bytes)
  uint16_t areaRadius;        // Radius of area (2 bytes)
  int16_t areaCeiling;        // Area ceiling (2 bytes)
  int16_t areaFloor;          // Area floor (2 bytes)
  uint8_t classification;     // Classification (1 byte)
  int16_t operatorAltitude;   // Altitude of Remote Pilot (2 bytes)
  uint32_t timestamp;         // Timestamp (4 bytes)
  uint8_t reserved;           // Reserved (1 byte)
} __attribute__((packed)) SystemMessage;

// Structure for Operator-ID Message as defined in ASTM F3411-22a standard
typedef struct OperatorIDMessage {
  MessageHeader header;    // Message header
  uint8_t operatorIDType;  // Operator ID Type (0: Operator ID, 1-200: Reserved, 201-255: Available for private use)
  char operatorID[20];     // ASCII Text for Operator ID (padded with nulls)
  uint8_t reserved[3];     // Reserved bytes
} OperatorIDMessage;

// Horizontal Accuracy Enumeration
typedef enum HorizontalAccuracy {
    H_ACC_UNKNOWN = 0,
    H_ACC_10_NM = 1,
    H_ACC_4_NM = 2,
    H_ACC_2_NM = 3,
    H_ACC_1_NM = 4,
    H_ACC_0_5_NM = 5,
    H_ACC_0_3_NM = 6,
    H_ACC_0_1_NM = 7,
    H_ACC_0_05_NM = 8,
    H_ACC_30_M = 9,
    H_ACC_10_M = 10,
    H_ACC_3_M = 11,
    H_ACC_1_M = 12,
    H_ACC_RESERVED = 13
} HorizontalAccuracy;

// Vertical Accuracy Enumeration
typedef enum VerticalAccuracy {
    V_ACC_UNKNOWN = 0,
    V_ACC_150_M = 1,
    V_ACC_45_M = 2,
    V_ACC_25_M = 3,
    V_ACC_10_M = 4,
    V_ACC_3_M = 5,
    V_ACC_1_M = 6
} VerticalAccuracy;

// Speed Accuracy Enumeration
typedef enum SpeedAccuracy {
    S_ACC_UNKNOWN = 0,
    S_ACC_10_M_S = 1,
    S_ACC_3_M_S = 2,
    S_ACC_1_M_S = 3,
    S_ACC_0_3_M_S = 4
} SpeedAccuracy;

// Define VendorSpecificTag structure
typedef struct VendorSpecificTag {
  uint8_t length;
  uint8_t oui[3];
  uint8_t vendType;
  uint8_t msgCounter;
  uint8_t advData[256];  // Example size, should be adjusted according to the actual requirement
} VendorSpecificTag;

typedef struct MessagePack {
  MessageHeader header;     // Message header
  uint8_t messageSize;      // Message Size (25 Bytes)
  uint8_t numMessages;      // Number of messages in the pack (up to 9)
  uint8_t messages[9][25];  // Array of messages, up to 9 messages, each 25 bytes long
} MessagePack;

#endif  // ASTM_F3411_22A_H