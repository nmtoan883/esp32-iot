# 🏡 Đồ án Smart Home & Hệ thống Cảnh báo Nhiệt độ (Version Hoàn thiện)

Dự án này là phiên bản chính thức, được tái cấu trúc theo mô hình **Modular Programming (C++)** để phù hợp làm việc nhóm 5 người. Toàn bộ tính năng đã được kiểm chứng hoạt động ổn định trên vi điều khiển ESP32.

---

## 🛠 Yêu cầu Cài đặt (Prerequisites)

Để code và chạy được dự án này, mỗi thành viên cần cài đặt:

1.  **Phần mềm Code:**
    *   Tải và cài đặt **Visual Studio Code (VS Code)**.
    *   Vào mục Extensions trong VS Code, cài đặt **PlatformIO IDE**.
2.  **Phần mềm Web Server & Database:**
    *   Cài đặt **XAMPP**. Bật module `Apache` và `MySQL`.
3.  **Driver Nạp Code:**
    *   Tải và cài driver USB cho mạch ESP32 (Thường là `CH340` hoặc `CP210x` tuỳ chip).

---

## 🔌 Bảng Sơ đồ Đấu nối Chân (Pinout Wiring)

Bảng này liệt kê chính xác các chân cắm đã được kiểm tra trên thực tế. **Lưu ý: Không dùng Adapter 5V riêng, mọi linh kiện dùng điện trực tiếp từ cổng USB của ESP32 thông qua chân `VIN` và `3V3`.**

| Khối Chức năng | Tên Linh kiện | Chân Linh kiện | ➔ Cắm vào ESP32 | Ghi chú quan trọng |
| :--- | :--- | :--- | :--- | :--- |
| **Năng lượng** | ESP32 | Cáp USB | ➔ Máy tính | Dùng làm nguồn chính |
| **Hiển thị** | LCD 1602 (I2C) | VCC, GND, SDA, SCL | ➔ `VIN`, `GND`, `D21`, `D22` | LCD dùng điện 5V từ VIN |
| **Bảo mật** | Thẻ từ RFID RC522| 3.3V, GND, RST | ➔ **`3V3`**, `GND`, **`D15`** | **Tuyệt đối không cắm VIN** |
| | | MISO, MOSI, SCK, SDA | ➔ `D19`, `D23`, `D18`, **`D5`** | *Rút D5, D15 khi nạp code* |
| **Cảm biến** | Chuyển động PIR | VCC, GND, OUT | ➔ `VIN`, `GND`, `D14` | PIR nhận diện 5m |
| | Nhiệt ẩm DHT11 | VCC, GND, DATA | ➔ `3V3`, `GND`, `D4` | |
| | Ánh sáng LDR | VCC, GND, AO | ➔ `3V3`, `GND`, `D34` | |
| | Cửa từ MC-38 | 2 Dây không phân cực | ➔ `GND`, `D25` | |
| **Giao tiếp** | Nút Mode | Chân 1, Chân 2 | ➔ `GND`, `D26` | Đã cấu hình INPUT_PULLUP |
| | Nút Door | Chân 1, Chân 2 | ➔ `GND`, `D27` | Đã cấu hình INPUT_PULLUP |
| **Báo động** | Còi Buzzer | Dài (+), Ngắn (-) | ➔ `D13`, `GND` | |
| | Đèn LED Đỏ | Dài (+), Ngắn (-) | ➔ **`D12`**, `GND` | *Rút D12 khi nạp code* |

---

## 👥 Bảng Phân công Nhiệm vụ (Team 5 Người)

Toàn bộ 5 thành viên đều trực tiếp lập trình C++ trên vi điều khiển ESP32. Mã nguồn được chia làm 5 phân hệ (Module) để tránh xung đột mã (conflict) khi ghép chung vào thư mục `src/`.

