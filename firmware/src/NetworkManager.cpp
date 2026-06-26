#include "NetworkManager.h"
#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void NetworkManager::init() {
    Serial.println("NetworkManager: Dang ket noi WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    // Cho den khi ket noi thanh cong
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("NetworkManager: WiFi da ket noi! IP: ");
    Serial.println(WiFi.localIP());
}

void NetworkManager::loop() {
    // Chong rot mang: Kiem tra moi 5 giay khong gay block loop
    if (millis() - lastConnectionCheck >= connectionCheckInterval) {
        lastConnectionCheck = millis();
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("NetworkManager: Mat ket noi WiFi! Dang ket noi lai...");
            WiFi.disconnect();
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        }
    }
}

void NetworkManager::sendDataToServer(float temp, float hum) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("NetworkManager: Khong the gui du lieu, WiFi chua ket noi!");
        return;
    }

    HTTPClient http;
    
    // Khoi tao ket noi HTTP toi SERVER_URL
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    // Dong goi JSON bang ArduinoJson v7
    JsonDocument doc;
    doc["temperature"] = temp;
    doc["humidity"] = hum;

    String jsonStr;
    serializeJson(doc, jsonStr);

    Serial.print("NetworkManager: Gui JSON: ");
    Serial.println(jsonStr);

    // Thuc hien HTTP POST
    int httpResponseCode = http.POST(jsonStr);

    if (httpResponseCode > 0) {
        Serial.print("NetworkManager: HTTP Response Code: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.print("NetworkManager: Server Response: ");
        Serial.println(response);
    } else {
        Serial.print("NetworkManager: Loi gui HTTP POST, Response Code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

