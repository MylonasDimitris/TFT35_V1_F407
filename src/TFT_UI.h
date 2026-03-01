#ifndef TFT_UI_H
#define TFT_UI_H

#include <Arduino.h>
#include "TFT35_V1_F407.h"

// --- PREDEFINED COLORS (RGB565) ---
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

#define MAX_GRID_SLOTS 24

typedef void (*ButtonCallback)();

/**
 * @brief The core Graphics Engine for drawing shapes, text, and colors.
 */
class TFT_UI {
public:
    /**
     * @brief Initializes the UI engine and binds it to the hardware display.
     * @param tft Pointer to the main TFT hardware driver instance.
     */
    TFT_UI(TFT35_V1_F407* tft);
    
    /**
     * @brief Converts a standard 24-bit web hex color to 16-bit RGB565.
     * @param hex24 The 24-bit color (e.g. 0xFF0000 for Red).
     * @return uint16_t The 16-bit color ready for the LCD.
     */
    uint16_t rgb565(uint32_t hex24);

    /**
     * @brief Draws a single pixel on the screen.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param color 16-bit color.
     */
    void drawPixel(int16_t x, int16_t y, uint16_t color);

    /**
     * @brief Draws a solid filled rectangle.
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param w Width in pixels.
     * @param h Height in pixels.
     * @param color 16-bit fill color.
     */
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

    /**
     * @brief Draws an empty rectangle outline (1 pixel thick).
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param w Width in pixels.
     * @param h Height in pixels.
     * @param color 16-bit outline color.
     */
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

    /**
     * @brief Draws a straight line between two points.
     * @param x0 Starting X coordinate.
     * @param y0 Starting Y coordinate.
     * @param x1 Ending X coordinate.
     * @param y1 Ending Y coordinate.
     * @param color 16-bit line color.
     */
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

    /**
     * @brief Draws an empty circle outline.
     * @param x0 Center X coordinate.
     * @param y0 Center Y coordinate.
     * @param r Radius in pixels.
     * @param color 16-bit outline color.
     */
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

    /**
     * @brief Draws a solid filled circle.
     * @param x0 Center X coordinate.
     * @param y0 Center Y coordinate.
     * @param r Radius in pixels.
     * @param color 16-bit fill color.
     */
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    
    /**
     * @brief Draws a single ASCII character.
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param c The character to draw.
     * @param color 16-bit text color.
     * @param bg 16-bit background color (Use same as text color for transparent).
     * @param size Multiplier for text scale (1 = 5x7, 2 = 10x14, etc.).
     */
    void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);

    /**
     * @brief Draws a null-terminated text string.
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param text Pointer to the character array/string.
     * @param color 16-bit text color.
     * @param bg 16-bit background color (Use same as text color for transparent).
     * @param size Multiplier for text scale.
     */
    void drawString(uint16_t x, uint16_t y, const char* text, uint16_t color, uint16_t bg, uint8_t size);


private:
    TFT35_V1_F407* _tft;
};

/**
 * @brief Base class for all interactive UI elements.
 */
class TFT_Widget {
public:
    virtual void draw() = 0;
    virtual bool checkTouch(uint16_t tx, uint16_t ty) = 0;
    virtual void setGeometry(uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
};

/**
 * @brief A standard solid-color clickable button with centered text.
 */
class TFT_Button : public TFT_Widget{
public:
    TFT_Button();

    /**
     * @brief Initializes the standard button.
     * @param ui Pointer to the UI engine.
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param w Width in pixels.
     * @param h Height in pixels.
     * @param color 16-bit background color.
     * @param textColor 16-bit text color.
     * @param label Text to display inside the button.
     * @param textSize Scale of the text.
     * @param callback Function to execute when pressed.
     */
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
            uint16_t color, uint16_t textColor, const char* label, 
            uint8_t textSize, ButtonCallback callback);

