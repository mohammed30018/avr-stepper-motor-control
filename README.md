# AVR Stepper Motor Control System

## Project Overview

This project implements an AVR-based stepper motor control system using a Master-Slave architecture with I2C communication.

The system is developed in C/C++ and integrates multiple embedded peripherals to provide motor control, user interaction, and hardware safety functionality.

## System Architecture

The project consists of two main modules:

### 1. AVR Application Master
The Master application handles the main control logic and peripheral interaction.

Main software modules include:
- Stepper motor control
- I2C communication
- UART communication
- LCD interface
- Matrix keypad interface
- Emergency stop functionality
- Timer module
- FIFO implementation

### 2. AVR I2C Slave
The Slave application handles I2C communication with the Master and includes the LED matrix software module.

## Technologies Used

- Programming Languages: C / C++
- Microcontroller: AVR
- Communication Protocol: I2C
- Serial Communication: UART
- Hardware Interfaces: LCD, Keypad, LED Matrix
- Motor Control: Stepper Motor
- Development Environment: Microchip Studio / AVR Toolchain (verify exact version)

## Repository Structure

```text
avr-stepper-motor-control/
│
├── AVR_Application_Master/
│   ├── Source Files (.cpp)
│   ├── Header Files (.h)
│   └── Project Files
│
├── AVR01_Slave_I2C/
│   ├── Source Files (.cpp)
│   ├── Header Files (.h)
│   └── Project Files
│
└── README.md
```

## Key Features

- Master-Slave communication using I2C.
- Stepper motor control.
- UART-based communication.
- LCD status display.
- Matrix keypad input.
- Emergency stop and limit switch functionality.
- Modular embedded software architecture.

## Project Purpose

The main objective of this project is to develop practical embedded software skills through microcontroller programming, peripheral interfacing, communication protocols, and hardware control.

## Author

Mohamed Aly

Electrical and Information Engineering Student

Deggendorf Institute of Technology
