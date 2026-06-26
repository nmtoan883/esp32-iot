#include "AlertManager.h"
#include "config.h"

void AlertManager::init() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_ROOM_PIN, OUTPUT);
    
    // Trang thai an toan ban dau
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, HIGH); // LED xanh sang
    digitalWrite(LED_ROOM_PIN, LOW);
    
    isAlarming = false;
    alarmState = false;
    lastAlarmToggleTime = 0;
}

void AlertManager::loop() {
    // Neu dang bao dong, chop tat LED do va coi moi 200ms
    if (isAlarming) {
        if (millis() - lastAlarmToggleTime >= 200) {
            lastAlarmToggleTime = millis();
            alarmState = !alarmState;
            digitalWrite(LED_RED_PIN, alarmState ? HIGH : LOW);
            digitalWrite(BUZZER_PIN, alarmState ? HIGH : LOW);
        }
    }
}

void AlertManager::beepSuccess() {
    // Tit - Tit nhanh
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
}

void AlertManager::triggerAlarm() {
    if (!isAlarming) {
        isAlarming = true;
        alarmState = true;
        lastAlarmToggleTime = millis();
        
        digitalWrite(LED_GREEN_PIN, LOW); // Tat LED xanh
        digitalWrite(LED_RED_PIN, HIGH);  // Bat LED do
        digitalWrite(BUZZER_PIN, HIGH);   // Bat coi
    }
}

void AlertManager::stopAlarm() {
    if (isAlarming) {
        isAlarming = false;
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_GREEN_PIN, HIGH); // Bat lai LED xanh an toan
    }
}

bool AlertManager::getAlarmState() {
    return isAlarming;
}

