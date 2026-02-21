#include "TFT35_V1_F407.h"

TFT35_V1_F407::TFT35_V1_F407() {
}

void TFT35_V1_F407::begin() {
    initFSMC();
    
    pinMode(L_BL, OUTPUT);     
    digitalWrite(L_BL, HIGH);
    
    pinMode(L_BL_ALT, OUTPUT); 
    digitalWrite(L_BL_ALT, HIGH);
    
    pinMode(L_RST, OUTPUT);
    digitalWrite(L_RST, LOW); 
    delay(200); 
    digitalWrite(L_RST, HIGH); 
    delay(200);

    initLCD();
}

void TFT35_V1_F407::setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    LCD_REG = 0x2A;
    LCD_DAT = (x1 >> 8); LCD_DAT = (x1 & 0xFF); LCD_DAT = (x2 >> 8); LCD_DAT = (x2 & 0xFF);
    LCD_REG = 0x2B; LCD_DAT = (y1 >> 8); LCD_DAT = (y1 & 0xFF); LCD_DAT = (y2 >> 8); LCD_DAT = (y2 & 0xFF);
    LCD_REG = 0x2C;
}

void TFT35_V1_F407::fillScreen(uint16_t color) {
    setWindow(0, 0, 319, 479);
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
    LCD_REG = 0x36; LCD_DAT = 0x28; // BGR orientation
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

// --- NEW: Internal SD and Video Pipeline ---

bool TFT35_V1_F407::initSD() {
    return sd.begin(SD_CS, SD_SCK_MHZ(50));
}

void TFT35_V1_F407::playVideo(const char* filename) {
    if (!animFile.open(filename, O_READ)) {
        return; 
    }

    while (animFile.available()) {
        // 1. Read Palette
        if (animFile.read(framePalette, 512) != 512) break;
        
        setWindow(0, 0, WIDTH - 1, HEIGHT - 1);
        uint8_t dmaBufIdx = 0;
        
        // 2. Process frame in blocks
        for (int y = 0; y < HEIGHT; y += LINES_PER_BLOCK) {
            animFile.read(indexedBuffer, PIXELS_PER_BLOCK);
            
            uint16_t* targetBuffer = colorBuffer[dmaBufIdx];
            for (int i = 0; i < PIXELS_PER_BLOCK; i++) {
                targetBuffer[i] = framePalette[indexedBuffer[i]];
            }

            // Wait for previous DMA block to finish pushing to the screen
            while(y > 0 && !isDMAReady());
            
            sendBufferDMA(targetBuffer, PIXELS_PER_BLOCK);
            dmaBufIdx = 1 - dmaBufIdx;
        }
        
        // 3. Skip 0xAA55 sync marker
        animFile.seekCur(2);
    }
    
    animFile.close();
}