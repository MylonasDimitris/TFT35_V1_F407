#include <TFT35_V1_F407.h>

TFT35_V1_F407 tft;

void setup() {
    tft.begin();
}

void loop() {
    tft.fillScreen(0xF800); // RED
    delay(1000);
    tft.fillScreen(0x001F); // BLUE
    delay(1000);
}
