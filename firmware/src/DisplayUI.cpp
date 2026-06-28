#include "DisplayUI.h"
#include "config.h"
#include <LiquidCrystal_I2C.h>

// Khởi tạo đối tượng lcd I2C với địa chỉ 0x27, màn hình 16 cột 2 hàng
LiquidCrystal_I2C lcd(0x27, 16, 2);

void DisplayUI::init() {
    // Khởi tạo màn hình
    lcd.init();
    lcd.backlight();
    
    // Cài đặt chân cho nút bấm (Sử dụng điện trở kéo lên nội bộ)
    pinMode(BTN_MODE_PIN, INPUT_PULLUP);
    pinMode(BTN_DOOR_PIN, INPUT_PULLUP);

    // Hiển thị dòng chữ chào mừng ban đầu
    showMessage("Smart Home", "Initializing...");
}

void DisplayUI::loop() {
    // Không cần xử lý nặng trong vòng lặp này đối với LCD
}

void DisplayUI::showMessage(const String& line1, const String& line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

void DisplayUI::updateSensorDisplay(float temp, float hum) {
    // Chỉ cập nhật màn hình mỗi 2 giây để chống nhấp nháy (flicker)
    unsigned long currentMillis = millis();
    if (currentMillis - lastLcdUpdate >= 2000) {
        lastLcdUpdate = currentMillis;
        
        // Tạo hiệu ứng xóa mượt (chỉ ghi đè lên hàng đầu tiên)
        // Cách này tốt hơn lcd.clear() vì không làm màn hình bị chớp
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temp, 1); // 1 chữ số thập phân
        lcd.print("C  ");
        
        lcd.setCursor(9, 0);
        lcd.print("H:");
        lcd.print(hum, 0); // Không lấy số thập phân
        lcd.print("%  ");
        
        // Cập nhật hàng 2 để xóa chữ "Initializing..."
        lcd.setCursor(0, 1);
        lcd.print("System Ready    ");
    }
}

bool DisplayUI::isModeButtonPressed() {
    bool currentState = digitalRead(BTN_MODE_PIN);
    bool pressed = false;

    // Phát hiện sườn âm (Từ HIGH xuống LOW)
    if (currentState == LOW && lastModeState == HIGH) {
        delay(50); // Thuật toán chống dội phím cơ bản (Debounce)
        if (digitalRead(BTN_MODE_PIN) == LOW) {
            pressed = true;
            modeButtonPressTime = millis();
            isHoldingMode = true;
        }
    } else if (currentState == HIGH && lastModeState == LOW) {
        isHoldingMode = false;
    }
    
    lastModeState = currentState;
    return pressed;
}

bool DisplayUI::isModeButtonHeld(unsigned long holdTimeMs) {
    if (isHoldingMode && (millis() - modeButtonPressTime >= holdTimeMs)) {
        isHoldingMode = false; // Ngăn chặn kích hoạt nhiều lần liên tiếp
        return true;
    }
    return false;
}

bool DisplayUI::isDoorButtonPressed() {
    bool currentState = digitalRead(BTN_DOOR_PIN);
    bool pressed = false;

    // Phát hiện sườn âm (Từ HIGH xuống LOW)
    if (currentState == LOW && lastDoorState == HIGH) {
        delay(50); // Thuật toán chống dội phím cơ bản (Debounce)
        if (digitalRead(BTN_DOOR_PIN) == LOW) {
            pressed = true;
        }
    }
    
    lastDoorState = currentState;
    return pressed;
}
