#include "DisplayUI.h"
#include "config.h"
#include <LiquidCrystal_I2C.h>

// Dia chi I2C pho bien la 0x27, man hinh 16x2
static LiquidCrystal_I2C lcd(0x27, 16, 2);

void DisplayUI::init() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    // Thiet lap nut nhan Pullup
    pinMode(BTN_MODE_PIN, INPUT_PULLUP);
    pinMode(BTN_DOOR_PIN, INPUT_PULLUP);
    
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM STARTING");
    lcd.setCursor(0, 1);
    lcd.print("PLEASE WAIT...");
}

void DisplayUI::loop() {
    // Rong
}

void DisplayUI::updateSensorDisplay(float temp, float hum) {
    if (millis() - lastDisplayTime >= displayInterval) {
        lastDisplayTime = millis();
        
        char line1[17];
        // Format chuoi 16 ky tu vua khop man hinh tranh bi lap lai ky tu cu
        snprintf(line1, sizeof(line1), "T: %.1fC  H: %.1f%%", temp, hum);
        
        lcd.setCursor(0, 0);
        lcd.print(line1);
        
        // Hàng 2 co the hien thi trang thai he thong
        lcd.setCursor(0, 1);
        lcd.print("SYSTEM SAFE     ");
    }
}

void DisplayUI::showMessage(const String& line1, const String& line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

bool DisplayUI::isModeButtonPressed() {
    bool reading = digitalRead(BTN_MODE_PIN);
    bool pressed = false;
    
    if (reading != lastModeBtnState) {
        lastModeDebounceTime = millis();
    }
    
    if ((millis() - lastModeDebounceTime) > debounceDelay) {
        if (reading == LOW && !modeBtnPressed) {
            pressed = true;
            modeBtnPressed = true;
        } else if (reading == HIGH) {
            modeBtnPressed = false;
        }
    }
    
    lastModeBtnState = reading;
    return pressed;
}

bool DisplayUI::isDoorButtonPressed() {
    bool reading = digitalRead(BTN_DOOR_PIN);
    bool pressed = false;
    
    if (reading != lastDoorBtnState) {
        lastDoorDebounceTime = millis();
    }
    
    if ((millis() - lastDoorDebounceTime) > debounceDelay) {
        if (reading == LOW && !doorBtnPressed) {
            pressed = true;
            doorBtnPressed = true;
        } else if (reading == HIGH) {
            doorBtnPressed = false;
        }
    }
    
    lastDoorBtnState = reading;
    return pressed;
}

