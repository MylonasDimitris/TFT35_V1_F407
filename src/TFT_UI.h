#ifndef TFT_UI_H
#define TFT_UI_H

#include <Arduino.h>
#include "TFT35_V1_F407.h"

// Define the Callback Function type for buttons
typedef void (*ButtonCallback)();

// --- THE GRAPHICS & TEXT ENGINE ---
class TFT_UI {
public:
    TFT_UI(TFT35_V1_F407* tft);

    // Graphic Primitives
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    
    // Text Engine
    void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
    void drawString(uint16_t x, uint16_t y, const char* text, uint16_t color, uint16_t bg, uint8_t size);

private:
    TFT35_V1_F407* _tft;
};

// --- THE BUTTON CLASS ---
class TFT_Button {
public:
    TFT_Button();
    
    // Initialize with size, colors, text label, and function trigger!
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
              uint16_t color, uint16_t textColor, const char* label, ButtonCallback callback);
    
    void draw();
    bool checkTouch(uint16_t touchX, uint16_t touchY);

private:
    TFT_UI* _ui;
    uint16_t _x, _y, _w, _h;
    uint16_t _color, _textColor;
    const char* _label;
    ButtonCallback _action;
    unsigned long _lastPressTime;
};

// --- COMPACT 5x7 ASCII FONT ARRAY ---
// (Characters 32 to 127)
extern const uint8_t font5x7[]; 

#endif