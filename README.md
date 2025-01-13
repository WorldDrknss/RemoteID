# RemoteID Build - ASTM F3411-22a Compliance

**Experimental, Use at Your Own Risk**

This repository contains the firmware and configuration for deploying a RemoteID solution based on the **ASTM F3411-22a** standard. The solution is designed to run on a **MarkerFocus Wireless Tracker (ESP32)** and was developed using **Arduino Studio**. It enables drones to broadcast Remote ID data via WiFi in compliance with ASTM specifications.

## Prerequisites

- **MarkerFocus Wireless Tracker (ESP32)** hardware.
- **Arduino Studio** installed and configured for ESP32 development.
- Basic understanding of ASTM F3411-22a and FAA Remote ID requirements.
- A compatible drone and environment to test the Remote ID functionality.

## Overview

The project provides an implementation for broadcasting Remote ID messages over WiFi using the ESP32. It supports all required message types defined in ASTM F3411-22a, ensuring full compliance with the standard.

### Key Features:
- Broadcasts all ASTM message types: Basic ID, Location/Vector, System, Operator ID, and more.
- Configurable parameters for UA Type, Emergency Status, Horizontal Speed, Vertical Speed, and others.
- SSID dynamically set to the generated UASID.
- Supports WiFi channel 6 and Beacon Interval (BI) set to 100ms.
- Outputs WiFi transmission power at 20.0 dBm.
- Fully implemented in Arduino Studio for ease of development and deployment.

## Application Configuration

The firmware configuration includes the following key elements:

- **WiFi Settings**: 
  - Channel: 6
  - SSID: Dynamically generated UASID
  - Beacon Interval: 100ms
  - Output Power: 20.0 dBm

- **ASTM Message Types**:
  - **Basic ID**: Identifies the drone uniquely.
  - **Location/Vector**: Provides real-time position and vector information.
  - **System**: Includes system status and capability information.
  - **Operator ID**: Provides information about the operator.
