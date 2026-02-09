# mini_tunel_smartx

## Overview

mini_tunel_smartx is an embedded automation project designed for the ESP32 platform, implementing a smart tunnel system with sensors, motor control, RGB LED feedback, display interface, and WiFi network management. The system is modular, with separate components for controller logic, display, motor, sensors, feeder, and network communication.

## Features

- **ESP32-based**: Utilizes the ESP32 microcontroller for robust performance and WiFi connectivity.
- **Modular Architecture**: Code is organized into modules for controller, display, motor, sensors, feeder, RGB LED, and network.
- **Sensor Integration**: Supports multiple sensors for box detection, emergency stop, and feeder status.
- **Motor Control**: Controls main and feeder motors with speed and direction settings.
- **Display Support**: Integrates with UnicViewAD display for user feedback and control.
- **WiFi Access Point**: Creates a WiFi AP for device management and status monitoring.
- **Web API**: Exposes endpoints for status monitoring and control via HTTP.
- **Settings Management**: Stores and restores last used settings for motor speed and retry count.
- **Task Scheduling**: Uses FreeRTOS tasks for concurrent operations.

## Hardware Requirements

- ESP32 (tested on ESP32-S3)
- UnicViewAD display (connected via Serial2)
- R700 sensor module
- Motors (main and feeder)
- RGB LED
- Various sensors (IN, OUT, Emergency, Feeder)

## Pinout

See `pins.h` for all pin definitions:

- R700: READ (18), OK (11), NOK (12)
- Motor: ON (39), REVERSE (40), SPEED1 (41), SPEED2 (42)
- Feeder Motor: ON (16), REVERSE (17)
- Sensors: EMG (5), IN (6), OUT (7), FEEDER (15)
- Display: TX (13), RX (14)

## Getting Started

1. **Clone the repository** to your local machine.
2. **Open the project** in the Arduino IDE or PlatformIO.
3. **Install required libraries**:
	 - Bounce2
	 - UnicViewAD
	 - ESP32 board support
4. **Configure WiFi credentials** in `credentials.h` (default: SSID `SMART_TUNNEL`, password `smartx12345`).
5. **Connect hardware** according to the pinout above.
6. **Build and upload** the firmware to your ESP32 board.

## Usage

- The device will start as a WiFi Access Point with the configured SSID and password.
- Access the web API for status and control (see network module for available endpoints).
- The display provides real-time feedback and control options.
- Emergency stop and sensor events are handled automatically by the controller.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

