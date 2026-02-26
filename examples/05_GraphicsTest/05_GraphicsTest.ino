#include <TFT35_V1_F407.h>
#include <TFT_UI.h>

TFT35_V1_F407 tft;
TFT_UI ui(&tft);

void setup() {
    tft.begin();
    tft.fillScreen(TFT_BLACK);

    // Draw pattern
    for (int i=0; i<480; i+=20) ui.drawLine(0, 0, i, 319, TFT_RED);
    for (int i=0; i<320; i+=20) ui.drawLine(0, 0, 479, i, TFT_BLUE);

    ui.fillCircle(240, 160, 60, TFT_GREEN);
    ui.drawCircle(240, 160, 70, TFT_WHITE);
    ui.drawRect(100, 100, 280, 120, TFT_YELLOW);
}

void loop() {}