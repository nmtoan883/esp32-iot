#include <Arduino.h>
#include "config.h"
#include "NetworkManager.h"
#include "DoorSystem.h"
#include "SensorManager.h"
#include "DisplayUI.h"
#include "AlertManager.h"

// Khoi tao cac doi tuong dieu khien
NetworkManager network;
DoorSystem door;
SensorManager sensors;
DisplayUI ui;
AlertManager alert;

unsigned long lastWebUploadTime = 0;
const unsigned long webUploadInterval = 10000; // 10 giay gui web mot lan

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("==========================================");
    Serial.println("🏡 HE THONG SMART HOME & CANH BAO KHOI CHAY");
    Serial.println("==========================================");
    
    // Khoi tao cac module
    network.init();
    door.init();
    sensors.init();
    ui.init();
    alert.init();
    
    ui.showMessage("SYSTEM ACTIVE", "WIFI CONNECTED");
    delay(2000);
}

void loop() {
    // Chay loop ngam cua tung module de chung tu dong hoat dong (nhiet do, rfid, nut bam, coi hu)
    network.loop();
    door.loop();
    sensors.loop();
    ui.loop();
    alert.loop();

    // Lay cac gia tri tu cam bien
    float temp = sensors.getTemperature();
    float hum = sensors.getHumidity();

    // Neu khong co bao dong dang hoat dong, hien thi nhiet do, do am len LCD
    if (!alert.getAlarmState()) {
        ui.updateSensorDisplay(temp, hum);
    }

    // 1. Kich ban Quet the tu RFID (DoorSystem tu dong xu ly RFID, ta kiem tra ket qua)
    int rfidResult = door.getLastScanResult();
    if (rfidResult == RFID_ACCEPTED) {
        Serial.println("RFID: The Master dung! Mo cua...");
        alert.beepSuccess();
        ui.showMessage("RFID ACCEPTED", "DOOR UNLOCKED");
    } else if (rfidResult == RFID_REJECTED) {
        Serial.println("RFID: The khong hop le! Canh bao!");
        alert.triggerAlarm();
        ui.showMessage("INVALID CARD!", "ACCESS DENIED");
    }

    // 2. Kich ban Nut bam mo cua (Door Button)
    if (ui.isDoorButtonPressed()) {
        Serial.println("Button: Nut bam mo cua tu phia trong!");
        door.unlockDoor();
        ui.showMessage("DOOR BUTTON", "DOOR UNLOCKED");
    }

    // 3. Kich ban Nut bam Mode (Mode Button de tat bao dong)
    if (ui.isModeButtonPressed()) {
        if (alert.getAlarmState()) {
            Serial.println("Button: Nut Mode reset coi canh bao!");
            alert.stopAlarm();
            ui.showMessage("ALARM CLEARED", "SYSTEM SAFE");
        } else {
            Serial.println("Button: Nut Mode duoc nhan trong che do binh thuong.");
        }
    }

    // 4. Kich ban Bao Chay (Nhiet do vuot qua nguong)
    if (temp >= TEMP_WARNING_THRESHOLD) {
        if (!alert.getAlarmState()) {
            Serial.printf("ALERT: Nhiet do vuot nguong: %.1fC (Nguong: %.1fC)!\n", temp, TEMP_WARNING_THRESHOLD);
            alert.triggerAlarm();
            ui.showMessage("!! FIRE ALARM !!", "TEMP TOO HIGH!");
        }
    }

    // 5. Kich ban Bao Trom (Cay cua: Cua bi mo vat ly MC-38 ma khoa doorState cua Servo van dang khoa)
    if (sensors.isDoorPhysicallyOpen() && !door.getDoorState()) {
        if (!alert.getAlarmState()) {
            Serial.println("ALERT: Cua bi mo trai phep! Co trom!");
            alert.triggerAlarm();
            ui.showMessage("!! INTRUSION !!", "DOOR FORCED OPEN");
        }
    }

    // 6. Bat den phong tu dong: Troi toi + co nguoi di qua (PIR)
    if (sensors.isDark() && sensors.isMotionDetected()) {
        digitalWrite(LED_ROOM_PIN, HIGH);
    } else {
        digitalWrite(LED_ROOM_PIN, LOW);
    }

    // 7. Dong bo gui du lieu len Web database moi 10 giay
    if (millis() - lastWebUploadTime >= webUploadInterval) {
        lastWebUploadTime = millis();
        // Chi gui khi co thong so hop le doc tu DHT11
        if (temp > 0.0 || hum > 0.0) {
            Serial.printf("Server Push: Nhiet do: %.1fC, Do am: %.1f%%\n", temp, hum);
            network.sendDataToServer(temp, hum);
        }
    }
}

