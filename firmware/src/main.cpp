#include <Arduino.h>
#include "config.h"

// ==========================================
// INCLUDE CÁC MODULE
// Nhóm 5 (Core Logic) ghép tất cả vào đây
// ==========================================
#include "wifi_manager.h"   // Nhóm 1 — Mạng & API
// #include "DoorSystem.h"  // Nhóm 2 — RFID & Cửa       (uncomment khi Nhóm 2 xong)
// #include "SensorManager.h" // Nhóm 3 — Cảm biến        (uncomment khi Nhóm 3 xong)
// #include "DisplayUI.h"   // Nhóm 4 — Màn hình LCD      (uncomment khi Nhóm 4 xong)
// #include "AlertManager.h"// Nhóm 5 — Còi & Đèn báo    (uncomment khi Nhóm 5 xong)

// ==========================================
// BIẾN THỜI GIAN (millis-based, không delay)
// ==========================================
unsigned long lastSendTime    = 0;
unsigned long lastCmdTime     = 0;

const unsigned long SEND_INTERVAL = 10000; // Gửi cảm biến mỗi 10 giây
const unsigned long CMD_INTERVAL  = 5000;  // Kiểm tra lệnh server mỗi 5 giây

// ==========================================
// SETUP — Chạy 1 lần khi khởi động
// ==========================================
void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("==========================================");
    Serial.println("     SMART HOME — KHOI DONG HE THONG     ");
    Serial.println("==========================================");

    // Khởi tạo kết nối WiFi (Nhóm 1)
    initWiFi();

    // TODO (Nhóm 2): doorSystem.init();
    // TODO (Nhóm 3): sensorManager.init();
    // TODO (Nhóm 4): displayUI.init();
    // TODO (Nhóm 5): alertManager.init();

    Serial.println("[System] He thong san sang!");
}

// ==========================================
// LOOP — Chạy liên tục
// ==========================================
void loop() {

    // --- Nhóm 1: Duy trì WiFi (chạy mỗi vòng lặp) ---
    maintainWiFi();

    // --- TODO Nhóm 2: doorSystem.loop(); ---
    // --- TODO Nhóm 3: sensorManager.loop(); ---
    // --- TODO Nhóm 4: displayUI.loop(); ---
    // --- TODO Nhóm 5: alertManager.loop(); ---

    unsigned long now = millis();

    // --- Kịch bản: Gửi dữ liệu cảm biến mỗi 10 giây ---
    if (now - lastSendTime >= SEND_INTERVAL) {
        lastSendTime = now;

        // TODO: Thay giá trị giả lập bằng hàm thật của Nhóm 3
        // float temp  = sensorManager.getTemperature();
        // float humid = sensorManager.getHumidity();
        // float light = sensorManager.getLightValue();
        float temp  = 27.8;  // Giả lập
        float humid = 62.1;  // Giả lập
        float light = 300.5; // Giả lập

        sendSensorData(temp, humid, light);
    }

    // --- Kịch bản: Kiểm tra lệnh từ server mỗi 5 giây ---
    if (now - lastCmdTime >= CMD_INTERVAL) {
        lastCmdTime = now;

        String cmd = fetchServerCommand();
        if (cmd != "" && cmd != "[]") {
            Serial.print("[Core] Nhan lenh tu server: ");
            Serial.println(cmd);

            // TODO (Nhóm 5): Xử lý lệnh tại đây
            // Ví dụ: if (cmd.indexOf("LOCK") >= 0) doorSystem.lockDoor();
        }
    }
}
