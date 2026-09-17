# SafeGas IoT – Smart Gas Leakage Detection and Automatic Safety System

## Project Overview

SafeGas IoT is an IoT-enabled smart gas leakage detection and automatic safety system designed to detect LPG/gas leakage and provide immediate alerts and safety actions.

The system uses an MQ-2 gas sensor connected to an ESP32 to monitor gas concentration. When the detected gas level exceeds the predefined threshold, the system activates safety mechanisms such as the buzzer, warning LED, relay control, and servo motor. The system is also integrated with the Blynk IoT platform for remote monitoring and notification.

## Objectives

- Detect gas leakage using the MQ-2 gas sensor.
- Monitor gas levels using ESP32.
- Provide an immediate warning through buzzer and LED.
- Automatically control the relay and servo motor during gas leakage.
- Send gas leakage notifications through the Blynk IoT platform.
- Enable remote monitoring of the system.

## Hardware Components

- ESP32 Development Board
- MQ-2 Gas Sensor
- Relay Module
- Servo Motor
- Buzzer
- Red LED
- Green LED
- Connecting Wires
- Breadboard
- Power Supply

## Software and Technologies

- Arduino IDE
- Embedded C/C++
- Blynk IoT Platform
- ESP32 Wi-Fi Connectivity

## System Operation

1. The MQ-2 gas sensor continuously monitors the surrounding environment.
2. The ESP32 reads the sensor value.
3. The sensor value is compared with the predefined threshold.
4. Under normal conditions, the system remains in the normal state.
5. When gas leakage is detected, the warning devices are activated.
6. The relay and servo motor perform the automatic safety action.
7. The Blynk IoT platform provides remote monitoring and notifications.

## Source Code

The complete Arduino IDE source code is available in this repository.

**Main file:** `SafeGas_IoT.ino`

## Team Members

| Name | Register Number |
|---|---|
| Subashinee N | 24CS0937 |
| Dhivya Bharathi R | 24CS0220 |

## Project

**Project Title:** SafeGas IoT – Smart Gas Leakage Detection and Automatic Safety System

This project was developed as part of the Semester 5 academic project.
