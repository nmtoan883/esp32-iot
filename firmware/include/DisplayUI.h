#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class DisplayUI {
public:
    void init();
    void loop();
    void showMessage(const String& line1, const String& line2);
    void updateSensorDisplay(float temp, float hum);

    bool isModeButtonPressed();
    bool isModeButtonHeld(unsigned long holdTimeMs); // Mới thêm
    bool isDoorButtonPressed();

private:
    unsigned long lastLcdUpdate = 0;
    unsigned long modeButtonPressTime = 0;
    bool lastModeState = HIGH;
    bool isHoldingMode = false;
    bool lastDoorState = HIGH;
};

// extern DisplayUI displayUI;

#endif
