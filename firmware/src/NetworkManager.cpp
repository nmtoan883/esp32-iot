#include "NetworkManager.h"
#include "config.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ==========================================
// Định nghĩa object toàn cục
// ==========================================
NetworkManager networkManager;


// ==========================================
// init() — Kết nối WiFi lần đầu
// ==========================================
void NetworkManager::init() {
    Serial.println("==========================================");
    Serial.println("[WiFi] Đang kết nối WiFi...");
    Serial.print("[WiFi] SSID: ");
    Serial.println(WIFI_SSID);
    Serial.println("==========================================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Chờ kết nối (blocking chỉ 1 lần khi khởi động)
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        retries++;

        // Sau 30 lần thử (~15 giây) in thông báo thất bại nhưng tiếp tục
        if (retries >= 30) {
            Serial.println();
            Serial.println("[WiFi] Không thể kết nối sau 15 giây. Tiếp tục chạy offline...");
            return;
        }
    }

    Serial.println();
    Serial.println("[WiFi] ✓ Kết nối thành công!");
    Serial.print("[WiFi] Địa chỉ IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("==========================================");
}


// ==========================================
// loop() — Chống rớt mạng (không dùng delay)
// ==========================================
void NetworkManager::loop() {
    unsigned long currentTime = millis();

    // Chỉ kiểm tra mỗi WIFI_CHECK_INTERVAL ms
    if (currentTime - _lastWifiCheckTime >= WIFI_CHECK_INTERVAL) {
        _lastWifiCheckTime = currentTime;

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("[WiFi] ⚠ Mất kết nối! Đang kết nối lại...");
            WiFi.reconnect();
        }
    }
}


// ==========================================
// sendDataToServer() — Gửi JSON lên server
// ==========================================
void NetworkManager::sendDataToServer(float temperature, float humidity) {
    // Kiểm tra WiFi trước khi gửi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[HTTP] ✗ Không thể gửi dữ liệu: WiFi chưa kết nối.");
        return;
    }

    // --- Bước 1: Tạo chuỗi JSON bằng ArduinoJson ---
    // Dùng StaticJsonDocument với dung lượng nhỏ (2 trường số)
    StaticJsonDocument<64> doc;
    doc["temperature"] = temperature;
    doc["humidity"]    = humidity;

    String jsonPayload;
    serializeJson(doc, jsonPayload);

    Serial.print("[HTTP] Gửi dữ liệu lên: ");
    Serial.println(SERVER_URL);
    Serial.print("[HTTP] Payload: ");
    Serial.println(jsonPayload);

    // --- Bước 2: Khởi tạo HTTPClient và gửi POST ---
    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(jsonPayload);

    // --- Bước 3: In kết quả phản hồi ---
    if (httpCode > 0) {
        Serial.print("[HTTP] HTTP Code: ");
        Serial.println(httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
            String response = http.getString();
            Serial.print("[HTTP] ✓ Phản hồi server: ");
            Serial.println(response);
        } else {
            Serial.println("[HTTP] ⚠ Server trả về mã lỗi.");
        }
    } else {
        // httpCode âm: lỗi kết nối, ví dụ -1 = mất mạng
        Serial.print("[HTTP] ✗ Lỗi kết nối. HTTP Code: ");
        Serial.println(httpCode);
        Serial.print("[HTTP] Chi tiết lỗi: ");
        Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end();
}


// ==========================================
// isConnected() — Trả về trạng thái WiFi
// ==========================================
bool NetworkManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
