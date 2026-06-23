# 🏡 Đồ án Smart Home & Hệ thống Cảnh báo Nhiệt độ (Version Teamwork)

Dự án này là mã nguồn chia sẻ dành cho nhóm 5 người. Hệ thống được cấu trúc theo mô hình **Modular Programming (C++)**, mỗi tính năng là một cụm file `.h` và `.cpp` độc lập.

---

## 🛠 1. HƯỚNG DẪN CÀI ĐẶT MÔI TRƯỜNG (BẮT BUỘC)

Để code và chạy được dự án này, toàn bộ 5 thành viên trong nhóm **bắt buộc** phải cài đặt các phần mềm sau trên máy tính cá nhân:

### 1.1. Cài đặt Trình soạn thảo & Biên dịch (VS Code + PlatformIO)
1. Tải và cài đặt **[Visual Studio Code](https://code.visualstudio.com/)**.
2. Mở VS Code, chọn biểu tượng **Extensions** (Cục xếp hình vuông ở bên trái).
3. Gõ tìm kiếm và cài đặt Extension có tên: **`PlatformIO IDE`** (Biểu tượng cái đầu kiến). Chờ nó tự động tải các gói phụ trợ (sẽ hơi lâu).
4. Khởi động lại VS Code sau khi cài xong.

### 1.2. Cài đặt Driver Nạp Code cho mạch ESP32
* Tùy thuộc vào loại mạch ESP32 nhóm đang dùng, hãy tải driver tương ứng:
  * Nếu chip USB hình chữ nhật nhỏ: Cài driver **`CP210x`**.
  * Nếu chip USB hình chữ nhật dài: Cài driver **`CH340`**.
* Nếu cắm mạch vào máy tính mà PlatformIO không nhận cổng `COM`, nghĩa là bạn chưa cài đúng driver.

### 1.3. Cài đặt Máy chủ Web Local (XAMPP)
*(Đặc biệt dành cho người test và chạy thử trang Web)*
1. Tải và cài đặt phần mềm **[XAMPP](https://www.apachefriends.org/download.html)**.
2. Mở XAMPP Control Panel, bấm nút **Start** ở 2 dòng: `Apache` và `MySQL`.

---

## 📚 2. DANH SÁCH THƯ VIỆN CẦN CÀI ĐẶT (LIBRARIES)

Dự án này sử dụng các thư viện bên thứ 3. Nếu bạn mở dự án bằng PlatformIO (mở thư mục `firmware`), PlatformIO sẽ **tự động tải** các thư viện này dựa trên file `platformio.ini`. 

Tuy nhiên, nếu nhóm xài Arduino IDE hoặc cần cài thủ công, đây là danh sách chính xác các thư viện phải có:
1. **`DHT sensor library`** (của Adafruit) - Đọc cảm biến nhiệt độ DHT11.
2. **`Adafruit Unified Sensor`** (của Adafruit) - Thư viện nền tảng bắt buộc đi kèm với DHT.
3. **`MFRC522`** (của GithubCommunity) - Thư viện đọc thẻ từ RFID qua chuẩn SPI.
4. **`ESP32Servo`** (của Kevin Harrington) - Thư viện chuyên biệt để điều khiển động cơ Servo trên chip ESP32 (Không dùng thư viện Servo mặc định của Arduino).
5. **`LiquidCrystal_I2C`** (của marcoschwartz) - Thư viện điều khiển màn hình LCD qua chuẩn I2C.
6. **`ArduinoJson`** (của Benoit Blanchon) - Dùng để đóng gói dữ liệu thành JSON trước khi gửi lên Web.

---

## 🔌 3. BẢNG THÔNG SỐ CÁC CHÂN ESP32 (PINOUT & WIRING)

**LƯU Ý CỰC KỲ QUAN TRỌNG CHO TEAM CODE:** 
Đây là bảng phần cứng ĐÃ CHỐT. Các thành viên dựa đúng vào bảng này để code khai báo chân (`pinMode`), tránh việc người này code chân 4 mà người kia lại cắm chân 5.

| Tên Thiết Bị / Linh Kiện | Chân trên Linh Kiện | Cắm vào chân ESP32 | Loại Tín Hiệu (Dành cho Coder) |
| :--- | :--- | :--- | :--- |
| **Cảm biến Nhiệt Ẩm (DHT11)** | DATA | `D4` | Kỹ thuật số (Thư viện DHT tự lo) |
| **Cảm biến Ánh sáng (LDR)** | AO (Analog Out) | `D34` | **Analog Input** (Dùng `analogRead()`) |
| **Cảm biến Chuyển động (PIR)**| OUT | `D14` | **Digital Input** (`HIGH` = Có người) |
| **Cửa từ (MC-38)** | Dây tín hiệu | `D25` | **Digital Input** (`HIGH` = Cửa bị mở) |
| **Nút nhấn MODE** | Chân tín hiệu | `D26` | **Digital Input PULLUP** (`LOW` = Bấm) |
| **Nút nhấn DOOR** | Chân tín hiệu | `D27` | **Digital Input PULLUP** (`LOW` = Bấm) |
| **Còi Báo động (Buzzer)** | Chân Dương (+) | `D13` | **Digital Output** (`HIGH` = Kêu) |
| **Đèn LED Đỏ (Cảnh báo)** | Chân Dương (+) | `D12` | **Digital Output** (*Rút dây ra khi nạp code*) |
| **Đèn LED Xanh (An toàn)** | Chân Dương (+) | `D32` | **Digital Output** |
| **Màn hình LCD 1602 I2C** | SDA, SCL | `D21` (SDA), `D22` (SCL) | Giao tiếp I2C (Thư viện tự lo) |
| **Động cơ Servo (Cửa)** | Dây Cam (Signal) | `D33` | Băm xung PWM (Thư viện tự lo) |
| **Đầu đọc Thẻ từ (RFID)** | SDA (SS) | `D5` | Giao tiếp SPI (*Rút dây ra khi nạp code*) |
| | RST | `D15` | Giao tiếp SPI (*Rút dây ra khi nạp code*) |
| | SCK, MOSI, MISO | `D18`, `D23`, `D19`| Các chân SPI mặc định của mạch ESP32 |

> ⚠️ **LƯU Ý NGUỒN ĐIỆN ĐẤU NỐI:** 
> * Mạch RFID RC522 **bắt buộc** dùng nguồn **3.3V** (Chân 3V3). Cắm 5V sẽ cháy mạch.
> * Các linh kiện còn lại (LCD, PIR, Buzzer) xài nguồn **5V** (Chân VIN).
> * Khi nạp code, bắt buộc phải rút 3 dây tín hiệu ở các chân: **D12, D5, D15** ra. Đợi báo *Connecting...* thì nhấn giữ nút `BOOT`. Nạp xong cắm 3 dây lại và nhấn nút `EN` để mạch chạy.

---

## 📌 4. PHÂN CÔNG NHIỆM VỤ THÀNH VIÊN
Để biết chi tiết ai đảm nhận viết File nào và code chức năng gì, vui lòng đọc kỹ file: 
👉 **[Phan_Cong_Nhiem_Vu.md](Phan_Cong_Nhiem_Vu.md)** đính kèm trong dự án này.
