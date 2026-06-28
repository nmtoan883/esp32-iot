#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include <Arduino.h>

// ============================================================
// Nhom 5 (Nguoi 5): Dieu phoi & He thong Canh bao - Core Logic
//
// Huong dan tich hop cho main.cpp:
//   setup() -> alert.init();
//   loop()  -> alert.loop();          // chop den/coi bao dong
//
//   Kich ban quet the dung -> alert.beepSuccess();
//   Kich ban chay / trom   -> alert.triggerFireAlarm();
//   Tat bao dong           -> alert.stopAlarm();
//   Kiem tra trang thai    -> alert.isCurrentlyAlarming();
// ============================================================

class AlertManager {
public:
    // Khoi tao chan GPIO
    void init();

    // Phat tieng "tit - tit" bao quet the dung (blocking ngan ~250ms)
    void beepSuccess();

    // Phat tieng "beep" don bao quet the sai
    void beepFail();

    // Kich hoat che do bao dong (chay / trom)
    void triggerFireAlarm();

    // Tat bao dong, tra ve trang thai binh thuong
    void stopAlarm();

    // Tra ve true neu dang trong trang thai bao dong
    bool isCurrentlyAlarming();

    // Vong lap khong blocking: chop den Do va Coi moi 200ms khi isAlarming
    void loop();

private:
    bool      isAlarming      = false;   // Co bao dong
    bool      alarmToggle     = false;   // Trang thai hien tai cua den/coi (bat/tat)
    unsigned long lastToggleTime = 0;    // Thoi diem lan cuoi toggle

    static const unsigned long ALARM_BLINK_MS = 200; // Chu ky chop tat (ms)
};

#endif
