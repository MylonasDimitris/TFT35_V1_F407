#include "TFT_UI.h"

// The Standard 5x7 ASCII Font 
const uint8_t font5x7[] = {
    0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x5F,0x00,0x00, 0x00,0x07,0x00,0x07,0x00, 0x14,0x7F,0x14,0x7F,0x14,
    0x24,0x2A,0x7F,0x2A,0x12, 0x23,0x13,0x08,0x64,0x62, 0x36,0x49,0x55,0x22,0x50, 0x00,0x05,0x03,0x00,0x00,
    0x00,0x1C,0x22,0x41,0x00, 0x00,0x41,0x22,0x1C,0x00, 0x08,0x2A,0x1C,0x2A,0x08, 0x08,0x08,0x3E,0x08,0x08,
    0x00,0x50,0x30,0x00,0x00, 0x08,0x08,0x08,0x08,0x08, 0x00,0x60,0x60,0x00,0x00, 0x20,0x10,0x08,0x04,0x02,
    0x3E,0x51,0x49,0x45,0x3E, 0x00,0x42,0x7F,0x40,0x00, 0x42,0x61,0x51,0x49,0x46, 0x21,0x41,0x45,0x4B,0x31,
    0x18,0x14,0x12,0x7F,0x10, 0x27,0x45,0x45,0x45,0x39, 0x3C,0x4A,0x49,0x49,0x30, 0x01,0x71,0x09,0x05,0x03,
    0x36,0x49,0x49,0x49,0x36, 0x06,0x49,0x49,0x29,0x1E, 0x00,0x36,0x36,0x00,0x00, 0x00,0x56,0x36,0x00,0x00,
    0x00,0x08,0x14,0x22,0x41, 0x14,0x14,0x14,0x14,0x14, 0x41,0x22,0x14,0x08,0x00, 0x02,0x01,0x51,0x09,0x06,
    0x32,0x49,0x79,0x41,0x3E, 0x7E,0x11,0x11,0x11,0x7E, 0x7F,0x49,0x49,0x49,0x36, 0x3E,0x41,0x41,0x41,0x22,
    0x7F,0x41,0x41,0x22,0x1C, 0x7F,0x49,0x49,0x49,0x41, 0x7F,0x09,0x09,0x01,0x01, 0x3E,0x41,0x41,0x51,0x32,
    0x7F,0x08,0x08,0x08,0x7F, 0x00,0x41,0x7F,0x41,0x00, 0x20,0x40,0x41,0x3F,0x01, 0x7F,0x08,0x14,0x22,0x41,
    0x7F,0x40,0x40,0x40,0x40, 0x7F,0x02,0x04,0x02,0x7F, 0x7F,0x04,0x08,0x10,0x7F, 0x3E,0x41,0x41,0x41,0x3E,
    0x7F,0x09,0x09,0x09,0x06, 0x3E,0x41,0x51,0x21,0x5E, 0x7F,0x09,0x19,0x29,0x46, 0x46,0x49,0x49,0x49,0x31,
    0x01,0x01,0x7F,0x01,0x01, 0x3F,0x40,0x40,0x40,0x3F, 0x1F,0x20,0x40,0x20,0x1F, 0x7F,0x20,0x18,0x20,0x7F,
    0x63,0x14,0x08,0x14,0x63, 0x03,0x04,0x78,0x04,0x03, 0x61,0x51,0x49,0x45,0x43, 0x00,0x7F,0x41,0x41,0x00,
    0x02,0x04,0x08,0x10,0x20, 0x00,0x41,0x41,0x7F,0x00, 0x04,0x02,0x01,0x02,0x04, 0x40,0x40,0x40,0x40,0x40,
    0x00,0x01,0x02,0x04,0x00, 0x20,0x54,0x54,0x54,0x78, 0x7F,0x48,0x44,0x44,0x38, 0x38,0x44,0x44,0x44,0x20,
    0x38,0x44,0x44,0x48,0x7F, 0x38,0x54,0x54,0x54,0x18, 0x08,0x7E,0x09,0x01,0x02, 0x08,0x14,0x54,0x54,0x3C,
    0x7F,0x08,0x04,0x04,0x78, 0x00,0x44,0x7D,0x40,0x00, 0x20,0x40,0x44,0x3D,0x00, 0x7F,0x10,0x28,0x44,0x00,
    0x00,0x41,0x7F,0x40,0x00, 0x7C,0x04,0x18,0x04,0x78, 0x7C,0x08,0x04,0x04,0x78, 0x38,0x44,0x44,0x44,0x38,
    0x7C,0x14,0x14,0x14,0x08, 0x08,0x14,0x14,0x18,0x7C, 0x7C,0x08,0x04,0x04,0x08, 0x48,0x54,0x54,0x54,0x20,
    0x04,0x3F,0x44,0x40,0x20, 0x3C,0x40,0x40,0x20,0x7C, 0x1C,0x20,0x40,0x20,0x1C, 0x3C,0x40,0x30,0x40,0x3C,
    0x44,0x28,0x10,0x28,0x44, 0x0C,0x50,0x50,0x50,0x3C, 0x44,0x64,0x54,0x4C,0x44, 0x00,0x08,0x36,0x41,0x00,
    0x00,0x00,0x7F,0x00,0x00, 0x00,0x41,0x36,0x08,0x00, 0x08,0x08,0x2A,0x1C,0x08
};

