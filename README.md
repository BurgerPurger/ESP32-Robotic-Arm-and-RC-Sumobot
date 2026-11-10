# ESP32 Robotic Arm and Bluetooth-Controlled Sumobot

## Project Overview

The ESP32 Robotic Arm and Bluetooth-Controlled Sumobot is an embedded systems project that integrates a 4-DOF robotic arm into a mobile robot platform.

The system uses an ESP32 development board as its main controller, allowing wireless operation through the Dabble mobile application. The robotic arm uses servo motors for joint movement and object gripping, while the Sumobot's wheels are controlled through an L298N motor driver.

The project combines embedded programming, hardware integration, wireless communication, motor control, and mechanical design.

## Features

- **4-DOF Robotic Arm:** Controls the base, shoulder, elbow, and claw.
- **Bluetooth Control:** Uses the Dabble GamePad application for wireless operation.
- **ESP32 Integration:** Uses the ESP32's built-in Bluetooth functionality.
- **Continuous-Rotation Servos:** Controls the shoulder and elbow through direction and speed commands.
- **Step-Based Base Movement:** Moves the base in 1-degree increments within a restricted range.
- **Claw Position Control:** Opens and closes the gripper with predefined movement limits.
- **Mobile Robot Control:** Drives the Sumobot forward, backward, and through pivot turns.
- **Integrated Control System:** Operates the robotic arm and mobile platform using a single ESP32.

## Hardware Components

| Component | Purpose |
|---|---|
| ESP32 30-Pin Development Board | Main microcontroller |
| ESP32 Expansion Board | Provides accessible connections and 5 V power rails |
| Acrylic 4-DOF Robotic Arm Kit | Mechanical structure of the arm |
| 2× SG90S Positional Servos | Base and claw movement |
| 2× 360° Continuous-Rotation Servos | Shoulder and elbow movement |
| L298N Motor Driver | Controls the Sumobot's wheel motors |
| 11.1 V 3S LiPo Battery | Main power source for the system |
| DC-DC Step-Down Converter | Regulates battery voltage |
| Connecting Wires | Electrical connections |

## Software and Technologies

- C++
- Arduino IDE
- ESP32
- DabbleESP32 Library
- ESP32Servo Library
- Bluetooth Communication
- PWM Motor Control

## System Architecture

The ESP32 acts as the central controller of the system.

The Dabble application sends wireless gamepad commands to the ESP32. These commands are processed by the control program and translated into servo signals or motor driver outputs.

The system has two main control sections:

**1. Robotic Arm Control**
- Base rotation using a positional servo.
- Shoulder movement using a continuous-rotation servo.
- Elbow movement using a continuous-rotation servo.
- Claw opening and closing using a positional servo.

**2. Sumobot Control**
- Forward and backward movement.
- Left and right pivot turning.
- Independent speed adjustment for the left and right wheels.
- L298N motor driver control through ESP32 GPIO and PWM signals.

## Pin Configuration

### Robotic Arm

| Component | ESP32 GPIO |
|---|---|
| Base Servo | GPIO 13 |
| Shoulder Servo | GPIO 25 |
| Elbow Servo | GPIO 14 |
| Claw Servo | GPIO 26 |

### L298N Motor Driver

| Connection | ESP32 GPIO |
|---|---|
| Left Motor ENA | GPIO 32 |
| Left Motor IN1 | GPIO 33 |
| Left Motor IN2 | GPIO 18 |
| Right Motor ENB | GPIO 22 |
| Right Motor IN1 | GPIO 19 |
| Right Motor IN2 | GPIO 21 |

## Dabble GamePad Controls

The system uses the Dabble GamePad module in Digital mode.

| Dabble Button | Function |
|---|---|
| Up | Move Sumobot forward |
| Down | Move Sumobot backward |
| Left / Right | Pivot Sumobot when Up or Down is held |
| Left / Right | Rotate robotic arm base when Up and Down are released |
| Triangle | Raise elbow |
| Cross | Lower elbow |
| Select + Up | Move shoulder forward |
| Select + Down | Move shoulder backward |
| Circle | Open claw |
| Square | Close claw |
| Start | Return arm to home position |

The Select button reserves the directional controls for shoulder movement and prevents simultaneous Sumobot movement.

## Movement Constraints and Calibration

The system uses predefined movement limits to prevent excessive servo movement.

| Joint | Configuration |
|---|---|
| Base | 70°–110°, with 90° as center |
| Shoulder | Continuous rotation; controlled by direction and speed |
| Elbow | Continuous rotation; controlled by direction and speed |
| Claw | 88° closed and 120° maximum open |

The base moves in 1-degree increments with a 40 ms interval between steps. The continuous-rotation servos use calibrated neutral values to stop their movement.

## Power Configuration

The ESP32 expansion board is configured to use its 5 V power rails for the servos. During the documented testing, the servos were powered through the USB-supplied 5 V rail without a separate external servo battery.

The shoulder and elbow experienced difficulty lifting the arm to higher positions. A stronger dedicated servo power supply is a planned improvement, with a common ground maintained between the power supply and controller.

The 11.1 V LiPo battery and DC-DC converter are part of the intended overall power configuration.

## 3D Modeling

The robotic arm and revised Sumobot enclosure were modeled to plan the integration of the arm into the mobile platform.

The enclosure design provides space for the electronic components, while the roof serves as the mounting platform for the robotic arm using M3 nuts and bolts.

## Development Tools

- Arduino IDE – Embedded software development.
- AutoCAD – 3D design and project visualization.
- Dabble – Mobile wireless gamepad control.

## Project Information

**Course:** CPE 014A – Embedded Systems  
**Institution:** Technological Institute of the Philippines – Quezon City  
**Project Type:** Academic Project  
**Controller:** ESP32 (30-Pin Development Board)  
**Programming Language:** C++
