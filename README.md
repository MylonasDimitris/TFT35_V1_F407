# TFT35_V1_F407

**Keywords:** #STM32F407 #FSMC #TFT-LCD #Reverse-Engineering #HMI

![Status: WIP](https://img.shields.io/badge/Status-Work_In_Progress-orange?style=flat-square)
![License: MIT](https://img.shields.io/badge/License-MIT-blue?style=flat-square)

High-performance graphics and UI framework for a **closed-source TFT35 V1.0 touchscreen module** (onboard **STM32F407VET6**) recovered from older 3D printer hardware.

This project was developed through practical reverse engineering (board tracing + multimeter probing) and focuses on deterministic rendering, responsive touch input, and SD-based media playback.

## Hardware flashing / connection (ST-Link)

Program the board through the **back-side SWD pins** using an ST-Link.

Typical SWD wiring:
- `3V3`
- `GND`
- `SWDIO`
- `SWCLK`
- `RST` (recommended)

> Keep wires short and confirm logic voltage before powering the board.

![1000118710](https://github.com/user-attachments/assets/e4209efb-1dcd-424d-a97f-f68a86cb35ce)


## Video demo

![Video playback GIF placeholder](docs/gifs/video-playback-demo.gif)

## Hardware Pin Map (Reverse Engineered)

| Subsystem | Interface | Discovered Mapping |
| :-- | :-- | :-- |
| LCD | 16-bit FSMC | Reg: `0x60000000`, Data: `0x61000000` |
| Touch (XPT2046) | Bit-banged SPI | `SCK=PC6`, `CS=PC7`, `MOSI=PC8`, `MISO=PC9`, `IRQ=PA8` |
| SD Card | SPI1 | `CS=PA4` |
| Backlight | GPIO / PWM capable | `PD13` |

## Why this library is strong

- **Hardware Abstraction with direct-register throughput**
    - Clean class API on top of low-level FSMC/DMA/touch/SD control.
    - Optimized for the TFT35 V1.0 integrated module.

- **Asynchronous Pipelines for media rendering**
    - Palette decode and LCD transfer are organized as a double-buffered DMA workflow.
    - Supports indexed `.BIN` playback from SD with block-based frame processing.

- **Deterministic UI Layout and interaction model**
    - Shared widget contracts (`draw`, `checkTouch`, `setGeometry`) keep behavior predictable.
    - Grid-driven placement reduces manual pixel math in larger interfaces.

- **Production-oriented graphics primitives**
    - Fast rectangle fills, line/circle primitives, text renderer, and RGB565 conversion.

## Advanced Grid Layout System

`TFT_Grid` acts as a **Dynamic Layout Manager** for UI composition on fixed-resolution TFT screens.

- **Automatic row/column coordinate calculation**
    - Computes cell geometry from grid bounds, rows/cols, and padding.

- **Widget spanning (`colSpan`, `rowSpan`)**
    - A widget can occupy multiple cells horizontally/vertically.
    - Span bounds are validated before assignment.

- **Padding/margin style spacing management**
    - Inter-cell spacing is handled through grid padding.
    - Spanned dimensions include internal spacing for consistent alignment.

## Performance & Concurrency

- **Memory-Mapped I/O (FSMC)**
    - LCD writes are done through mapped register/data addresses, effectively treating the panel bus as high-speed external memory.

- **Double-Buffered DMA (DMA2)**
    - Color blocks are expanded into alternating RGB565 buffers and transferred with DMA, reducing CPU stall during frame output.

- **Bus Yield Strategy for touch responsiveness**
    - The touch stack uses controlled micro-delays in XPT2046 bit-banged transactions for stable sampling.
    - Video playback is processed in fixed line blocks (frame-loop counters via `LINES_PER_BLOCK`), enabling predictable scheduling points for touch polling between frame updates.

## Software Architecture

The UI is built around **polymorphism** through the `TFT_Widget` base class:

- `draw()`
- `checkTouch(uint16_t tx, uint16_t ty)`
- `setGeometry(uint16_t x, uint16_t y, uint16_t w, uint16_t h)`

This keeps the system extensible and allows different controls (`TFT_Button`, `TFT_Toggle`, `TFT_ProgressBar`, `TFT_ImageButton`) to be managed uniformly by containers such as `TFT_Grid`.

## Requirements

- TFT35 V1.0 touchscreen unit with onboard STM32F407VET6
- Arduino core for STM32
- [SdFat](https://github.com/greiman/SdFat)
- (Optional) Python 3 for media conversion tools

## Quick start

```cpp
#include <TFT35_V1_F407.h>
#include <TFT_UI.h>

TFT35_V1_F407 tft;
TFT_UI ui(&tft);
TFT_Button btn;

void onPress() {
        // Called when the button receives a valid touch event
        Serial.println("Pressed");
}

void setup() {
        // Serial output for debug/logging
        Serial.begin(115200);

        // Initialize LCD/FSMC backend
        tft.begin();

        // Initialize touch controller (XPT2046)
        tft.initTouch();

        // Build and render one basic button widget
        btn.init(&ui, 40, 40, 140, 50, TFT_BLUE, TFT_WHITE, "START", 2, onPress);
        btn.draw();
}

void loop() {
        uint16_t x, y;

        // Poll touch and dispatch to widget logic
        if (tft.getTouchCoordinates(&x, &y)) {
                btn.checkTouch(x, y);
        }
}
```

## Media tools

Use scripts in `tools/` to prepare SD-ready assets.

When to use each tool:

- `image_converter.py`
    - Convert PNG/JPG into `.RAW` RGB565 for fast static UI assets.

- `video_converter.py`
    - Convert videos into indexed `.BIN` streams for `openVideo()` + `playFrame()`.

- `bin_preview.py`
    - Preview `.BIN` output on desktop before copying to SD.

Typical workflow:
1. Convert source media on your PC.
2. Copy generated files to the SD card.
3. Call `tft.initSD()` in `setup()`.
4. Render using `drawRAW()` / `drawBMP()` or play via `openVideo()` + `playFrame()`.

Commands:

```bash
# Convert image to RGB565 RAW
python tools/image_converter.py input.png -o ICON.RAW --size 100 100

# Convert video to indexed BIN stream
python tools/video_converter.py input.mp4 -o TEST.BIN --fps 15

# Optional desktop preview
python tools/bin_preview.py TEST.BIN
```

Video playback snippet (minimal):

```cpp
#include <TFT35_V1_F407.h>

TFT35_V1_F407 tft;

void setup() {
        tft.begin();
        tft.initSD();

        // Open SD video file (8.3 filename format)
        tft.openVideo("TEST.BIN");
}

void loop() {
        // Push one frame per call; reopen when stream ends
        if (!tft.playFrame()) {
                tft.openVideo("TEST.BIN");
        }
}
```

## Project layout

- `src/` -> library source
- `tools/` -> Python conversion utilities

## Notes

This project is still **Work In Progress**, but the core hardware driver, graphics stack, touch input, and UI system are functional.
