#include <Arduino.h>
#include "config.h"

// ============================================================
// Nhom 5 - main.cpp  |  Trai tim he thong Smart Home
// Dieu phoi toan bo 5 module chay song song bang millis()
//
// TICH HOP TUNG BUOC:
//   [x] AlertManager  - Hoan chinh
//   [x] DoorSystem    - Hoan chinh
//   [ ] SensorManager - Cho Nhom 3 hoan thien roi bo comment
//   [ ] DisplayUI     - Cho Nhom 4 hoan thien roi bo comment
//   [ ] NetworkManager- Cho Nhom 1 hoan thien roi bo comment
// ============================================================

// --- MODULE DA HOAN CHINH ---
#include "AlertManager.h"
#include "DoorSystem.h"

// --- MODULE DANG PHAT TRIEN (bo comment khi nhom tuong ung nop code) ---
#include "SensorManager.h"
#include "DisplayUI.h"
#include "NetworkManager.h"

// ==========================================
// KHAI BAO DOI TUONG TOAN CUC
// ==========================================
AlertManager alert;
DoorSystem   door;

SensorManager  sensor;   // Uncomment khi Nhom 3 xong
DisplayUI      display;  // Uncomment khi Nhom 4 xong
// NetworkManager network;  // Uncomment khi Nhom 1 xong

// ==========================================
// BO DEM GUI DU LIEU LEN SERVER (10 giay / lan)
// ==========================================
static const unsigned long WEB_PUSH_INTERVAL = 10000; // 10 giay
static unsigned long lastWebPushTime = 0;

// ==========================================
// SETUP - Khoi tao toan bo he thong
// ==========================================
void setup() {
    Serial.begin(115200);
    delay(500); // Cho Serial on dinh
    Serial.println("==============================================");
    Serial.println("   Smart Home - He thong Canh bao v1.0");
    Serial.println("==============================================");

    // Khoi tao cac module theo thu tu
    pinMode(FAN_RELAY_PIN, OUTPUT);
    digitalWrite(FAN_RELAY_PIN, HIGH); // Tắt quạt mặc định (Kích mức thấp nên HIGH là Tắt)
    
    alert.init();    // 1. He thong canh bao (AlertManager)
    door.init();     // 2. He thong cua RFID (DoorSystem)

    sensor.init();  // 3. Cam bien (Nhom 3)
    display.init(); // 4. Man hinh LCD (Nhom 4)
    initWiFi(); // 5. Mang WiFi (Nhom 1)

    Serial.println("[main] Khoi tao xong. He thong san sang.");
    Serial.println("----------------------------------------------");
}

