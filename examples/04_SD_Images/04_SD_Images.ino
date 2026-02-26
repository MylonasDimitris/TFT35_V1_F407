#include <TFT35_V1_F407.h>
#include <TFT_UI.h>

TFT35_V1_F407 tft;
TFT_UI ui(&tft);

void setup() {
    tft.begin();
    if (!tft.initSD()) {
        ui.drawString(10, 10, "No SD Card Found", TFT_RED, TFT_BLACK, 2);
        while(1);
    }

    // 1. Blast a RAW background (Blazing fast, no header overhead)
    tft.drawRAW("BG.RAW", 0, 0, 480, 320);

    // 2. Load a standard 24-bit BMP icon on top
    tft.drawBMP("ICON.BMP", 190, 110);
    
    ui.drawString(10, 10, "Media Loaded", TFT_YELLOW, 0, 1);
}

void loop() {}