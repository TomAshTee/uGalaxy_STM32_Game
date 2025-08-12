# uGalaxy – STM32 Game

A ‘Space Shooter’ arcade game written for an **STM32L4** microcontroller with a **128×128 OLED** display based on the **SSD1327** controller.  
The first concept of the game dates back to 2014, when an early version was created for a 128×64 OLED and SSD1306 controller.

---

## Why this project stands out

uGalaxy is not just another “blinking LED” example – it’s a fully playable, feature-rich arcade shooter designed specifically for embedded hardware.  
It shows how much can be achieved on a **low-power STM32** with limited RAM and Flash, using efficient C code and carefully designed game logic.

### Key Highlights
- Runs on bare-metal STM32 with STM32 HAL (no OS required)  
- Optimized for performance and memory – smooth gameplay on an 80 MHz Cortex-M4 with only 64 KB RAM  
- Full-featured game loop including:
  - 10 progressively challenging levels
  - Boss battles with unique movement and attack patterns
  - Animated background with dynamic starfield
  - Power-ups: extra lives & guided missiles
- SSD1327 OLED driver integration – custom graphics routines for grayscale rendering  
- Modular code structure with separate layers for:
  - Game logic
  - Graphics rendering
  - Hardware abstraction (input, display, ADC joystick)

### Technologies Used
- **Microcontroller:** STM32L432KC (Cortex-M4, 80 MHz, 256 KB Flash, 64 KB RAM)  
- **Display:** Waveshare 128×128 OLED, SSD1327 controller  
- **Language:** C (C99)  
- **Framework:** STM32CubeIDE + STM32 HAL  
- **Graphics Library:** Custom SSD1327 GFX layer (bitmap, shape, and text drawing functions)  
- **Peripherals:**
  - SPI for display communication
  - ADC for analog joystick input
  - GPIO for button controls

### What this project demonstrates
- Advanced embedded programming techniques  
- Efficient 2D rendering on resource-constrained devices  
- Application of game development principles to microcontroller platforms  
- Clean, maintainable C codebase suitable for educational and hobby use

---

## Tech Stack

**Hardware:** STM32L432KC Nucleo, Waveshare OLED 128×128 SSD1327  
**Software:** STM32CubeIDE  
**Language:** C  

---

## Roadmap

* ✅ Port to STM32 HAL libraries  
* ✅ SSD1327 OLED controller support  
* 📖 Expand the game with story elements  
* 🛠️ Design a PCB for a handheld console version  
* 🌍 Translate code comments into English  
* ✨ Add new gameplay features  
* 🔌 Document hardware connection layout  

---

## Features

- 10 levels with increasing difficulty  
- Boss battles  
- Animated background – starfield effect  
- Bonuses – extra lives and guided missiles  

---

## Media

**Gameplay – tracking missiles**  
![App Screenshot](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/IMG_3148.gif)

**Gameplay – end of game**  
![App Screenshot](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/IMG_3148%20(1).gif)