// ==========================================
// LOOP - Vong lap chinh, tuyet doi khong blocking
// ==========================================
void loop() {

    // ==========================================
    // BUOC 1: Chay vong lap nen cua tung module
    // ==========================================
    alert.loop();    // Chop den/coi bao dong (non-blocking)
    door.loop();     // Tu dong khoa cua sau 5 giay (non-blocking)

    sensor.loop();  // Cap nhat DHT11 moi 2 giay (Nhom 3)
    display.loop(); // Lam moi man hinh LCD (Nhom 4)
    maintainWiFi(); // Kiem tra va giu ket noi WiFi (Nhom 1)
    
    // ==========================================
    // XỬ LÝ NÚT NHẤN VẬT LÝ
    // ==========================================
    if (display.isModeButtonPressed()) {
        alert.stopAlarm(); // Nhấn nút Mode 1 cái để tắt còi rú khẩn cấp
    }
    if (display.isModeButtonHeld(5000)) {
        display.showMessage("RESETTING WIFI", "PLEASE WAIT...");
        resetWiFi(); // Nhấn giữ nút Mode 5 giây để xóa Wifi
    }
    
    if (display.isDoorButtonPressed()) {
        door.unlockDoor(); // Nhấn nút Door để mở cửa từ bên trong
        Serial.println("[main] Nut Door: Da mo cua tu ben trong.");
        display.showMessage("CUA DA MO", "XIN MOI RA");
    }

    // ==========================================
    // KICH BAN 8: CHỐNG KẸP CỬA THÔNG MINH (PIR)
    // Nếu cửa ĐANG MỞ mà phát hiện CÒN NGƯỜI đứng đó, 
    // tự động gia hạn thêm thời gian, không cho phép đóng cửa!
    // ==========================================
    if (door.getDoorState() && sensor.isMotionDetected()) {
        door.unlockDoor(); // Việc gọi lại hàm này sẽ Reset bộ đếm lùi về lại 0
    }

    // ==========================================
    // BUOC 2: DOC DU LIEU CAM BIEN
    // (Kich hoat khi Nhom 3 hoan thanh SensorManager)
    // ==========================================
    float temperature = sensor.getTemperature();
    float humidity    = sensor.getHumidity();

    // ==========================================
    // KICH BAN 1: BAO CHAY & QUAT LAM MAT
    // Neu nhiet do > nguong -> kich hoat bao dong va bat quat
    // ==========================================
    if (temperature > TEMP_WARNING_THRESHOLD) {
        digitalWrite(FAN_RELAY_PIN, LOW); // Bật quạt thông gió (Relay kích mức thấp)
        door.unlockDoor();                // Tự động mở cửa thoát hiểm
        
        if (!alert.isCurrentlyAlarming()) {
            alert.triggerFireAlarm();
            display.showMessage("!!! CANH BAO !!!", "NHIET DO CAO!");
            Serial.print("[main] BAO CHAY! Nhiet do: ");
            Serial.println(temperature);
        }
    } else {
        digitalWrite(FAN_RELAY_PIN, HIGH); // Tắt quạt nếu nhiệt độ an toàn
    }

    // ==========================================
    // KICH BAN 2: BAO TROM
    // Cua vat ly bi mo (MC-38=HIGH) nhung Servo chua mo (doorState=false)
    // => Bi cay cua trai phep -> bao dong
    // ==========================================
    if (sensor.isDoorPhysicallyOpen() && !door.getDoorState()) {
        if (!alert.isCurrentlyAlarming()) {
            alert.triggerFireAlarm();
            display.showMessage("!!! BAO TROM !!!", "CUA BI CAY!");
            Serial.println("[main] BAO TROM! Cua bi mo trai phep!");
        }
    }

    // ==========================================
    // KICH BAN 3: PHAN HOI QUET THE RFID
    // Lay ket qua tu DoorSystem, goi AlertManager va DisplayUI tuong ung
    // ==========================================
    int rfidResult = door.getLastScanResult();

    if (rfidResult == RFID_ACCEPTED) {
        alert.stopAlarm(); // Tắt báo động nếu chủ nhà về quẹt thẻ
        alert.beepSuccess();
        Serial.println("[main] The hop le - Cua da mo. Da tat bao dong.");
        display.showMessage("CHAO MUNG!", "CUA DA MO");  // Nhom 4
    }
    else if (rfidResult == RFID_REJECTED) {
        alert.beepFail();
        Serial.println("[main] The khong hop le - Bi tu choi.");
        display.showMessage("THE SAI!", "KHONG HOP LE");  // Nhom 4
    }

    // ==========================================
    // KICH BAN 4: DOC THE RFID LIEN TUC
    // ==========================================
    door.checkRFID();

    // ==========================================
    // KICH BAN 5: GUi DU LIEU LEN WEB MOI 10 GIAY
    // (Kich hoat khi ca Nhom 1 va Nhom 3 hoan thanh)
    // ==========================================
    unsigned long now = millis();
    if (now - lastWebPushTime >= WEB_PUSH_INTERVAL) {
        lastWebPushTime = now;
        sendSensorData(temperature, humidity, 0.0);
        Serial.println("[main] Da gui du lieu len server.");
    }

    // ==========================================
    // KICH BAN 6: CAP NHAT MAN HINH LCD
    // (Kich hoat khi Nhom 3 va Nhom 4 hoan thanh)
    // ==========================================
    if (!alert.isCurrentlyAlarming()) {
        display.updateSensorDisplay(temperature, humidity);
    }
}
