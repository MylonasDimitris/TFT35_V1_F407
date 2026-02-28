# TFT35_V1_F407 UI & Graphics Library

![Status: WIP](https://img.shields.io/badge/Status-Work_In_Progress-orange?style=flat-square)
![License: MIT](https://img.shields.io/badge/License-MIT-blue?style=flat-square)

> **Note:** This library is currently a **Work In Progress**. The core hardware drivers (FSMC/DMA) and UI widgets are functional, but documentation and additional examples are still being added.

A high-performance graphics and UI framework for STM32F407VET6 microcontrollers driving 3.5" TFT displays (ILI9486/ILI9488). The library utilizes hardware-level optimizations including **FSMC (Flexible Static Memory Controller)** and **DMA (Direct Memory Access)** to achieve high-speed rendering and fluid video playback.

---

## Technical Architecture

### Hardware Interfacing (FSMC)
This library treats the display as an external memory bank. By using the STM32 FSMC, the LCD is mapped directly to the CPU's address space. This bypasses slow GPIO toggling, allowing the hardware to handle the timing signals for the 16-bit parallel bus.
* **Register Address:** `0x60000000` (RS Pin Low)
* **Data Address:** `0x61000000` (RS Pin High)



### Memory Management & DMA
For video and heavy graphical updates, the library implements a dual-buffered DMA2 pipeline. This allows the CPU to decode frame data into one buffer while the DMA controller simultaneously pushes the second buffer to the display, eliminating screen tearing and reducing CPU load during playback.

---

## Features

* **Graphics Engine:** Optimized implementations of Bresenham's line and circle algorithms.
* **UI Component System:** * **TFT_Button & TFT_Toggle:** Interactive widgets with customizable colors, labels, and font scaling.
    * **TFT_ProgressBar:** Smooth-drag slider with coordinate-to-value mapping and input debouncing.
    * **TFT_ImageButton:** Support for standard 24-bit `.BMP` and high-speed custom 16-bit `.RAW` assets.
* **Media Decoders:** * **RAW Decoder:** Direct file-to-FSMC streaming for high-speed UI assets.
    * **Video Player:** 8-bit indexed color playback with dynamic palette updates and sync-marker verification.
* **Touch Interface:** Calibrated touch driver with noise-filtering and "Lift-off Drift" protection to ensure clean input data.

---

## Wiring Configuration

| Component | TFT Pin | STM32 Pin | Function |
| :--- | :--- | :--- | :--- |
| **LCD Data** | D0 - D15 | PD/PE (various) | 16-bit Parallel Bus |
| **Control** | RS / WR / RD | PD11 / PD5 / PD4 | FSMC Control Lines |
| **Touch** | CS / SCK / DIN / DOUT | PC7 / PC6 / PC8 / PC9 | Bit-banged SPI |
| **Touch IRQ** | IRQ | PA8 | External Interrupt |
| **SD Card** | CS | PA4 | SPI Chip Select |



---

## Setup & Usage

### 1. Requirements
* **Hardware:** STM32F407VET6 Development Board.
* **Libraries:** [SdFat](https://github.com/greiman/SdFat) (required for high-speed SD access).
* **Python:** 3.x with `OpenCV`, `NumPy`, and `Pillow` for media conversion.

### 2. Media Preparation
Use the included Python scripts in the `/extras` directory to prepare assets for the SD card:

```bash
# Convert image to optimized RAW format
python image_converter.py input.png -o ICON.RAW --size 100 100

# Convert video to DMA-ready binary format
python video_converter.py input.mp4 -o TEST.BIN --fps 15
```
### 3. Example Implementation

```cpp
#include <TFT35_V1_F407.h>
#include <TFT_UI.h>

TFT35_V1_F407 tft;
TFT_UI ui(&tft);
TFT_Button myButton;

// 1. Define the callback function
void handleStartPress() {
    Serial.println("Button Pressed: START");
}

void setup() {
    Serial.begin(115200); // Initialize Serial communication
    
    tft.begin();
    tft.initTouch();
    tft.initSD();

    // 2. Pass 'handleStartPress' instead of 'nullptr'
    myButton.init(&ui, 50, 50, 160, 60, TFT_BLUE, TFT_WHITE, "START", 2, handleStartPress);
    myButton.draw();
}

void loop() {
    uint16_t x, y;
    if (tft.getTouchCoordinates(&x, &y)) {
        // 3. checkTouch will now trigger handleStartPress() automatically
        myButton.checkTouch(x, y);
    }
}
```
## Directory Structure

* **/src**: Library source code (`.h` and `.cpp`).
* **/examples**: Pre-configured `.ino` sketches for basic setup, video playback, and UI testing.
* **/tools**: Python-based media conversion toolchain.
