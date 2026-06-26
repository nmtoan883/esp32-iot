#include "wifi_manager.h"
#include "config.h"

#include <WiFi.h>
#include <HTTPClient.h>

// ==========================================
// Cấu hình endpoint server — lấy từ config.h
// SERVER_URL đã được khai báo trong config.cpp
// Tách endpoint save và get từ base URL
// ==========================================

// Ví dụ SERVER_URL = "http://192.168.1.212/Canh_bao_nhiet/web_dashboard/api/save_data.php"
// Ta cũng tạo thêm endpoint get_data dựa trên cùng base path
static String _baseUrl; // Sẽ được khởi tạo trong initWiFi()

// ==========================================
// initWiFi() — Kết nối WiFi lần đầu
// ==========================================
void initWiFi() {
    Serial.println("==========================================");
    Serial.println("[WiFi] Dang ket noi mang...");
    Serial.print("[WiFi] SSID: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int counter = 0;
    while (WiFi.status() != WL_CONNECTED && counter < 20) {
        delay(500);
        Serial.print(".");
        counter++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("[WiFi] Ket noi thanh cong!");
        Serial.print("[WiFi] IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println();
        Serial.println("[WiFi] Ket noi khong thanh cong. He thong se thu lai sau.");
    }
    Serial.println("==========================================");
}


// ==========================================
// maintainWiFi() — Chống rớt mạng (không delay)
// ==========================================
void maintainWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WiFi] Mat song, dang thuc hien ket noi lai...");
        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}


// ==========================================
// sendSensorData() — Gửi cảm biến môi trường
// Dùng: application/x-www-form-urlencoded
// Payload: type=sensor&temperature=...&humidity=...&light=...
// ==========================================
bool sendSensorData(float temp, float humid, float light) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[HTTP] Khong the gui: WiFi chua ket noi.");
        return false;
    }

    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Đóng gói dữ liệu dạng form
    String postData = "type=sensor"
                      "&temperature=" + String(temp, 2) +
                      "&humidity="    + String(humid, 2) +
                      "&light="       + String(light, 2);

    Serial.print("[HTTP] Gui cam bien: ");
    Serial.println(postData);

    int responseCode = http.POST(postData);
    bool success = false;

    if (responseCode > 0) {
        Serial.print("[HTTP] Response Code: ");
        Serial.println(responseCode);
        if (responseCode == 200) {
            Serial.print("[HTTP] Server: ");
            Serial.println(http.getString());
            success = true;
        }
    } else {
        Serial.print("[HTTP] Loi ket noi, code: ");
        Serial.println(responseCode);
    }

    http.end();
    return success;
}


// ==========================================
// sendRFIDLog() — Gửi lịch sử quẹt thẻ
// Dùng: application/x-www-form-urlencoded
// Payload: type=rfid&uid=...
// ==========================================
bool sendRFIDLog(const String& uid) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[HTTP] Khong the gui RFID: WiFi chua ket noi.");
        return false;
    }

    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "type=rfid&uid=" + uid;

    Serial.print("[HTTP] Gui RFID log: ");
    Serial.println(postData);

    int responseCode = http.POST(postData);
    bool success = false;

    if (responseCode > 0) {
        Serial.print("[HTTP] Response Code: ");
        Serial.println(responseCode);
        if (responseCode == 200) {
            Serial.print("[HTTP] Server: ");
            Serial.println(http.getString());
            success = true;
        }
    } else {
        Serial.print("[HTTP] Loi ket noi RFID, code: ");
        Serial.println(responseCode);
    }

    http.end();
    return success;
}


// ==========================================
// fetchServerCommand() — Nhận lệnh từ server
// Dùng HTTP GET tới get_data.php?cmd=1
// Trả về chuỗi JSON từ server, hoặc "" nếu lỗi
// ==========================================
String fetchServerCommand() {
    if (WiFi.status() != WL_CONNECTED) {
        return "";
    }

    // Xây dựng URL get_data từ SERVER_URL
    // SERVER_URL trỏ tới save_data.php → thay bằng get_data.php
    String getUrl = String(SERVER_URL);
    getUrl.replace("save_data.php", "get_data.php");
    getUrl += "?cmd=1";

    HTTPClient http;
    http.begin(getUrl);

    int responseCode = http.GET();
    String result = "";

    if (responseCode > 0) {
        if (responseCode == 200) {
            result = http.getString();
            // Chỉ in nếu có dữ liệu thực sự (không phải mảng rỗng)
            if (result != "[]" && result != "") {
                Serial.print("[HTTP] Nhan lenh tu server: ");
                Serial.println(result);
            }
        }
    } else {
        Serial.print("[HTTP] Loi GET, code: ");
        Serial.println(responseCode);
    }

    http.end();
    return result;
}
