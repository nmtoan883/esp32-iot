#include "config.h"

// Cấu hình Mạng (Cập nhật đúng IP IPv4 của máy tính)
const char* WIFI_SSID = "VITAMIN COFFEE";
const char* WIFI_PASSWORD = "79797979";
const char* SERVER_URL = "http://192.168.1.212/Canh_bao_nhiet/web_dashboard/api/save_data.php"; 

// Mã thẻ Master UID
const String MASTER_RFID_UID_1 = "A1 21 34 16";
const String MASTER_RFID_UID_2 = "01 5C 27 07";

// Ngưỡng nhiệt độ cảnh báo nguy hiểm (38 độ C)
const float TEMP_WARNING_THRESHOLD = 38.0;

// Ngưỡng ánh sáng: LDR cho giá trị ADC cao khi trời tối (0-4095)
const int LIGHT_DARK_THRESHOLD = 2500; 

// Thời gian mở chốt cửa bằng Servo (5 giây)
const unsigned long DOOR_OPEN_TIME = 5000;
