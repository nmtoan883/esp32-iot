# 📋 BẢNG PHÂN CÔNG NHIỆM VỤ ĐỒ ÁN SMART HOME (FIRMWARE ESP32)

Dự án này được áp dụng mô hình lập trình hướng đối tượng (OOP) và chia nhỏ thành các Module (C++) độc lập. 
**Yêu cầu bắt buộc:** Cả 5 thành viên đều phải tham gia viết code C++ trên mạch ESP32.

Dưới đây là bảng phân công chi tiết cho từng thành viên:

---

## 🌐 Nhóm Mạng & Giao tiếp API (Network Communicator)
**Nhiệm vụ chính:** Đảm bảo mạch ESP32 luôn có Internet và gửi được dữ liệu lên trang Web thành công.

* **File phụ trách (Tự viết code vào đây):** 
  * `firmware/include/NetworkManager.h`
  * `firmware/src/NetworkManager.cpp`
* **Chức năng cần code:**
  1. Khai báo thư viện `<WiFi.h>` và `<HTTPClient.h>`.
  2. Viết hàm kết nối WiFi. Nếu WiFi rớt, phải viết logic tự động kết nối lại (dùng millis, không dùng delay).
  3. Cài đặt thư viện `ArduinoJson` để gom dữ liệu Nhiệt độ, Độ ẩm thành một chuỗi JSON.
  4. Viết hàm bắn lệnh `HTTP POST` để đẩy chuỗi JSON đó lên đường dẫn `SERVER_URL`. Xử lý và in ra màn hình Terminal nếu mạng bị lỗi (Error 404, -1).

---

## 🔐 Nhóm An ninh Ra Vào (Access Control & RFID)
**Nhiệm vụ chính:** Quản lý logic đọc thẻ từ và điều khiển khóa cửa (Servo).

* **File phụ trách (Tự viết code vào đây):** 
  * `firmware/include/DoorSystem.h`
  * `firmware/src/DoorSystem.cpp`
* **Chức năng cần code:**
  1. Sử dụng thư viện `<MFRC522.h>` (giao tiếp SPI) để quét mã thẻ (UID).
  2. Viết thuật toán so sánh chuỗi: Nếu thẻ quét vào trùng với `MASTER_RFID_UID_1` hoặc `MASTER_RFID_UID_2` thì cho phép mở cửa. Nếu sai thì từ chối.
  3. Sử dụng thư viện `<ESP32Servo.h>`. Nếu thẻ đúng, xuất tín hiệu PWM quay Servo lên 90 độ (Mở chốt).
  4. Viết bộ đếm ngược (dùng millis). Đúng 5 giây sau khi mở, tự động quay Servo về 0 độ (Khóa chốt).

---

## 🌡 Nhóm Thu thập Cảm biến (Environment Data Acquisition)
**Nhiệm vụ chính:** Đọc và làm sạch dữ liệu từ toàn bộ các cảm biến gắn trên mạch.

* **File phụ trách (Tự viết code vào đây):** 
  * `firmware/include/SensorManager.h`
  * `firmware/src/SensorManager.cpp`
* **Chức năng cần code:**
  1. Khai báo và sử dụng thư viện `<DHT.h>`. Viết hàm đọc Nhiệt độ (C) và Độ ẩm (%) mỗi 2 giây. Cần có code bắt lỗi (báo lỗi `NaN`) nếu cảm biến bị lỏng dây.
  2. Dùng hàm `analogRead()` đọc tín hiệu Cảm biến Ánh sáng (LDR). Viết logic: Nếu thông số ADC lớn hơn `LIGHT_DARK_THRESHOLD` thì kết luận là Trời Tối.
  3. Dùng hàm `digitalRead()` đọc tín hiệu Cảm biến Chuyển động (PIR).
  4. Dùng hàm `digitalRead()` đọc trạng thái Cửa từ (MC-38) để biết cửa vật lý đang đóng hay đang hé mở.

---

## 🖥 Nhóm Giao diện (HMI - Human Machine Interface)
**Nhiệm vụ chính:** Giao tiếp với người dùng qua màn hình LCD và nút nhấn cứng.

* **File phụ trách (Tự viết code vào đây):** 
  * `firmware/include/DisplayUI.h`
  * `firmware/src/DisplayUI.cpp`
* **Chức năng cần code:**
  1. Sử dụng thư viện `<LiquidCrystal_I2C.h>`. Khởi tạo màn hình I2C địa chỉ `0x27`.
  2. Viết các hàm in thông báo lên màn hình (Ví dụ in 2 dòng: Dòng 1 ghi Nhiệt độ, Dòng 2 ghi "Door LOCKED").
  3. Xóa và cập nhật màn hình mượt mà không bị giật (flicker).
  4. Viết code xử lý 2 nút nhấn vật lý (`BTN_MODE_PIN` và `BTN_DOOR_PIN`). Bắt buộc phải viết thuật toán **Chống dội phím (Debounce)** để tránh tình trạng bấm 1 cái mạch đếm thành bấm 2 cái.

---

## 🧠 Nhóm Trưởng: Điều phối & Hệ thống Cảnh báo (Core Logic)
**Nhiệm vụ chính:** Quản lý hệ thống cảnh báo (Còi/Đèn) và là người ghép nối code của các module kia lại với nhau để mạch chạy hoàn chỉnh.

* **File phụ trách (Tự viết code vào đây):** 
  * `firmware/include/AlertManager.h`
  * `firmware/src/AlertManager.cpp`
  * `firmware/src/main.cpp`
* **Chức năng cần code:**
  1. Viết code băm xung bật/tắt Còi (Buzzer) và các đèn LED.
  2. Viết hàm tạo nhịp còi: Quẹt đúng thẻ thì kêu "Tít Tít" nhanh. Khi báo cháy thì hú dài liên tục và đèn LED đỏ chớp tắt liên tục (dùng millis).
  3. **File ghép nối (`main.cpp`):** Gọi hàm `init()` của tất cả các module. Ở trong vòng `loop()`, viết logic tổng:
     * *Logic Báo cháy:* Lấy nhiệt độ từ code của (Nhóm Cảm biến). Nếu lớn hơn `TEMP_WARNING_THRESHOLD` -> Ra lệnh hú còi.
     * *Logic Trộm:* Lấy trạng thái từ cảm biến MC-38, nếu cửa mở mà không có lệnh từ thẻ từ -> Kích hoạt còi hú.
     * *Logic Gửi Web:* Cứ mỗi 10 giây, lấy số liệu của (Nhóm Cảm biến) quăng cho (Nhóm Mạng) để gửi lên Web.

---

**🎯 Chúc toàn team hoàn thành xuất sắc đồ án! Nhớ tuân thủ đúng file của người nào người nấy code để lúc ráp code (merge) không bị lỗi nhé!**
