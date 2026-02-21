#ifndef TFT35_V1_F407_H
#define TFT35_V1_F407_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>

// Hardware Addresses
#define LCD_REG (*((volatile uint16_t *) 0x60000000))
#define LCD_DAT (*((volatile uint16_t *) 0x61000000))

// Pin Definitions
#define L_BL     PD12 
#define L_BL_ALT PD13 
#define L_RST    PC6  
#define SD_CS    PA4

// Video Constraints
#define WIDTH 480
#define HEIGHT 320
#define LINES_PER_BLOCK 16
#define PIXELS_PER_BLOCK (WIDTH * LINES_PER_BLOCK)

class TFT35_V1_F407 {
public:
    TFT35_V1_F407();

    void begin();
    void setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void fillScreen(uint16_t color);

    bool initSD();
    void playVideo(const char* filename);

private:
    void initFSMC();
    void initLCD();
    void sendBufferDMA(uint16_t* source, uint32_t len);
    bool isDMAReady();

    SdFat sd;
    File animFile;
    uint16_t framePalette[256];
    uint8_t  indexedBuffer[PIXELS_PER_BLOCK];
    uint16_t colorBuffer[2][PIXELS_PER_BLOCK];
};

#endif