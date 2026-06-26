#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Arduino.h>

class DisplayUI {
public:
    void init();
    void loop();
    void updateSensorDisplay(float temp, float hum);
    void showMessage(const String& line1, const String& line2);
    bool isModeButtonPressed();
    bool isDoorButtonPressed();

private:
    unsigned long lastDisplayTime = 0;
    const unsigned long displayInterval = 2000;

    // Phim bam - Debounce
    bool lastModeBtnState = HIGH;
    unsigned long lastModeDebounceTime = 0;
    bool modeBtnPressed = false;

    bool lastDoorBtnState = HIGH;
    unsigned long lastDoorDebounceTime = 0;
    bool doorBtnPressed = false;

    const unsigned long debounceDelay = 50; // 50ms
};

#endif

