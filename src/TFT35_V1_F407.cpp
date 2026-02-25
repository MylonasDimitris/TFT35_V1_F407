#include "TFT35_V1_F407.h"

volatile bool screenTapped = false;
uint32_t actualTouchCS = 0xFFFFFFFF; 

void touchInterrupt() {
    screenTapped = true; 
}

TFT35_V1_F407::TFT35_V1_F407() {
}

void TFT35_V1_F407::begin() {
    initFSMC();
    
    pinMode(L_BL, OUTPUT);     
    digitalWrite(L_BL, HIGH);
    
    pinMode(L_BL_ALT, OUTPUT); 
    digitalWrite(L_BL_ALT, HIGH);
    
    initLCD();
}

void TFT35_V1_F407::setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    LCD_REG = 0x2A;
    LCD_DAT = (x1 >> 8); LCD_DAT = (x1 & 0xFF); LCD_DAT = (x2 >> 8); LCD_DAT = (x2 & 0xFF);
    LCD_REG = 0x2B; LCD_DAT = (y1 >> 8); LCD_DAT = (y1 & 0xFF); LCD_DAT = (y2 >> 8); LCD_DAT = (y2 & 0xFF);
    LCD_REG = 0x2C;
}

void TFT35_V1_F407::fillScreen(uint16_t color) {
    setWindow(0, 0, 479, 319); 
    for(uint32_t i=0; i<153600; i++) {
        LCD_DAT = color;
    }
}

void TFT35_V1_F407::initFSMC() {
    RCC->AHB3ENR |= RCC_AHB3ENR_FSMCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN;

    uint32_t pinsD[] = {0, 1, 4, 5, 7, 8, 9, 10, 14, 15};
    for(int p : pinsD) {
        GPIOD->MODER &= ~(3 << (p * 2));
        GPIOD->MODER |= (2 << (p * 2));
        GPIOD->OSPEEDR |= (3 << (p * 2));
        if(p < 8) GPIOD->AFR[0] |= (12 << (p * 4)); else GPIOD->AFR[1] |= (12 << ((p-8) * 4));
    }
    
    uint32_t pinsE[] = {2, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    for(int p : pinsE) {
        GPIOE->MODER &= ~(3 << (p * 2));
        GPIOE->MODER |= (2 << (p * 2));
        GPIOE->OSPEEDR |= (3 << (p * 2));
        if(p < 8) GPIOE->AFR[0] |= (12 << (p * 4)); else GPIOE->AFR[1] |= (12 << ((p-8) * 4));
    }
    
    // Configured for high-speed transfers
    FSMC_Bank1->BTCR[0] = FSMC_BCR1_MWID_0 | FSMC_BCR1_WREN | FSMC_BCR1_MBKEN;
    FSMC_Bank1->BTCR[1] = (0 << 0) | (3 << 8) | (0 << 16);
}

void TFT35_V1_F407::initLCD() {
    LCD_REG = 0x11; delay(150);
    LCD_REG = 0x3A; LCD_DAT = 0x55;
    LCD_REG = 0x36; LCD_DAT = 0x28; // BGR orientation (Landscape)
    LCD_REG = 0x29; delay(100);
}

void TFT35_V1_F407::sendBufferDMA(uint16_t* source, uint32_t len) {
    DMA2_Stream0->CR &= ~DMA_SxCR_EN;
    while(DMA2_Stream0->CR & DMA_SxCR_EN);
    
    DMA2->LIFCR = 0x3D;
    DMA2_Stream0->PAR = (uint32_t)source;
    DMA2_Stream0->M0AR = (uint32_t)&LCD_DAT;
    DMA2_Stream0->NDTR = len;
    
    // Priority: Very High, M2M, Source Inc, Dest Fixed
    DMA2_Stream0->CR = (3 << 16) | (2 << 6) | (1 << 9) | (1 << 13) | (1 << 11) | (1 << 0);
}

bool TFT35_V1_F407::isDMAReady() {
    return (DMA2->LISR & (1 << 5));
}

// --- Internal SD and Video Pipeline ---

bool TFT35_V1_F407::initSD() {
    return sd.begin(SD_CS, SD_SCK_MHZ(50));
}

bool TFT35_V1_F407::openVideo(const char* filename) {
    if (animFile.isOpen()) {
        animFile.close(); 
    }
    return animFile.open(filename, O_READ);
}

bool TFT35_V1_F407::isTouched() {
    return (digitalRead(TOUCH_IRQ) == LOW);
}

void TFT35_V1_F407::initTouch() {
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH); // Put touch chip to sleep
    
    pinMode(TOUCH_SCK, OUTPUT);
    digitalWrite(TOUCH_SCK, LOW);
    
    pinMode(TOUCH_MOSI, OUTPUT);
    digitalWrite(TOUCH_MOSI, LOW);
    
    pinMode(TOUCH_MISO, INPUT);
    pinMode(TOUCH_IRQ, INPUT_PULLUP); 
}

