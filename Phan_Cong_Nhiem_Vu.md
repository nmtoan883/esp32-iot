# 📋 BẢNG PHÂN CÔNG NHIỆM VỤ ĐỒ ÁN SMART HOME (FIRMWARE ESP32)

Dự án này áp dụng mô hình lập trình "Chia để trị". Toàn bộ nhóm sẽ chia nhau viết code cho 5 khối chức năng độc lập trên ESP32.
Dưới đây là mô tả chi tiết **Cần làm gì** và **Phải code ra sao** cho từng chức năng:

---

## 🌐 Nhóm 1: Mạng & Giao tiếp API (Network Communicator)
**Mục tiêu:** ESP32 phải luôn giữ được kết nối WiFi và gửi được dữ liệu cảm biến (JSON) lên Database thông qua XAMPP.

* **File tự viết code:** `firmware/src/NetworkManager.cpp`
* **Yêu cầu triển khai chi tiết:**
  1. **Hàm `init()`:** 
     * Gọi lệnh `WiFi.begin(WIFI_SSID, WIFI_PASSWORD)`.
     * In ra Terminal dòng chữ "Đang kết nối WiFi..." và chờ đến khi `WiFi.status() == WL_CONNECTED` mới đi tiếp. In ra địa chỉ IP sau khi kết nối thành công.
  2. **Hàm `loop()` (Chống rớt mạng):**
     * Dùng `millis()` tạo bộ đếm kiểm tra liên tục. Nếu `WiFi.status() != WL_CONNECTED` thì tự động gọi `WiFi.reconnect()`. *Tuyệt đối không dùng `delay()` làm treo mạch.*
  3. **Hàm `sendDataToServer(nhiệt_độ, độ_ẩm)`:**
     * Bắt buộc dùng thư viện `ArduinoJson` để tạo cục dữ liệu có dạng: `{"temperature": 32.5, "humidity": 60}`.
     * Khởi tạo `HTTPClient`, thiết lập Header là `application/json`.
     * Dùng lệnh `http.POST()` gửi chuỗi JSON lên đường dẫn `SERVER_URL`. 
     * In ra Terminal mã phản hồi (Ví dụ: `HTTP Code: 200` là thành công, `404` là không tìm thấy trang, `-1` là lỗi mất mạng).

---

## 🔐 Nhóm 2: An ninh Ra Vào (Access Control & RFID)
**Mục tiêu:** Nhận diện đúng thẻ Master thì mới cho phép động cơ Servo mở chốt cửa, sau đó tự động đóng chốt lại.

* **File tự viết code:** `firmware/src/DoorSystem.cpp`
* **Yêu cầu triển khai chi tiết:**
  1. **Hàm `init()`:** 
     * Kích hoạt chuẩn SPI và thư viện `MFRC522`. 
     * Cài đặt chân `SERVO_PIN` cho thư viện `ESP32Servo` và cho Servo quay về góc 0 độ (Khóa cửa).
  2. **Hàm `checkRFID()`:**
     * Kiểm tra có thẻ quẹt vào hay không (`PICC_IsNewCardPresent`).
     * Đọc mã UID của thẻ, ghép thành một chuỗi String (Ví dụ: `"A1 21 34 16"`).
     * So sánh chuỗi đó với biến `MASTER_RFID_UID_1` và `MASTER_RFID_UID_2`. Nếu TRÙNG KHỚP -> Gọi hàm `unlockDoor()`. Nếu SAI -> Từ chối.
  3. **Hàm `unlockDoor()` và `lockDoor()`:**
     * `unlockDoor()`: Ghi góc 90 độ cho Servo, bật cờ `doorState = true`, và lưu lại thời điểm `millis()` lúc vừa mở.
     * `lockDoor()`: Ghi góc 0 độ cho Servo, bật cờ `doorState = false`.
  4. **Hàm `loop()` (Tự động khóa cửa):**
     * Kiểm tra nếu `doorState` đang là `true` VÀ `millis()` đã trôi qua quá 5000ms (5 giây) kể từ lúc mở cửa ➔ Gọi hàm `lockDoor()` để cửa tự động đóng lại.

---

## 🌡 Nhóm 3: Thu thập Cảm biến (Environment Data Acquisition)
**Mục tiêu:** Cung cấp thông số Nhiệt độ, Độ ẩm, Ánh sáng, Chuyển động và Trạng thái Cửa một cách liên tục và chính xác nhất.

