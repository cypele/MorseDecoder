# MorseDecoder

Decode Morse code transmitted via light using the KL05Z microcontroller, and display the decoded text on an LCD.

---

## Table of Contents
1. [Features](#features)
2. [Demo Video](#demo-video)
3. [Hardware Setup](#hardware-setup)
4. [User Instructions](#user-instructions)
5. [Connection Diagram](#connection-diagram)
6. [Build & Flash](#build--flash)
7. [About](#about)

---

## Features
- **Light-based decoding**: Interpret Morse signals sent by a flashlight using an ALS-PT19 light sensor.
- **Dynamic display**: Show dots, dashes, letters, and full messages on an LCD1602A.
- **Error feedback**: Invalid letters prompt a “Wrong letter!” message.
- **Message recall**: Send five dots (`.....`) to display the full decoded message history.
- **Light timing thresholds**: Customize dot/dash lengths in firmware.

---

## Demo Video
Watch MorseDecoder in action:  
[![](https://img.youtube.com/vi/loDQmXkXVRg/0.jpg)](https://youtube.com/shorts/loDQmXkXVRg?feature=share)

---

## Hardware Setup
1. **Microcontroller**: FRDM-KL05Z (ARM Cortex-M0+)
2. **Light Sensor**: ALS-PT19
3. **Display**: LCD1602A (I2C interface)
4. **Connections**:
   - **LCD1602A**
     - `PTB3` → `SCL`
     - `PTB4` → `SDA`
     - `+5V` → `VCC`
     - `GND` → `GND`
   - **ALS-PT19**
     - `3.3V` → `+`
     - `GND` → `-`
     - `PTB11` → `OUT`

---

## User Instructions
1. Power on the board: LCD shows welcome message, then clears.
2. Shine a flashlight at the sensor:
   - **Dot**: Light on **< 0.1s**
   - **Dash**: Light on **0.1–0.2s**
3. Long light (> 0.2s) adds a space in the current word.
4. Send `.....` (five dots) to view and scroll through the full decoded message.
5. Invalid sequences display “Wrong letter!”.

---

## Connection Diagram
```plaintext
FRDM-KL05Z       Component        Pins
-------------    -------------    ----------------
PTB3             LCD1602A SCL     SCL
PTB4             LCD1602A SDA     SDA
+5V              LCD1602A VCC     +5V
GND              LCD1602A GND     GND
3.3V             ALS-PT19 +       VCC
GND              ALS-PT19 -       GND
PTB11            ALS-PT19 OUT     Signal In
````

---

## Build & Flash

1. Open the project in **Keil uVision5**.
2. Ensure the target is **FRDM-KL05Z** and compiler is **Arm Compiler v6**.
3. Build the project (`Rebuild All`).
4. Connect the board via OpenSDA and flash.
5. Reset the board to start decoding.

---

## About

This firmware runs on NXP’s KL05Z MCU, sampling a light sensor to detect Morse timing. Display logic uses a 16×2 I2C LCD. Developed in Keil uVision5 with Arm CC6.

---

© 2025 cypele — licensed under MIT

```
```
