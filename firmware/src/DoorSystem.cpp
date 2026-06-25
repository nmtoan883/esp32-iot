#include "DoorSystem.h"
#include "config.h"

#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

// Chi su dung cac chan thuoc Nhom 2: SS_PIN, RST_PIN, SERVO_PIN (config.h)
// Khong dung SPI/I2C/GPIO cua Nhom khac de tranh xung dot phan cung
static MFRC522 mfrc522(SS_PIN, RST_PIN);
static Servo doorServo;

// ==========================================
// 1. KHOI TAO SPI, MFRC522 VA ESP32Servo
// ==========================================
void DoorSystem::init() {
    doorState = false;
    doorOpenTime = 0;
    lastScanResult = RFID_NO_CARD;

    SPI.begin();
    mfrc522.PCD_Init();

    doorServo.setPeriodHertz(50);
    doorServo.attach(SERVO_PIN, 500, 2400);
    doorServo.write(0);
}

// ==========================================
// 2. QUET THE RFID VA KIEM TRA THE MASTER
// ==========================================
void DoorSystem::checkRFID() {
    lastScanResult = RFID_NO_CARD;

    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (i > 0) {
            uid += " ";
        }
        if (mfrc522.uid.uidByte[i] < 0x10) {
            uid += "0";
        }
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    if (uid == MASTER_RFID_UID_1 || uid == MASTER_RFID_UID_2) {
        lastScanResult = RFID_ACCEPTED;
        unlockDoor();
    } else {
        lastScanResult = RFID_REJECTED;
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}

// ==========================================
// 3. MO / KHOA CUA BANG SERVO
// ==========================================
void DoorSystem::unlockDoor() {
    doorServo.write(90);
    doorState = true;
    doorOpenTime = millis();
}

void DoorSystem::lockDoor() {
    doorServo.write(0);
    doorState = false;
}

bool DoorSystem::getDoorState() {
    return doorState;
}

int DoorSystem::getLastScanResult() {
    return lastScanResult;
}

// ==========================================
// 4. TU DONG KHOA CUA SAU 5 GIAY
// ==========================================
void DoorSystem::loop() {
    if (doorState && (millis() - doorOpenTime >= DOOR_OPEN_TIME)) {
        lockDoor();
    }
}
