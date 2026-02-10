# Bluetooth Controlled RC Car (ESP32)

## Overview
This project implements a four-wheel drive RC car using an ESP32 microcontroller and dual L298N motor drivers. The car is controlled wirelessly via Bluetooth and supports forward, backward, differential steering, and in-place rotation.

---

## Hardware Used
- ESP32 Development Board
- Two L298N Motor Driver Modules
- Four DC Motors (Front-Left, Front-Right, Back-Left, Back-Right)
- Battery Power Supply
- Bluetooth Controller / Mobile Application
- Chassis and Wiring

---

## Software Used
- Arduino IDE
- ESP32 Board Support Package
- BluetoothSerial Library

---

## Motor Configuration
Each motor is independently controlled using one enable (PWM) pin and two direction pins, allowing precise speed and direction control for all four wheels.

---

## Control Logic
The code tracks button press and release states to allow smooth motion and combined actions. Differential steering is achieved by varying motor speeds on the left and right sides.

---

## Control Commands
| Command | Action |
|--------|--------|
| F / f | Forward press / release |
| B / b | Backward press / release |
| Q / q | Rotate right (clockwise) press / release |
| C / c | Rotate left (anti-clockwise) press / release |
| F + Q | Forward-right (differential) |
| F + C | Forward-left (differential) |
| B + Q | Backward-right (differential) |
| B + C | Backward-left (differential) |

---

## Speed Control
- Full speed: PWM value 255  
- Differential speed: PWM value 100  

These values can be adjusted in the code for tuning performance.

---

## How to Use
1. Connect motors to the L298N drivers as per pin definitions.
2. Upload the code to the ESP32 using Arduino IDE.
3. Power the motor drivers and ESP32.
4. Pair with the Bluetooth device named `KHALPAAR_RC_Car`.
5. Send control commands from the controller application.

---

## Features
- Independent control of all four motors
- Differential steering
- Smooth combined motion handling
- Bluetooth-based wireless control

---

## Author
Soham Bhattacharya

---

## License
This project is intended for educational and non-commercial use.