static uint16_t readTouchData(uint8_t command) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(TOUCH_MOSI, (command & 0x80) ? HIGH : LOW);
        command <<= 1;
        digitalWrite(TOUCH_SCK, LOW);  delayMicroseconds(1);
        digitalWrite(TOUCH_SCK, HIGH); delayMicroseconds(1);
    }
    digitalWrite(TOUCH_MOSI, LOW);
    digitalWrite(TOUCH_SCK, LOW);
    
    // Wait for the analog voltage to travel across the glass
    delayMicroseconds(15); 
    
    // Pulse the BUSY clock
    digitalWrite(TOUCH_SCK, HIGH); delayMicroseconds(1);
    digitalWrite(TOUCH_SCK, LOW);  delayMicroseconds(1);
    

    uint16_t result = 0;
    for (int i = 0; i < 12; i++) {
        // READ THE BIT FIRST!
        result <<= 1;
        if (digitalRead(TOUCH_MISO) == HIGH) {
            result |= 1;
        }
        
        digitalWrite(TOUCH_SCK, HIGH); delayMicroseconds(1);
        digitalWrite(TOUCH_SCK, LOW);  delayMicroseconds(1);
    }

    // Clear the remaining 3 empty clock cycles
    for (int i = 0; i < 3; i++) {
        digitalWrite(TOUCH_SCK, HIGH); delayMicroseconds(1);
        digitalWrite(TOUCH_SCK, LOW);  delayMicroseconds(1);
    }
    
    return result; 
}


bool TFT35_V1_F407::getTouchCoordinates(uint16_t *x, uint16_t *y) {
    if (!isTouched()) return false; 

    digitalWrite(SD_CS, HIGH);
    digitalWrite(TOUCH_CS, LOW); 
    delayMicroseconds(5); 

    // --- 1. TAKE TWO SAMPLES ---
    uint16_t rawX1 = readTouchData(0xD0);
    uint16_t rawY1 = readTouchData(0x90);
    
    uint16_t rawX2 = readTouchData(0xD0);
    uint16_t rawY2 = readTouchData(0x90);

    digitalWrite(TOUCH_CS, HIGH); 

    // Basic Noise filter
    if (rawX1 < 100 || rawX1 > 4090 || rawY1 < 100 || rawY1 > 4090) return false;

    // --- 2. THE LIFT-OFF DRIFT FILTER ---
    // Calculate the difference between the two samples
    int diffX = (int)rawX1 - (int)rawX2;
    int diffY = (int)rawY1 - (int)rawY2;
    
    // If the coordinates jumped by more than 50 raw points in one microsecond, 
    // your finger is lifting off! Discard the garbage data.
    if (abs(diffX) > 50 || abs(diffY) > 50) return false;

    // Use the first stable reading
    uint16_t rawX = rawX1;
    uint16_t rawY = rawY1;

    // Constrain Lock
    rawX = constrain(rawX, 200, 3900);
    rawY = constrain(rawY, 200, 3900);
    
    // Full Resolution Map
    long mappedX = map(rawY, 3900, 200, 0, 480);
    long mappedY = map(rawX, 200, 3900, 0, 320); 

    *x = mappedX;
    *y = mappedY;

    return true;
}

bool TFT35_V1_F407::playFrame() {
    if (!animFile.isOpen() || animFile.available() < 154114) {
        animFile.close();
        return false; 
    }

    uint32_t frameStartPos = animFile.curPosition();

    if (animFile.read(framePalette, 512) != 512) return false;
    
    setWindow(0, 0, WIDTH - 1, HEIGHT - 1);
    uint8_t dmaBufIdx = 0;
    
    for (int y = 0; y < HEIGHT; y += LINES_PER_BLOCK) {
        if (animFile.read(indexedBuffer, PIXELS_PER_BLOCK) != PIXELS_PER_BLOCK) return false;
        
        uint16_t* targetBuffer = colorBuffer[dmaBufIdx];
        for (int i = 0; i < PIXELS_PER_BLOCK; i++) {
            targetBuffer[i] = framePalette[indexedBuffer[i]];
        }

        while(y > 0 && !isDMAReady());
        sendBufferDMA(targetBuffer, PIXELS_PER_BLOCK);
        dmaBufIdx = 1 - dmaBufIdx;
    }
    
    while(!isDMAReady());
    
    animFile.seekSet(frameStartPos + 154114);
    return true; 
}