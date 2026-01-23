# uGalaxy – STM32 Arcade Game

uGalaxy is a small arcade **space shooter game** written in **C** for an **STM32L4** microcontroller and a **128×128 OLED display (SSD1327)**.

The very first version of this game was created around **2014** as a student project, running on a 128×64 OLED with an SSD1306 controller.  
Over the years, the project has been gradually rewritten, cleaned up, and expanded.  
Today, uGalaxy serves both as a **playable handheld game** and a **learning playground for embedded systems programming**.

This repository shows the current, actively improved version of the project.

---

## What this project is about

This is not a demo or proof-of-concept.  
uGalaxy is a complete game loop running on a resource-constrained microcontroller:

- player movement and shooting
- enemies and bosses with their own behavior
- simple animations and effects
- real hardware input (buttons + analog joystick)
- real hardware rendering on a grayscale OLED

The main goal of the project is to explore **clean C architecture**, **embedded graphics**, and **timing-sensitive logic** on bare metal hardware, while still keeping things fun.

---

## Current state of the project

The project is **fully playable** and continuously refined.  
Recent work focused mostly on **code structure, readability, and correctness**, not only on adding features.

What is already done:

- ✅ Ported to **STM32 HAL** (CubeIDE project)
- ✅ Stable **SSD1327 OLED driver** with custom GFX layer
- ✅ Double-buffered rendering with DMA SPI
- ✅ Clear separation of modules:
  - game logic
  - graphics
  - input handling
  - application control
- ✅ Multiple levels with increasing difficulty
- ✅ Boss fights with individual movement patterns
- ✅ Power-ups (extra lives, guided missiles)
- ✅ Custom **handheld PCB** designed and manufactured
- ✅ Code comments translated and cleaned up
- ✅ README, repository structure, and commits improved based on community feedback

The project is still evolving, but it is already in a solid, understandable state.

---

## Hardware & tools

- **Microcontroller:** STM32L432KC (Cortex-M4, 80 MHz, 256 KB Flash, 64 KB RAM)
- **Display:** 128×128 OLED (SSD1327, grayscale)
- **Language:** C (C99)
- **IDE / Framework:** STM32CubeIDE + STM32 HAL
- **Peripherals used:**
  - SPI (OLED communication)
  - DMA (display refresh)
  - ADC (analog joystick)
  - GPIO (buttons)

---

## Media

### 🎥 Full project overview

The video below shows:
- manufactured PCB,
- assembled handheld device,
- real gameplay running on STM32.

https://github.com/user-attachments/assets/c8ee87c2-e7b8-4f8f-901f-ed7810f95134

---

## 🧩 PCB Design

### 3D PCB render

![PCB 3D Render](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/PCB_3D.png)

### PCB routing – copper traces

![PCB Routing](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/PCB.png)

---

## 🎮 Gameplay samples

### Tracking missiles

![Gameplay Screenshot](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/IMG_3148.gif)

### End of game

![Gameplay Screenshot](https://raw.githubusercontent.com/TomAshTee/uGalaxy_STM32_Game/main/IMG_3148%20(1).gif)

---

## Credits & inspiration

This project would not look the same without learning from others and community remembered feedback.

- **Atnel - mirekk36 – YouTube channel**  
  A big source of practical embedded knowledge, and low-level programming mindset:  
  https://www.youtube.com/@mirekk36

- **Graphics & bitmap tools**  
  Used while preparing sprites and graphics for the OLED display:
  - https://bmp2array.com/
  - https://dot2pic.com/

- **Reddit embedded community**  
  Thanks to everyone who shared feedback and suggestions in this thread:  
  https://www.reddit.com/r/embedded/comments/1qet23z/stm32_baremetal_game_project_looking_for/

- **Adafruit GFX Library**  
  Parts of the custom GFX layer were inspired by design ideas and concepts from the Adafruit GFX Library  
  (API style, basic drawing primitives, overall approach to graphics abstraction):  
  https://github.com/adafruit/Adafruit-GFX-Library  
   
  The code itself was adapted and rewritten to fit the SSD1327 controller, grayscale rendering,  
  and STM32-specific constraints.

---

## Final note

uGalaxy is a hobby project, but it is treated seriously.  
The focus is not only on *making it work*, but on understanding **why it works**, and how to write embedded code that is readable, testable, and fun to improve over time.
