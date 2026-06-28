#include "NetworkManager.h"
#include "config.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiManager.h>
#include <Preferences.h>

// ==========================================
// Cấu hình endpoint server
// ==========================================
static String _baseUrl; 
Preferences preferences;

// ==========================================
// initWiFi() — Kết nối WiFi qua Captive Portal
// ==========================================
void initWiFi() {
    Serial.println("==========================================");
    Serial.println("[WiFi] Khoi tao WiFiManager...");

    WiFiManager wm;
    
    // Mở bộ nhớ để lấy URL đã lưu
    preferences.begin("smarthome", false);
    String savedUrl = preferences.getString("server_url", "http://192.168.1.xxx/Canh_bao_nhiet/web_dashboard/api/save_data.php");
    
    // Tạo ô nhập liệu cho SERVER_URL
    WiFiManagerParameter custom_server_url("server_url", "Server URL (API save_data.php)", savedUrl.c_str(), 100);
    wm.addParameter(&custom_server_url);

    // Cài đặt giao diện
    wm.setConfigPortalTimeout(180); // Chờ 3 phút, nếu không ai kết nối thì tự khởi động lại hoặc thử lại
    
    // Bắt đầu phát WiFi Setup
    if (!wm.autoConnect("SmartHome_Setup")) {
        Serial.println("[WiFi] Khong the ket noi. Tu dong khoi dong lai sau 3s...");
        delay(3000);
        ESP.restart();
    }

    // Đã kết nối thành công! Lấy URL mới nhất do người dùng nhập (nếu có)
    _baseUrl = String(custom_server_url.getValue());
    
    // Lưu URL mới vào bộ nhớ
    preferences.putString("server_url", _baseUrl);
    preferences.end();

    Serial.println();
    Serial.println("[WiFi] Ket noi thanh cong!");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WiFi] Server URL: ");
    Serial.println(_baseUrl);
    Serial.println("==========================================");
}

// ==========================================
// maintainWiFi() — Tự động xử lý nếu rớt mạng
// ==========================================
void maintainWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WiFi] Rot mang! Dang thu ket noi lai...");
        // WiFiManager tự động lo việc kết nối lại dưới nền của ESP32, 
        // nhưng nếu mất mạng quá lâu ta có thể gọi ESP.restart()
        delay(1000);
    }
}

// ==========================================
// resetWiFi() — Xóa dữ liệu WiFi và URL
// ==========================================
void resetWiFi() {
    Serial.println("[WiFi] XOA TOAN BO CAU HINH WIFI VA KHOI DONG LAI...");
    WiFiManager wm;
    wm.resetSettings(); // Xóa WiFi
    
    preferences.begin("smarthome", false);
    preferences.remove("server_url"); // Xóa URL
    preferences.end();
    
    delay(1000);
    ESP.restart();
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
    http.begin(_baseUrl);
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
    http.begin(_baseUrl);
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
    String getUrl = _baseUrl;
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
