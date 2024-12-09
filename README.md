# Hand Gesture Controlled Robotic Arm

This Arduino-based project features a **hand gesture-controlled robotic arm**, leveraging wireless communication to operate. The project comprises two units: a **transmitter** and a **receiver**, enabling the robotic arm to mimic hand movements in real time.

## Table of Contents
- [Project Overview](#project-overview)
- [Hardware Requirements](#hardware-requirements)
- [System Design](#system-design)
  - [Transmitter Unit](#transmitter-unit)
  - [Receiver Unit](#receiver-unit)
- [Features](#features)
- [Setup and Usage](#setup-and-usage)
- [Circuit Diagrams](#circuit-diagrams)
- [Code](#code)
- [Future Improvements](#future-improvements)
- [Acknowledgments](#acknowledgments)

---

## Project Overview

This project allows for controlling a 4-degree-of-freedom (4DOF) robotic arm wirelessly using hand gestures.

### Key Concepts:
- **Hand Movement Tracking**: Using the MPU6050 sensor to measure accelerations and angular velocities of the hand.
- **Wireless Communication**: Using the nRF24L01 RF module for robust and real-time communication.
- **Robotic Arm Control**: A 4DOF robotic arm mimics the gestures transmitted.

---

## Hardware Requirements

### Transmitter Unit
- **Arduino Nano**: Microcontroller for processing hand movements.
- **MPU6050**: Accelerometer and gyroscope sensor for detecting hand gestures.
- **nRF24L01**: RF module for transmitting data wirelessly.
- **Tact Switch**: For toggling modes or resetting states.
- **Power Supply**: 9V battery or USB power.

### Receiver Unit
- **Arduino Uno/Mega**: Microcontroller for controlling the robotic arm servos.
- **4DOF Robotic Arm Kit**: Includes servos for arm movement.
- **nRF24L01**: RF module for receiving data from the transmitter.

---

## System Design

### Transmitter Unit
1. **Sensor Data Acquisition**:
   - The **MPU6050** detects hand movements in three axes.
2. **Data Processing**:
   - The Arduino Nano processes sensor data to determine gestures and maps them to corresponding robotic arm actions.
3. **Data Transmission**:
   - Processed data is sent via the nRF24L01 module to the receiver unit.

### Receiver Unit
1. **Data Reception**:
   - The nRF24L01 receives gesture data from the transmitter.
2. **Action Execution**:
   - Arduino Uno processes the received commands to control the servos of the 4DOF robotic arm.
3. **Servo Movement**:
   - Each servo adjusts its position based on the mapped gestures.

---

## Features
- **Wireless Control**: Transmits hand gestures up to 30 meters in an open area.
- **Real-Time Response**: Immediate robotic arm movement with minimal latency.
- **Multi-Axis Control**: Control the robotic arm in multiple degrees of freedom (up, down, left, right, etc.).
- **Modular Design**: Easy to expand or modify.

---

## Setup and Usage

### Step 1: Hardware Connections
- Assemble the transmitter and receiver units as per the circuit diagrams.
- Connect servos to the robotic arm kit and ensure power stability.

### Step 2: Uploading Code
- Upload the transmitter and receiver codes to the respective Arduino boards.

### Step 3: Powering Up
- Power both the transmitter and receiver units.
- Ensure both nRF24L01 modules are on the same channel and configured with the same settings.

### Step 4: Testing
- Wear the transmitter on your hand and move it to test the robotic arm's response.
- Use the tact switch to calibrate or reset the system if needed.

---

## Circuit Diagrams

### Transmitter Circuit
- **Connections**:
  - **MPU6050 -> Arduino Nano**
    - VCC -> 3.3V
    - GND -> GND
    - SCL -> A5
    - SDA -> A4
  - **nRF24L01 -> Arduino Nano**
    - VCC -> 3.3V
    - GND -> GND
    - CE -> D9
    - CSN -> D8
    - SCK -> D13
    - MOSI -> D11
    - MISO -> D12
  - **Tact Switch -> Arduino Nano**
    - One side -> GND
    - Other side -> D2 (with a pull-up resistor).

### Receiver Circuit
- **Connections**:
  - **nRF24L01 -> Arduino Uno**
    - Same as transmitter.
  - **Servos -> Arduino Uno**
    - Signal pins -> PWM pins (D3, D5, D6, D9).
    - VCC -> External power supply (5V).
    - GND -> Common ground with Arduino.

---

## Code

The Arduino codes for the transmitter and receiver are available in the `/Hand_Gesture_Transmitter` and  `/Robotic_Arm_Receiver` folder respectively. 

- **Transmitter Code**: Captures hand movements and sends data.
- **Receiver Code**: Receives data and controls servos accordingly.

---

## Future Improvements
- Implementing **machine learning** to classify gestures for more complex control.
- Adding a **gripper** for object manipulation.
- Using **Bluetooth or Wi-Fi** for extended range and reliability.
- Developing a **mobile app** for monitoring and additional control.

---

Feel free to contribute to this project by creating a pull request or reporting issues! 😊
