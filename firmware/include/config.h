#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==========================================
// THÔNG TIN MẠNG (THÀNH VIÊN 1)
// ==========================================
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;
extern const char* SERVER_URL;

// ==========================================
// CẤU HÌNH THẺ TỪ & CỬA (THÀNH VIÊN 2)
// ==========================================
extern const String MASTER_RFID_UID_1;
extern const String MASTER_RFID_UID_2;
extern const unsigned long DOOR_OPEN_TIME;

// ==========================================
// CHÂN KẾT NỐI ESP32
// ==========================================
// Cảm biến
#define DHTPIN 4
#define DHTTYPE DHT11
#define PIR_PIN 14
#define LDR_PIN 34
#define DOOR_SENSOR_PIN 25

// RFID RC522 (SPI)
#define SS_PIN 5
#define RST_PIN 15

// Động cơ Servo
#define SERVO_PIN 33

// Hiển thị & Cảnh báo
#define BUZZER_PIN 13
#define LED_RED_PIN 12
#define LED_GREEN_PIN 32
#define LED_ROOM_PIN 17

// Nút nhấn
#define BTN_MODE_PIN 26
#define BTN_DOOR_PIN 27

// ==========================================
// NGƯỠNG CẢNH BÁO (THÀNH VIÊN 3 & 5)
// ==========================================
extern const float TEMP_WARNING_THRESHOLD;
extern const int LIGHT_DARK_THRESHOLD;

#endif
