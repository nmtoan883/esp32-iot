#include "AlertManager.h"
#include "config.h"

// ============================================================
// Nhom 5 - AlertManager.cpp
// Quan ly He thong Canh bao: Coi Buzzer, Den Do, Den Xanh
// Su dung millis() tuyet doi, khong delay() trong loop()
// ============================================================

// ==========================================
// 1. KHOI TAO CHAN GPIO
// ==========================================
void AlertManager::init() {
    pinMode(BUZZER_PIN,    OUTPUT);
    pinMode(LED_RED_PIN,   OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);

    // Trang thai mac dinh: an toan
    digitalWrite(BUZZER_PIN,    LOW);
    digitalWrite(LED_RED_PIN,   LOW);
    digitalWrite(LED_GREEN_PIN, HIGH); // Den xanh sang = an toan

    isAlarming    = false;
    alarmToggle   = false;
    lastToggleTime = 0;

    Serial.println("[AlertManager] Khoi tao thanh cong. Den xanh bat.");
}

// ==========================================
// 2. BEEP THANH CONG (Quet the dung)
// Tin hieu: Bat 100ms - Tat 50ms - Bat 100ms
// Dung delay() ngan (~250ms) - chap nhan duoc cho tinh hieu mot lan
// ==========================================
void AlertManager::beepSuccess() {
    // Khong phat bip khi dang bao dong de tranh xung dot am thanh
    if (isAlarming) return;

    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("[AlertManager] Beep thanh cong - The hop le.");
}

// ==========================================
// 3. BEEP THAT BAI (Quet the sai)
// Tin hieu: Mot tieng dai 500ms
// ==========================================
void AlertManager::beepFail() {
    if (isAlarming) return;

    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("[AlertManager] Beep that bai - The bi tu choi.");
}

// ==========================================
// 4. KICH HOAT BAO DONG
// ==========================================
void AlertManager::triggerFireAlarm() {
    if (isAlarming) return; // Tranh goi nhieu lan

    isAlarming    = true;
    alarmToggle   = false;
    lastToggleTime = millis();

    // Bat ngay lap tuc, khong doi chu ky dau tien
    digitalWrite(LED_RED_PIN,   HIGH);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(BUZZER_PIN,    HIGH);

    Serial.println("[AlertManager] *** BAO DONG KICH HOAT ***");
}

// ==========================================
// 5. TAT BAO DONG - TRA VE TRANG THAI BINH THUONG
// ==========================================
void AlertManager::stopAlarm() {
    isAlarming = false;

    digitalWrite(BUZZER_PIN,    LOW);
    digitalWrite(LED_RED_PIN,   LOW);
    digitalWrite(LED_GREEN_PIN, HIGH); // Den xanh bat lai

    Serial.println("[AlertManager] Bao dong da duoc tat. He thong binh thuong.");
}

// ==========================================
// 6. TRANG THAI BAO DONG
// ==========================================
bool AlertManager::isCurrentlyAlarming() {
    return isAlarming;
}

// ==========================================
// 7. VONG LAP CHINH - KHONG BLOCKING
// Chop den Do + Coi moi 200ms khi dang bao dong
// ==========================================
void AlertManager::loop() {
    if (!isAlarming) return;

    unsigned long now = millis();
    if (now - lastToggleTime >= ALARM_BLINK_MS) {
        lastToggleTime = now;
        alarmToggle    = !alarmToggle;

        digitalWrite(LED_RED_PIN, alarmToggle ? HIGH : LOW);
        digitalWrite(BUZZER_PIN,  alarmToggle ? HIGH : LOW);
        // Den xanh luon tat khi bao dong
        digitalWrite(LED_GREEN_PIN, LOW);
    }
}