### 🧑‍💻 Thành viên 1: Network & Web Communicator (Mạng & Giao tiếp Web)
*   **Module phụ trách:** `NetworkManager.h` & `NetworkManager.cpp`
*   **Mô tả Công việc:** 
    *   Lập trình kết nối WiFi sử dụng thư viện `<WiFi.h>`. Tự động cấu hình kết nối lại nếu mất mạng.
    *   Sử dụng `<HTTPClient.h>` để mở cổng POST tới Web Server (XAMPP).
    *   Sử dụng thư viện `ArduinoJson` để đóng gói dữ liệu của Thành viên 3 (Nhiệt độ, Độ ẩm) thành chuỗi JSON và gửi lên Web. Xử lý các mã lỗi (Error -1, 404).

### 🧑‍💻 Thành viên 2: Access Control & RFID (An ninh Ra Vào)
*   **Module phụ trách:** `DoorSystem.h` & `DoorSystem.cpp`
*   **Mô tả Công việc:** 
    *   Cài đặt thư viện `<MFRC522.h>` để giao tiếp qua chuẩn SPI với mắt đọc thẻ từ.
    *   Quản lý logic nhận diện Thẻ Chủ (Master Card UID).
    *   Cài đặt thư viện `<ESP32Servo.h>`. Lập trình băm xung PWM điều khiển động cơ Servo SG90 quay 90 độ để mở chốt cửa, đếm ngược 5 giây rồi tự động quay về 0 độ để đóng lại.

### 🧑‍💻 Thành viên 3: Environment Data Acquisition (Thu thập Cảm biến)
*   **Module phụ trách:** `SensorManager.h` & `SensorManager.cpp`
*   **Mô tả Công việc:** 
    *   Sử dụng thư viện `<DHT.h>` đọc chỉ số Môi trường (Nhiệt độ, Độ ẩm) theo chu kỳ.
    *   Lập trình đọc tín hiệu ADC 12-bit từ cảm biến quang trở (LDR) để phân biệt Ngày/Đêm.
    *   Lập trình quét trạng thái (Polling) hoặc Ngắt (Interrupt) để phát hiện người từ cảm biến PIR và trạng thái mở cửa vật lý từ cảm biến MC-38. Lọc nhiễu dữ liệu.

### 🧑‍💻 Thành viên 4: HMI - Human Machine Interface (Giao tiếp Người-Máy)
*   **Module phụ trách:** `DisplayUI.h` & `DisplayUI.cpp`
*   **Mô tả Công việc:** 
    *   Sử dụng `<LiquidCrystal_I2C.h>` để in thông báo lên Màn hình LCD 1602.
    *   Viết các hàm hiển thị động: Màn hình chào mừng, Nhiệt độ thực tế, Trạng thái Khóa cửa (LOCKED/UNLOCKED), Thông báo Quẹt thẻ Sai/Đúng.
    *   Lập trình thuật toán chống dội (Debounce Timer) cho cụm nút nhấn vật lý (Mode, Door) để tránh việc tín hiệu chập chờn gây nhấn 1 thành nhấn 2 lần.

### 🧑‍💻 Thành viên 5: Core Logic & Alert System (Trưởng Nhóm - Lắp ghép)
*   **Module phụ trách:** `AlertManager.h`, `AlertManager.cpp` & `main.cpp`
*   **Mô tả Công việc:** 
    *   Lập trình tín hiệu PWM xuất ra còi Buzzer: Tạo giai điệu "tít tít" 2 tiếng khi quẹt đúng thẻ, và tiếng hú kéo dài liên tục khi phát hiện Nhiệt độ vượt ngưỡng hoặc Cửa bị phá (MC-38 hở mà chưa quẹt thẻ).
    *   **Lắp ghép hệ thống (`main.cpp`):** Khởi tạo tất cả 4 Module trên. Sử dụng thuật toán Non-Blocking (dùng hàm `millis()` thay vì hàm `delay()`) ở vòng lặp siêu tốc `loop()` để đảm bảo ESP32 có thể vừa hú còi, vừa mở cửa, vừa gửi Web đồng thời mà không bị treo máy.
