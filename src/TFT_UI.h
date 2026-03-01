#ifndef TFT_UI_H
#define TFT_UI_H

#include <Arduino.h>
#include "TFT35_V1_F407.h"

// --- THE COLOR DICTIONARY ---
#define TFT_BLACK       0x0000
#define TFT_WHITE       0xFFFF
#define TFT_RED         0xF800
#define TFT_GREEN       0x07E0
#define TFT_BLUE        0x001F
#define TFT_CYAN        0x07FF
#define TFT_MAGENTA     0xF81F
#define TFT_YELLOW      0xFFE0
#define TFT_ORANGE      0xFDA0
#define TFT_DARKGREY    0x7BEF
#define TFT_LIGHTGREY   0xC618

typedef void (*ButtonCallback)();

class TFT_UI {
public:
    TFT_UI(TFT35_V1_F407* tft);
    
    uint16_t rgb565(uint32_t hex24);

    // Graphic Primitives
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    
    // Text Engine
    void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
    void drawString(uint16_t x, uint16_t y, const char* text, uint16_t color, uint16_t bg, uint8_t size);

private:
    TFT35_V1_F407* _tft;
};

// --- STANDARD BUTTON ---
class TFT_Button {
public:
    TFT_Button();
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
            uint16_t color, uint16_t textColor, const char* label, 
            uint8_t textSize, ButtonCallback callback);
    void draw();
    bool checkTouch(uint16_t touchX, uint16_t touchY);
    void setColor(uint16_t color);
    void setLabel(const char* label);
private:
    TFT_UI* _ui;
    uint16_t _x, _y, _w, _h;
    uint16_t _color, _textColor;
    const char* _label;
    uint8_t _textSize;
    ButtonCallback _action;
    unsigned long _lastPressTime;
};

// --- IMAGE BUTTON (SD CARD GRAPHICS) ---
class TFT_ImageButton {
public:
    TFT_ImageButton();
    
    //We pass the raw hardware driver (tft) so it can access the SD card
    void init(TFT35_V1_F407* tft, TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
              const char* filename, ButtonCallback callback);
              
    void draw();
    bool checkTouch(uint16_t touchX, uint16_t touchY);

private:
    TFT35_V1_F407* _tft;
    TFT_UI* _ui;
    uint16_t _x, _y, _w, _h;
    const char* _filename;
    ButtonCallback _action;
    unsigned long _lastPressTime;
};

// --- TOGGLE SWITCH ---
class TFT_Toggle {
public:
    TFT_Toggle();
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
            uint16_t colorOn, uint16_t colorOff, uint16_t textColor, 
            const char* labelOn, const char* labelOff, 
            uint8_t textSize, ButtonCallback callback);
    void draw();
    bool checkTouch(uint16_t touchX, uint16_t touchY);
    bool getState();
    void setState(bool state);
private:
    TFT_UI* _ui;
    uint16_t _x, _y, _w, _h;
    uint16_t _colorOn, _colorOff, _textColor;
    const char* _labelOn;
    const char* _labelOff;
    uint8_t _textSize;
    ButtonCallback _action;
    unsigned long _lastPressTime;
    bool _state; 
};

// --- INTERACTIVE PROGRESS BAR / SLIDER ---
class TFT_ProgressBar {
public:
    TFT_ProgressBar();
    
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
              uint16_t bgColor, uint16_t barColor, float minVal, float maxVal, ButtonCallback callback = nullptr);
    void draw();
    void setValue(float val);
    float getValue();
    
    bool checkTouch(uint16_t touchX, uint16_t touchY);

private:
    TFT_UI* _ui;
    uint16_t _x, _y, _w, _h;
    uint16_t _bgColor, _barColor;
    float _minVal, _maxVal, _currentVal;
    ButtonCallback _action; 
    unsigned long _lastTouchTime; 
};

extern const uint8_t font5x7[]; 

#endif