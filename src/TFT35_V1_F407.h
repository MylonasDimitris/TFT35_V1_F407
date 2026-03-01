#ifndef TFT35_V1_F407_H
#define TFT35_V1_F407_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>

// Hardware Addresses for FSMC LCD Mapping
#define LCD_REG (*((volatile uint16_t *) 0x60000000))
#define LCD_DAT (*((volatile uint16_t *) 0x61000000))

// --- PIN DEFINITIONS ---
#define L_BL     PD13
#define SD_CS    PA4

// --- TOUCH PANEL PINS ---
#define TOUCH_IRQ  PA8
#define TOUCH_SCK  PC6  // DCLK
#define TOUCH_CS   PC7  // CS
#define TOUCH_MOSI PC8  // DIN
#define TOUCH_MISO PC9  // DOUT

// Video/Display Constraints
#define WIDTH 480
#define HEIGHT 320
#define LINES_PER_BLOCK 16
#define PIXELS_PER_BLOCK (WIDTH * LINES_PER_BLOCK)

/**
 * @brief Low-level Hardware Driver for the TFT35 V1.0 (STM32F407VET6).
 * Manages FSMC Display communication, DMA transfers, SD Card, and Touch.
 */
class TFT35_V1_F407 {
public:
    TFT35_V1_F407();

    /**
     * @brief Initializes FSMC, GPIOs, Backlight, and the LCD Controller.
     */
    void begin();

    /**
     * @brief Sets the active drawing window on the LCD.
     * @param x1 Starting X coordinate.
     * @param y1 Starting Y coordinate.
     * @param x2 Ending X coordinate.
     * @param y2 Ending Y coordinate.
     */
    void setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

    /**
     * @brief Fills the entire screen with a single solid color.
     * @param color 16-bit RGB565 color.
     */
    void fillScreen(uint16_t color);

    /**
     * @brief Initializes the SD card using the SPI protocol.
     * @return true if SD card is found and initialized, false otherwise.
     */
    bool initSD();

    /**
     * @brief Opens a .BIN video file from the SD card for playback.
     * @param filename 8.3 formatted filename (e.g. "VIDEO.BIN").
     * @return true if file opened successfully.
     */
    bool openVideo(const char* filename);

    /**
     * @brief Decodes and displays the next frame of the opened video file.
     * Uses double-buffered DMA for high-speed playback.
     * @return true if frame was played, false if EOF or error.
     */
    bool playFrame();

    /**
     * @brief Draws a standard 24-bit BMP image from the SD card.
     * @param filename Path to the .bmp file.
     * @param x X position to start drawing.
     * @param y Y position to start drawing.
     * @return true if successful.
     */
    bool drawBMP(const char* filename, int16_t x, int16_t y);

    /**
     * @brief Draws a custom 16-bit RAW RGB565 image from the SD card.
     * @param filename Path to the .raw file.
     * @param x X position.
     * @param y Y position.
     * @param w Width of the image.
     * @param h Height of the image.
     * @return true if successful.
     */
    bool drawRAW(const char* filename, int16_t x, int16_t y, int16_t w, int16_t h);

    /**
     * @brief Configures the SPI pins and IRQ for the XPT2046 touch controller.
     */
    void initTouch();

    /**
     * @brief Checks the hardware IRQ pin to see if the screen is being pressed.
     * @return true if the panel is currently touched.
     */
    bool isTouched();

    /**
     * @brief Reads, filters, and maps raw touch data to screen coordinates.
     * @param x Pointer to store the resulting X coordinate (0-480).
     * @param y Pointer to store the resulting Y coordinate (0-320).
     * @return true if a valid, stable touch was detected.
     */
    bool getTouchCoordinates(uint16_t *x, uint16_t *y);

    /**
     * @brief Temporarily disables touch processing (useful for debouncing).
     * @param milliseconds Duration of the lock.
     */
    void lockTouch(uint16_t milliseconds);
    
    /**
     * @brief Provides access to the internal SdFat instance.
     * @return Reference to the SdFat object.
     */
    SdFat& getSD();

    /**
     * @brief Scans the root directory and returns a sorted list of filenames.
     * @param fileList A 2D array [maxFiles][13] to store the names.
     * @param maxFiles Maximum number of files to retrieve.
     * @param extensionFilter Optional filter (e.g., ".RAW").
     * @param sortAlphabetical If true, applies a bubble sort to the results.
     * @return uint16_t Total number of files found and stored.
     */
    uint16_t getFiles(char fileList[][13], uint16_t maxFiles, const char* extensionFilter = nullptr, bool sortAlphabetical = true);

private:
    void initFSMC();
    void initLCD();
    void sendBufferDMA(uint16_t* source, uint32_t len);
    bool isDMAReady();
    unsigned long _touchLockTime;

    SdFat sd;
    File animFile;
    uint16_t framePalette[256];
    uint8_t  indexedBuffer[PIXELS_PER_BLOCK];
    uint16_t colorBuffer[2][PIXELS_PER_BLOCK];
};

#endif