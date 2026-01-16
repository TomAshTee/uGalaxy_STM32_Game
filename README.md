# uGalaxy – STM32 Game

A ‘Space Shooter’ arcade game written for an **STM32L4** microcontroller with a **128×128 OLED** display based on the **SSD1327** controller.  
The first concept of the game dates back to 2014, when an early version was created for a 128×64 OLED and SSD1306 controller.

---

## Why this project stands out

uGalaxy is not just another “blinking LED” example – it’s a fully playable, feature-rich arcade shooter designed specifically for embedded hardware.  
It demonstrates how much can be achieved on a **low-power STM32** with limited RAM and Flash using efficient C code and carefully designed game logic.

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

---

## Technologies Used

- **Microcontroller:** STM32L432KC (Cortex-M4, 80 MHz, 256 KB Flash, 64 KB RAM)
- **Display:** Waveshare 128×128 OLED, SSD1327 controller
- **Language:** C (C99)
- **Framework:** STM32CubeIDE + STM32 HAL
- **Graphics Library:** Custom SSD1327 GFX layer
- **Peripherals:**
  - SPI – OLED communication
  - ADC – analog joystick
  - GPIO – buttons

---

## What this project demonstrates

- Advanced embedded programming techniques
- Efficient 2D rendering on resource-constrained devices
- Applying game development concepts to microcontroller platforms
- Clean, maintainable, modular C code suitable for learning and further extension

---

## Roadmap

* ✅ Port to STM32 HAL libraries
* ✅ SSD1327 OLED controller support
* ✅ Custom PCB for handheld console
* 📖 Expand the game with story elements
* ✅ Translate code comments into English
* ✨ Add new gameplay features
* 🔌 Document hardware connections

---

## Features

- 10 levels with increasing difficulty
- Boss battles
- Animated starfield background
- Bonuses – extra lives and guided missiles

---

## Media

## 🎥 Full project overview 

The video below shows:
- manufactured PCB,
- assembled handheld device,
- real gameplay running on STM32

 <video width="640" controls>
  <source src="https://github.com/user-attachments/assets/75927860-7996-492d-9544-00e155972743/IMG_0586.mp4" type="video/mp4">
  Your browser does not support the video tag.
</source>
</video>

---

## 🧩 PCB Design

### 3D PCB render (components & layout)

The image below shows a 3D visualization of the custom PCB, including component placement and overall board layout.

![PCB 3D Render](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/PCB_3D.png)

---

### PCB routing – copper traces

The following image presents the PCB routing view, showing signal paths, power distribution, and overall board topology.

![PCB Routing](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/PCB.png)



---

## 🎮 Gameplay samples

### Gameplay – tracking missiles
![Gameplay Screenshot](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/IMG_3148.gif)

### Gameplay – end of game
![Gameplay Screenshot](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/IMG_3148%20(1).gif)