// ============================================================================
// UI ENGINE IMPLEMENTATION
// ============================================================================

TFT_UI::TFT_UI(TFT35_V1_F407* tft) { 
    _tft = tft; 
}

// --- COLOR MATH ---

uint16_t TFT_UI::rgb565(uint32_t hex24) {
    // Crushes a 24-bit web hex (0xRRGGBB) into a 16-bit RGB565 format
    uint8_t r = (hex24 >> 16) & 0xFF;
    uint8_t g = (hex24 >> 8) & 0xFF;
    uint8_t b = hex24 & 0xFF;
    
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// --- GEOMETRY PRIMITIVES ---

void TFT_UI::drawPixel(int16_t x, int16_t y, uint16_t color) {
    fillRect(x, y, 1, 1, color);
}

void TFT_UI::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (!_tft) return;
    
    // Bounds checking to prevent drawing outside the physical screen
    if (x >= 480 || y >= 320) return;
    if (x + w > 480) w = 480 - x;
    if (y + h > 320) h = 320 - y;

    _tft->setWindow(x, y, x + w - 1, y + h - 1); 
    
    uint32_t totalPixels = w * h;
    for(uint32_t i = 0; i < totalPixels; i++) {
        LCD_DAT = color;
    }
}

void TFT_UI::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    fillRect(x, y, w, 1, color);         // Top edge
    fillRect(x, y + h - 1, w, 1, color); // Bottom edge
    fillRect(x, y, 1, h, color);         // Left edge
    fillRect(x + w - 1, y, 1, h, color); // Right edge
}

void TFT_UI::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    // Standard Bresenham Line Drawing Algorithm
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    
    if (steep) {
        int16_t tmp = x0; x0 = y0; y0 = tmp;
        tmp = x1; x1 = y1; y1 = tmp;
    }
    
    if (x0 > x1) {
        int16_t tmp = x0; x0 = x1; x1 = tmp;
        tmp = y0; y0 = y1; y1 = tmp;
    }
    
    int16_t dx = x1 - x0;
    int16_t dy = abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;

    for (; x0 <= x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        
        err -= dy;
        if (err < 0) { 
            y0 += ystep; 
            err += dx; 
        }
    }
}

