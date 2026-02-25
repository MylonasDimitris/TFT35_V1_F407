#include <TFT35_V1_F407.h>

TFT35_V1_F407 tft;

void setup() {
    // 1. Initialize the FSMC DMA engine and LCD
    tft.begin();
    
    // 2. Initialize the SD Card
    if (!tft.initSD()) {
        // SD Card failed to mount: Fill screen Red and halt
        tft.fillScreen(0xF800); 
        while(1); 
    }

    // 3. Open the video file
    if (!tft.openVideo("anim_lite.bin")) {
        // File not found on SD card: Fill screen Blue and halt
        tft.fillScreen(0x001F); 
        while(1);
    }
}

void loop() {
    // playFrame() pushes exactly one frame to the screen via DMA.
    // It returns 'false' when the video reaches the end of the file.
    if (!tft.playFrame()) {
        
        // The video ended. Re-open the file to loop it seamlessly!
        tft.openVideo("anim_lite.bin"); 
    }
}
