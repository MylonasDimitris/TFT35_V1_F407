#ifndef TFT_UI_H
#define TFT_UI_H

#include <Arduino.h>
#include "TFT35_V1_F407.h"

typedef void (*ButtonCallback)();

class TFT_UI {
public:
    TFT_UI(TFT35_V1_F407* tft);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
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
              uint16_t color, uint16_t textColor, const char* label, ButtonCallback callback);
    void draw();
    bool checkTouch(uint16_t touchX, uint16_t touchY);
    
    // NEW: Dynamic modifiers!
    void setColor(uint16_t color);
    void setLabel(const char* label);

private:
    TFT_UI* _ui;
    uint16_t _x, _y, _w, _h;
    uint16_t _color, _textColor;
    const char* _label;
    ButtonCallback _action;
    unsigned long _lastPressTime;
};

// --- TOGGLE SWITCH ---
class TFT_Toggle {
public:
    TFT_Toggle();
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
              uint16_t colorOn, uint16_t colorOff, uint16_t textColor, 
              const char* labelOn, const char* labelOff, ButtonCallback callback);
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
    ButtonCallback _action;
    unsigned long _lastPressTime;
    bool _state; // True = ON, False = OFF
};

// --- PROGRESS BAR ---
class TFT_ProgressBar {
public:
    TFT_ProgressBar();
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
              uint16_t bgColor, uint16_t barColor, float minVal, float maxVal);
    void draw();
    void setValue(float val);

private:
    TFT_UI* _ui;
    uint16_t _x, _y, _w, _h;
    uint16_t _bgColor, _barColor;
    float _minVal, _maxVal, _currentVal;
};

extern const uint8_t font5x7[]; 

#endif