void TFT_UI::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    drawPixel(x0, y0 + r, color); 
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color); 
    drawPixel(x0 - r, y0, color);
    
    while (x < y) {
        if (f >= 0) { 
            y--; 
            ddF_y += 2; 
            f += ddF_y; 
        }
        x++; 
        ddF_x += 2; 
        f += ddF_x;
        
        drawPixel(x0 + x, y0 + y, color); 
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color); 
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color); 
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color); 
        drawPixel(x0 - y, y0 - x, color);
    }
}

void TFT_UI::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    fillRect(x0, y0 - r, 1, 2 * r + 1, color);
    
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    while (x < y) {
        if (f >= 0) { 
            y--; 
            ddF_y += 2; 
            f += ddF_y; 
        }
        x++; 
        ddF_x += 2; 
        f += ddF_x;
        
        fillRect(x0 + x, y0 - y, 1, 2 * y + 1, color);
        fillRect(x0 - x, y0 - y, 1, 2 * y + 1, color);
        fillRect(x0 + y, y0 - x, 1, 2 * x + 1, color);
        fillRect(x0 - y, y0 - x, 1, 2 * x + 1, color);
    }
}

// --- TEXT RENDERING ---

void TFT_UI::drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
    if (c < 32 || c > 127) return; // Keep within standard ASCII limits
    
    uint8_t fontIndex = c - 32;
    
    for (int8_t i = 0; i < 5; i++) { 
        uint8_t line = font5x7[fontIndex * 5 + i];
        
        for (int8_t j = 0; j < 8; j++, line >>= 1) { 
            if (line & 1) {
                fillRect(x + i * size, y + j * size, size, size, color);
            } else if (bg != color) {
                fillRect(x + i * size, y + j * size, size, size, bg);
            }
        }
    }
}

void TFT_UI::drawString(uint16_t x, uint16_t y, const char* text, uint16_t color, uint16_t bg, uint8_t size) {
    while (*text) {
        drawChar(x, y, *text, color, bg, size);
        x += (5 * size) + (1 * size); // Move cursor to the next character (+1 pixel spacing)
        text++;
    }
}

// ============================================================================
// STANDARD BUTTON IMPLEMENTATION
// ============================================================================

TFT_Button::TFT_Button() { 
    _ui = nullptr; 
    _action = nullptr; 
    _lastPressTime = 0; 
}

void TFT_Button::init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
                      uint16_t color, uint16_t textColor, const char* label, 
                      uint8_t textSize, ButtonCallback callback) {
    _ui = ui; 
    _x = x; 
    _y = y; 
    _w = w; 
    _h = h; 
    _color = color; 
    _textColor = textColor; 
    _label = label; 
    _textSize = textSize; 
    _action = callback;
}

void TFT_Button::setColor(uint16_t color) { 
    _color = color; 
    draw(); 
}

void TFT_Button::setLabel(const char* label) { 
    _label = label; 
    draw(); 
}

void TFT_Button::draw() {
    if (!_ui) {
        return;
    }
    
    // 1. Draw the button background
    _ui->fillRect(_x, _y, _w, _h, _color);

    // 2. Calculate centering based on the dynamic text size
    uint16_t tw = 0;
    const char* c = _label;
    
    // Each character is 6 pixels wide (5 pixels + 1 spacer) multiplied by scale
    while (*c) { 
        tw += (6 * _textSize); 
        c++; 
    } 
    
    uint16_t textX = _x + (_w - tw) / 2;
    uint16_t textY = _y + (_h - (8 * _textSize)) / 2; 

    // 3. Draw the text string centered
    _ui->drawString(textX, textY, _label, _textColor, _color, _textSize);
}

bool TFT_Button::checkTouch(uint16_t tx, uint16_t ty) {
    if (tx >= _x && tx <= (_x + _w) && ty >= _y && ty <= (_y + _h)) {
        
        if (millis() - _lastPressTime > 300) { 
            _lastPressTime = millis();
            
            // Tactile feedback: Flash white
            _ui->fillRect(_x, _y, _w, _h, TFT_WHITE); 
            delay(50); 
            draw();
            
            // Execute callback
            if (_action) {
                _action(); 
            }
            return true;
        }
    } 
    return false;
}

