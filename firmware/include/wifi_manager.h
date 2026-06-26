#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

// ==========================================
// WIFI MANAGER — Nhóm 1: Mạng & Giao tiếp API
// Giao diện lập trình cho toàn bộ nhóm
// ==========================================

// Khởi tạo kết nối mạng ban đầu
// Gọi 1 lần trong setup()
void initWiFi();

// Duy trì và tự động kết nối lại khi gặp sự cố mạng
// Gọi liên tục trong loop()
void maintainWiFi();

// Gửi thông số môi trường lên server (Dùng cho Nhóm 3 - Cảm biến)
// temp: nhiệt độ (°C), humid: độ ẩm (%), light: giá trị ADC ánh sáng
// Trả về true nếu gửi thành công
bool sendSensorData(float temp, float humid, float light);

// Gửi lịch sử quẹt thẻ RFID lên server (Dùng cho Nhóm 2 - An ninh)
// uid: chuỗi UID thẻ, ví dụ "A1 21 34 16"
// Trả về true nếu gửi thành công
bool sendRFIDLog(const String& uid);

// Kiểm tra và nhận tín hiệu hoặc lệnh từ server (Dùng cho Nhóm 5 - Core Logic)
// Trả về chuỗi lệnh JSON từ server, hoặc "" nếu không có lệnh / lỗi mạng
String fetchServerCommand();

#endif
