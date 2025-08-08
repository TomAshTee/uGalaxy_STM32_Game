# uGalaxy - STM32 Game

A simple 'Space Shooter' game written for an STM32L4 microcontroller with a 128×128 OLED display based on the SSD1327 controller.  
The idea for the game came while I was still at university in 2014, at a time when the first version was developed for a 128×64 OLED screen and SSD1306 controller.  

---

## Why this project stands out 🚀

uGalaxy is not just another microcontroller demo – it’s a fully playable, feature-rich arcade shooter built from the ground up for embedded hardware.  
It demonstrates how much can be achieved on a **low-power STM32 microcontroller** with limited RAM and Flash, when paired with efficient C code and smart game logic.

### Key Highlights
- **Runs on bare-metal STM32** with STM32 HAL (no OS required)  
- **Optimized for performance and memory** – smooth gameplay on an 80 MHz Cortex-M4 with only 64 KB RAM  
- **Full-featured game loop** including:
  - 10 progressively challenging levels
  - Boss battles with unique movement and attack patterns
  - Background animation with dynamic starfield
  - Power-ups: extra lives & guided missiles  
- **SSD1327 OLED driver integration** – custom graphics routines for grayscale rendering  
- **Modular code structure** with separate layers for:
  - Game logic
  - Graphics rendering
  - Hardware abstraction (input, display, ADC joystick)

### Technologies Used
- **Microcontroller:** STM32L432KC (Cortex-M4, 80 MHz, 256 KB Flash, 64 KB RAM)
- **Display:** Waveshare 128×128 OLED, SSD1327 controller
- **Language:** C (C99 standard)
- **Framework:** STM32CubeIDE + STM32 HAL
- **Graphics Library:** Custom SSD1327 GFX layer with bitmap, shape, and text drawing functions
- **Peripherals:**
  - SPI for display communication
  - ADC for analog joystick input
  - GPIO for button controls

### What this project demonstrates
- Advanced embedded programming techniques  
- Efficient 2D rendering on resource-constrained devices  
- Game development principles applied to microcontroller platforms  
- Clear, maintainable C codebase suitable for educational and hobby projects

---

## Tech Stack

**Hardware:** STM32L432KC Nucleo, Waveshare OLED 128×128 SSD1327  
**Software:** STM32CubeIDE  
**Programming language:** C  

---

## Roadmap

* [X] Adaptation to STM32 HAL libraries
* [X] Adapting the library to the SSD1327 OLED controller
* [ ] Expanding the game with story inserts
* [ ] Designing a PCB as a console
* [ ] Translation of comments into English
* [ ] Adding new functionalities to the game
* [ ] Layout of hardware component connections

---

## Features

- 10 levels in the game
- Boss battles
- Background animation – star effect
- Bonuses – extra lives, guided missiles

---

## GIF from the game 

#### Gameplay – tracking missiles  
![App Screenshot](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/IMG_3148.gif)

#### Gameplay – end of game  
![App Screenshot](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/IMG_3148%20(1).gif)