// ============================================================================
// TOGGLE SWITCH IMPLEMENTATION
// ============================================================================

TFT_Toggle::TFT_Toggle() { 
    _ui = nullptr; 
    _action = nullptr; 
    _lastPressTime = 0; 
    _state = false; 
}

void TFT_Toggle::init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
                      uint16_t colorOn, uint16_t colorOff, uint16_t textColor, 
                      const char* labelOn, const char* labelOff, 
                      uint8_t textSize, ButtonCallback callback) {
    _ui = ui; 
    _x = x; 
    _y = y; 
    _w = w; 
    _h = h; 
    _colorOn = colorOn; 
    _colorOff = colorOff; 
    _textColor = textColor; 
    _labelOn = labelOn; 
    _labelOff = labelOff; 
    _textSize = textSize; 
    _action = callback;
}

void TFT_Toggle::draw() {
    if (!_ui) {
        return;
    }
    
    // 1. Determine current color and label based on state
    uint16_t cc = _state ? _colorOn : _colorOff;
    const char* cl = _state ? _labelOn : _labelOff;

    // 2. Draw the background
    _ui->fillRect(_x, _y, _w, _h, cc);

    // 3. Calculate text centering
    uint16_t tw = 0;
    const char* c = cl;
    
    while (*c) { 
        tw += (6 * _textSize); 
        c++; 
    } 
    
    uint16_t textX = _x + (_w - tw) / 2;
    uint16_t textY = _y + (_h - (8 * _textSize)) / 2; 

    // 4. Draw the label
    _ui->drawString(textX, textY, cl, _textColor, cc, _textSize);
}

bool TFT_Toggle::getState() { 
    return _state; 
}

void TFT_Toggle::setState(bool state) { 
    _state = state; 
    draw(); 
}

bool TFT_Toggle::checkTouch(uint16_t tx, uint16_t ty) {
    if (tx >= _x && tx <= (_x + _w) && ty >= _y && ty <= (_y + _h)) {
        
        if (millis() - _lastPressTime > 300) { 
            _lastPressTime = millis(); 
            _state = !_state; 
            draw();
            
            if (_action) {
                _action(); 
            }
            return true;
        }
    } 
    return false;
}

// ============================================================================
// INTERACTIVE PROGRESS BAR / SLIDER IMPLEMENTATION
// ============================================================================

TFT_ProgressBar::TFT_ProgressBar() { 
    _ui = nullptr; 
    _currentVal = 0; 
    _action = nullptr; 
    _lastTouchTime = 0;
}

void TFT_ProgressBar::init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
                           uint16_t bgColor, uint16_t barColor, float minVal, float maxVal, ButtonCallback callback) {
    _ui = ui; 
    _x = x; 
    _y = y; 
    _w = w; 
    _h = h;
    _bgColor = bgColor; 
    _barColor = barColor; 
    _minVal = minVal; 
    _maxVal = maxVal; 
    _currentVal = minVal;
    _action = callback;
}

void TFT_ProgressBar::draw() { 
    if (!_ui) return;
    
    // Force a complete redraw (bypassing the speed optimization in setValue)
    uint16_t fillWidth = (uint16_t)(((_currentVal - _minVal) / (_maxVal - _minVal)) * _w);
    
    if (fillWidth > 0) {
        _ui->fillRect(_x, _y, fillWidth, _h, _barColor);
    }
    if (fillWidth < _w) {
        _ui->fillRect(_x + fillWidth, _y, _w - fillWidth, _h, _bgColor);
    }
}

float TFT_ProgressBar::getValue() { 
    return _currentVal; 
}

