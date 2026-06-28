#include "DisplayUI.h"
#include "config.h"

// TODO: (Người 4) Khởi tạo đối tượng lcd I2C
// LiquidCrystal_I2C lcd(0x27, 16, 2);

void DisplayUI::init() {
    // TODO: (Người 4) Khởi tạo LCD I2C và pinMode cho 2 nút nhấn
}

void DisplayUI::loop() {
    // Thường không cần xử lý nặng ở vòng lặp này
}

void DisplayUI::showMessage(const String& line1, const String& line2) {
    // TODO: (Người 4) Viết code xóa màn hình và in 2 dòng Text lên LCD
}

void DisplayUI::updateSensorDisplay(float temp, float hum) {
    // TODO: (Người 4) Viết code in nhiệt độ và độ ẩm ở hàng trên của LCD (chỉ cập nhật mỗi 2 giây)
}

bool DisplayUI::isModeButtonPressed() {
    // TODO: (Người 4) Viết code bắt sự kiện bấm nút Mode (có chống dội - debounce)
    return false;
}

bool DisplayUI::isDoorButtonPressed() {
    // TODO: (Người 4) Viết code bắt sự kiện bấm nút Door (có chống dội - debounce)
    return false;
}
