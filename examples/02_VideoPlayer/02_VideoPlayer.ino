#include <TFT35_V1_F407.h>

TFT35_V1_F407 tft;

void setup() {
    tft.begin();
    tft.initSD(); // Initialize the SD card internally
}

void loop() {
    tft.playVideo("anim_lite.bin"); // Plays the whole video!
}