* **File tự viết code:** `firmware/src/SensorManager.cpp`
* **Yêu cầu triển khai chi tiết:**
  1. **Hàm `loop()` (Cập nhật DHT11):**
     * Dùng `millis()`, cứ mỗi 2000ms (2 giây) thì gọi hàm `dht.readTemperature()` và `dht.readHumidity()`.
     * Cần có lệnh `isnan()` để lọc nhiễu. Nếu dữ liệu bị lỗi (NaN) thì không được ghi đè lên biến lưu trữ cũ.
  2. **Hàm `isDark()`:**
     * Gọi lệnh `analogRead(LDR_PIN)`. Nếu giá trị đo được lớn hơn ngưỡng `LIGHT_DARK_THRESHOLD` (Trời tối đi thì điện trở LDR tăng, ADC tăng) ➔ Trả về `true`, ngược lại `false`.
  3. **Hàm `isMotionDetected()`:**
     * Lấy giá trị `digitalRead(PIR_PIN)`. Cảm biến PIR phát hiện có người đi qua sẽ xuất mức `HIGH`.
  4. **Hàm `isDoorPhysicallyOpen()`:**
     * Cửa từ MC-38 hoạt động theo nguyên lý: Cửa đóng (Nam châm chạm nhau) -> `LOW`. Cửa mở (Nam châm tách ra) -> `HIGH`. Hàm phải đọc chân `DOOR_SENSOR_PIN` và trả về kết quả tương ứng.

---

## 🖥 Nhóm 4: Giao diện (HMI - Human Machine Interface)
**Mục tiêu:** Giao tiếp trực quan với người dùng qua màn hình LCD và đảm bảo nút nhấn không bị chập chờn.

* **File tự viết code:** `firmware/src/DisplayUI.cpp`
* **Yêu cầu triển khai chi tiết:**
  1. **Hàm `updateSensorDisplay(temp, hum)`:**
     * Hàm này được gọi liên tục nhưng chỉ được phép vẽ lại lên màn hình LCD mỗi 2000ms một lần (Dùng `millis()`) để tránh hiện tượng màn hình chớp nháy loạn xạ.
     * Hàng 1: Hiển thị chữ `T: 32.5C  H: 60%`.
  2. **Hàm `showMessage(line1, line2)`:**
     * Có chức năng xóa sạch màn hình (`lcd.clear()`) và in 2 câu thông báo do các Module khác yêu cầu (Ví dụ: Báo cháy, Sai thẻ, v.v...).
  3. **Nút nhấn `isModeButtonPressed()` / `isDoorButtonPressed()`:**
     * Bắt buộc áp dụng thuật toán **Chống dội phím (Debounce)**.
     * Khi nút được nhấn (`LOW`), phải `delay(50)` hoặc dùng `millis()` để đợi phím ổn định cơ học, sau đó mới xác nhận là đã nhấn. Tránh tình trạng 1 lần bấm tay bị mạch tính thành 2-3 lần.

---

## 🧠 Điều phối & Hệ thống Cảnh báo (Core Logic)
**Mục tiêu:** Quản lý logic báo động trung tâm và viết kịch bản ghép nối toàn bộ code của 4 nhóm trên chạy song song trơn tru.

* **File tự viết code:** `firmware/src/AlertManager.cpp` và `firmware/src/main.cpp`
* **Yêu cầu triển khai chi tiết:**
  1. **Quản lý Còi/Đèn (`AlertManager.cpp`):**
     * `beepSuccess()`: Phát tín hiệu còi "Tít - Tít" thật nhanh (Bật 100ms, Tắt 50ms, Bật 100ms) để báo hiệu quẹt thẻ đúng.
     * Vòng lặp `loop()`: Nếu cờ `isAlarming == true` (Đang có báo động), dùng `millis()` tạo hiệu ứng chớp tắt đèn Đỏ và Còi liên tục mỗi 200ms để đinh tai nhức óc.
  2. **Trái tim hệ thống (`main.cpp`):**
     * Trong `setup()`: Gọi toàn bộ các hàm `init()` của 5 module.
     * Trong `loop()`: 
       * Gọi các hàm `loop()` của 5 module để chúng tự chạy ngầm.
       * **Kịch bản Báo Cháy:** Dùng `SensorManager` lấy nhiệt độ, nếu lớn hơn `TEMP_WARNING_THRESHOLD` thì gọi `AlertManager.triggerFireAlarm()`, đồng thời báo chữ "FIRE ALARM" ra màn hình LCD.
       * **Kịch bản Trộm:** Dùng `SensorManager` kiểm tra nếu `isDoorPhysicallyOpen() == true` MÀ cửa lại chưa được `DoorSystem.unlockDoor()` (Bị cạy cửa) ➔ Lập tức hú còi báo động.
       * **Kịch bản Đẩy Web:** Lập bộ đếm `millis()`, cứ đúng 10 giây (10000ms) thì yêu cầu `NetworkManager` đóng gói Nhiệt độ/Độ ẩm gửi lên Web một lần.
