#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>

// ==========================================
// CLASS: NetworkManager (Nhóm 1)
// Quản lý kết nối WiFi và giao tiếp HTTP API
// ==========================================
class NetworkManager {
public:
    // Khởi tạo kết nối WiFi, chờ cho đến khi kết nối thành công
    void init();

    // Vòng lặp kiểm tra và tự động kết nối lại WiFi (không dùng delay)
    void loop();

    // Gửi dữ liệu nhiệt độ và độ ẩm lên server qua HTTP POST (JSON)
    void sendDataToServer(float temperature, float humidity);

    // Kiểm tra trạng thái kết nối WiFi hiện tại
    bool isConnected();

private:
    // Khoảng thời gian kiểm tra WiFi (10 giây)
    static const unsigned long WIFI_CHECK_INTERVAL = 10000;

    // Lưu thời điểm lần kiểm tra WiFi gần nhất
    unsigned long _lastWifiCheckTime = 0;
};

// Khai báo object toàn cục để các module khác có thể sử dụng
extern NetworkManager networkManager;

#endif
