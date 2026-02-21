# TFT35_V1_F407 High-Performance Multimedia Driver

A high-performance hardware driver that transforms generic **TFT35 V1.0** screens (often sold as 3D printer smart displays) into standalone, high-speed multimedia development boards.

These boards feature a powerful onboard **STM32F407VET6** running at 168MHz, but their screens are often bottlenecked by standard, slow display libraries. This library completely bypasses traditional bit-banging and SPI routines by utilizing the STM32's **Flexible Static Memory Controller (FSMC)** and **Direct Memory Access (DMA)**. This allows you to push pixels to the screen with virtually zero CPU overhead.

## 🚀 Key Features
* **True FSMC Memory Mapping:** Accesses the LCD directly at hardware memory registers (`0x60000000` and `0x61000000`).
* **Zero-Wait DMA Double-Buffering:** Asynchronous DMA streams blast 16-line pixel blocks to the screen while the CPU fetches the next data block.
* **Integrated Video Player API:** Play video directly from an SD Card using a single command.
* **Complete Multimedia Toolkit:** Includes Python tools to quantize standard `.mp4` videos into an optimized 8-bit indexed `.bin` format built specifically for microcontroller RAM limits.

## 🎯 Project Inspiration
Because this setup requires almost no CPU time to run the display, it is perfect for embedded projects that need looping animations alongside sensor processing.

## 🔌 Hardware Setup & Pinout

### ST-Link Programming Connection
To flash new firmware to the onboard STM32F407VET6, you will need to solder standard header pins to the SWD debug pads located on the board. Connect an **ST-Link V2** programmer to these pins (`3.3V`, `GND`, `SWDIO`, `SWCLK`) to upload your code via the Arduino IDE.

![ST-Link Soldering Example](insert_your_image_link_here.jpg)

### Internal Pin Routing
The library is hardcoded to the internal routing of the standard TFT35 V1.0 board:
* **LCD Backlight:** `PD12` (Backup: `PD13`)
* **LCD Reset:** `PC6`
* **SD Card CS:** `PA4`
* *(The FSMC Data/Command pins are automatically routed to `GPIOD` and `GPIOE` alternate functions internally)*.

---

## 💻 Installation

1. Download this repository as a `.zip` file.
2. In the Arduino IDE, go to **Sketch** -> **Include Library** -> **Add .ZIP Library** and select the downloaded file.
3. Install the **SdFat** library via the Arduino Library Manager (required for high-speed SD access).
4. Access the examples under **File** -> **Examples** -> **TFT35_V1_F407**.

### Quick Start: Playing a Video

~~~cpp
#include <TFT35_V1_F407.h>

TFT35_V1_F407 tft;

void setup() {
    tft.begin();
    
    // Initialize the SD card over 50MHz SPI
    if (!tft.initSD()) {
        while(1); // Halt if SD fails
    }
}

void loop() {
    // Play a converted video file from the SD card
    tft.playVideo("anim_lite.bin"); 
}
~~~

---

## 🛠️ Python Tools (Video & Image Conversion)

To play video or display static images, you must first convert them into our optimized formats using the included Python scripts located in the `/tools` folder.

First, install the required dependencies:
~~~bash
pip install -r tools/requirements.txt
~~~

### 1. Converting Video (`video_converter.py`)
This script uses K-Means clustering to reduce standard video down to an optimized 256-color (8-bit) indexed palette. This slashes the file size by 50%, allowing the STM32 to process it in real-time.

~~~bash
python tools/video_converter.py my_video.mp4 -o anim_lite.bin --fps 15
~~~
*Note: The script automatically resizes the video to 480x320 to fit the screen.*

### 2. Previewing the Output (`bin_preview.py`)
Want to see what your color-quantized video looks like before putting it on the SD Card? Run the previewer on your PC:

~~~bash
python tools/bin_preview.py anim_lite.bin
~~~

### 3. Converting Static Images (`image_converter.py`)
Converts standard images (PNG, JPG) to a raw 16-bit RGB565 format for instant loading on the display.

~~~bash
python tools/image_converter.py splash.png -o image.raw
~~~

---

## ⚙️ The "15 FPS" Reality
While the FSMC engine can drive the screen well over 60 FPS, standard SD cards operating in SPI mode have a physical read-speed bottleneck of around 2.0 - 2.5 MB/s. 

To play full-screen (480x320) video, the STM32 must constantly pull massive amounts of data from the SD card. Thanks to our custom 8-bit quantization format, this library achieves a rock-solid **~15 FPS video playback**—the absolute mathematical maximum for this hardware over SPI. This framerate is highly stable and perfect for tech UI animations, pixel art, anime loops, and status dashboards.