    void setGeometry(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;

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


/**
 * @brief A clickable button that renders an image from the SD card.
 */
class TFT_ImageButton : public TFT_Widget{
public:
    TFT_ImageButton();
    
    /**
     * @brief Initializes the image button.
     * @param tft Pointer to the hardware display driver (for SD access).
     * @param ui Pointer to the UI engine.
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param w Width in pixels (ignored for BMP, required for RAW).
     * @param h Height in pixels (ignored for BMP, required for RAW).
     * @param filename 8.3 formatted filename on SD card (e.g. "ICON.RAW").
     * @param callback Function to execute when pressed.
     */
    void init(TFT35_V1_F407* tft, TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
              const char* filename, ButtonCallback callback);
    void setGeometry(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;
              
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


/**
 * @brief A two-state toggle switch (On/Off).
 */
class TFT_Toggle : public TFT_Widget{
public:
    TFT_Toggle();

    /**
     * @brief Initializes the toggle switch.
     * @param ui Pointer to the UI engine.
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param w Width in pixels.
     * @param h Height in pixels.
     * @param colorOn 16-bit background color when active.
     * @param colorOff 16-bit background color when inactive.
     * @param textColor 16-bit text color.
     * @param labelOn Text to display when active.
     * @param labelOff Text to display when inactive.
     * @param textSize Scale of the text.
     * @param callback Function to execute when toggled.
     */
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
            uint16_t colorOn, uint16_t colorOff, uint16_t textColor, 
            const char* labelOn, const char* labelOff, 
            uint8_t textSize, ButtonCallback callback);

    void setGeometry(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;

    void draw();

    bool checkTouch(uint16_t touchX, uint16_t touchY);

    /** @brief Returns true if toggle is ON, false if OFF. */
    bool getState();

    /** @brief Programmatically sets the state and redraws. */
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

/**
 * @brief A dynamic slider/progress bar that can be updated or dragged.
 */
class TFT_ProgressBar : public TFT_Widget{
public:
    TFT_ProgressBar();
    
    /**
     * @brief Initializes the progress bar.
     * @param ui Pointer to the UI engine.
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param w Width in pixels.
     * @param h Height in pixels.
     * @param bgColor 16-bit background (empty) color.
     * @param barColor 16-bit fill (active) color.
     * @param minVal Minimum mathematical value.
     * @param maxVal Maximum mathematical value.
     * @param callback Optional function to execute when dragged/touched.
     */
    void init(TFT_UI* ui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
              uint16_t bgColor, uint16_t barColor, float minVal, float maxVal, ButtonCallback callback = nullptr);

    void setGeometry(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;

    void draw();

    /** @brief Updates the bar's fill level. */
    void setValue(float val);

    /** @brief Returns the current internal value. */
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

/**
 * @brief An automatic Layout Manager that arranges widgets into rows and columns.
 */
class TFT_Grid {
public:
    TFT_Grid();
    
    /**
     * @brief Initializes the grid boundaries and cell count.
     * @param startX X coordinate of the entire grid's top-left corner.
     * @param startY Y coordinate of the entire grid's top-left corner.
     * @param w Total width of the grid area.
     * @param h Total height of the grid area.
     * @param cols Number of columns.
     * @param rows Number of rows.
     * @param padding Pixel gap between cells and edges.
     */
    void init(uint16_t startX, uint16_t startY, uint16_t w, uint16_t h, 
              uint8_t cols, uint8_t rows, uint8_t padding);
              
    /**
     * @brief Places a widget into a grid slot and forces it to adopt the slot's dimensions.
     * @param slotIndex The slot index (0 = top left, counting left-to-right).
     * @param widget Pointer to any UI element inheriting from TFT_Widget.
     * @param colSpan Number of columns to stretch across (default 1).
     * @param rowSpan Number of rows to stretch down (default 1).
     * @return true if assignment successful, false if out of bounds.
     */
    bool assignWidget(uint8_t slotIndex, TFT_Widget* widget, uint8_t colSpan = 1, uint8_t rowSpan = 1);

    /**
     * @brief Removes a widget from the grid.
     * @param slotIndex The slot index to clear.
     */
    void clearSlot(uint8_t slotIndex);
    
    /** @brief Instructs all assigned widgets to draw themselves. */
    void draw();

    /** @brief Checks touch coordinates against all assigned widgets. */
    bool checkTouch(uint16_t touchX, uint16_t touchY);

private:
    uint16_t _x, _y, _w, _h;
    uint8_t _cols, _rows, _padding;
    uint16_t _btnW, _btnH;
    
    TFT_Widget* _slots[MAX_GRID_SLOTS]; 
};

#endif