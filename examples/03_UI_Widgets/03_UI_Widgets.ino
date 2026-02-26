#include <TFT35_V1_F407.h>
#include <TFT_UI.h>

TFT35_V1_F407 tft;
TFT_UI ui(&tft);

TFT_Button btnAction;
TFT_Toggle swPower;
TFT_ProgressBar sldLevel;

void onBtn() { Serial.println("Action Triggered!"); }
void onSld() { Serial.print("Value: "); Serial.println(sldLevel.getValue()); }

void setup() {
    tft.begin();
    tft.initTouch();
    tft.fillScreen(TFT_BLACK);

    // init(ui, x, y, w, h, color, text_color, label, text_size, callback)
    btnAction.init(&ui, 20, 40, 120, 50, TFT_BLUE, TFT_WHITE, "RUN", 2, onBtn);
    
    // init(ui, x, y, w, h, cOn, cOff, tc, lOn, lOff, text_size, callback)
    swPower.init(&ui, 160, 40, 200, 50, TFT_GREEN, TFT_DARKGREY, TFT_WHITE, "ENABLED", "DISABLED", 2, nullptr);

    // init(ui, x, y, w, h, bg, bar, min, max, callback)
    sldLevel.init(&ui, 20, 120, 440, 30, TFT_DARKGREY, TFT_CYAN, 0.0, 100.0, onSld);

    btnAction.draw();
    swPower.draw();
    sldLevel.draw();
}

void loop() {
    uint16_t tx, ty;
    if (tft.getTouchCoordinates(&tx, &ty)) {
        btnAction.checkTouch(tx, ty);
        swPower.checkTouch(tx, ty);
        sldLevel.checkTouch(tx, ty);
    }
}