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
    bool isDoorButtonPressed();

private:
    unsigned long lastLcdUpdate = 0;
    bool lastModeState = HIGH;
    bool lastDoorState = HIGH;
};

// extern DisplayUI displayUI;

#endif