void TFT_ProgressBar::setValue(float val) {
    if (!_ui) return;
    
    if (val < _minVal) val = _minVal;
    if (val > _maxVal) val = _maxVal;
    
    // Don't redraw if the value hasn't mathematically changed pixels (Prevents lag)
    uint16_t oldFill = (uint16_t)(((_currentVal - _minVal) / (_maxVal - _minVal)) * _w);
    uint16_t newFill = (uint16_t)(((val - _minVal) / (_maxVal - _minVal)) * _w);
    _currentVal = val;
    
    if (oldFill == newFill) return; // Save processor time

    // Draw the active/filled segment
    if (newFill > 0) {
        _ui->fillRect(_x, _y, newFill, _h, _barColor);
    }
    
    // Draw the empty background segment
    if (newFill < _w) {
        _ui->fillRect(_x + newFill, _y, _w - newFill, _h, _bgColor);
    }
}

bool TFT_ProgressBar::checkTouch(uint16_t touchX, uint16_t touchY) {
    // Basic hit-box (with +20px fat finger allowance)
    bool inBounds = (touchX >= _x && touchX <= (_x + _w) && touchY >= (_y - 20) && touchY <= (_y + _h + 20));
    
    // DRAG LOCK: If touched inside, OR if we were just dragging it 150ms ago, lock on!
    if (inBounds || (millis() - _lastTouchTime < 150)) {
        
        // Debounce the physical hardware action, but allow the visual drag to be instant
        bool shouldTriggerAction = (millis() - _lastTouchTime > 50);
        _lastTouchTime = millis(); // Refresh the Drag-Lock timer!
        
        // Calculate percentage (Allow math to go outside physical X boundaries while dragging)
        float percentage = (float)((int16_t)touchX - (int16_t)_x) / (float)_w;
        if (percentage < 0.0) percentage = 0.0;
        if (percentage > 1.0) percentage = 1.0;
        
        setValue(_minVal + (percentage * (_maxVal - _minVal))); // Visually redraw

        if (_action != nullptr && shouldTriggerAction) {
            _action();
        }
        return true; // We consume the touch!
    }
    return false;
}

TFT_ImageButton::TFT_ImageButton() { 
    _tft = nullptr;
    _ui = nullptr; 
    _action = nullptr; 
    _lastPressTime = 0; 
}

void TFT_ImageButton::init(TFT35_V1_F407* tft, TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
                           const char* filename, ButtonCallback callback) {
    _tft = tft;
    _ui = ui; 
    _x = x; 
    _y = y; 
    _w = w; 
    _h = h; 
    _filename = filename; 
    _action = callback;
}

void TFT_ImageButton::draw() {
    if (!_tft) return;
    
    // Auto-Detect the file type based on the extension!
    const char* dot = strrchr(_filename, '.');
    
    if (dot != nullptr && (dot[1] == 'r' || dot[1] == 'R') && (dot[2] == 'a' || dot[2] == 'A') && (dot[3] == 'w' || dot[3] == 'W')) {
        // It's a custom RAW file! Pass the width and height.
        _tft->drawRAW(_filename, _x, _y, _w, _h);
    } else {
        // Assume standard BMP format
        _tft->drawBMP(_filename, _x, _y);
    }
}

bool TFT_ImageButton::checkTouch(uint16_t tx, uint16_t ty) {
    // Check the invisible Hitbox dimensions
    if (tx >= _x && tx <= (_x + _w) && ty >= _y && ty <= (_y + _h)) {
        
        if (millis() - _lastPressTime > 300) { 
            _lastPressTime = millis();
            
            // Tactile Feedback: Draw a fast white box around the image!
            if (_ui) {
                _ui->drawRect(_x, _y, _w, _h, TFT_WHITE);
                _ui->drawRect(_x + 1, _y + 1, _w - 2, _h - 2, TFT_WHITE); // Double thickness
                
                delay(80); 
                
                // Redraw the image to erase the white box
                draw(); 
            }
            
            // Execute the attached function
            if (_action) {
                _action(); 
            }
            return true;
        }
    } 
    return false;
}

SdFat& TFT35_V1_F407::getSD() {
    return sd;
}