# BẢNG PHÂN CÔNG NHIỆM VỤ ĐỒ ÁN
**Tên đề tài:** Hệ thống Smart Home Cảnh báo Nhiệt độ & Kiểm soát Cửa ra vào (IoT)
**Số lượng thành viên:** 05

---

## MÔ HÌNH PHÂN CÔNG (AGILE / MODULE-BASED)
Dự án được chia thành 5 module độc lập. Mỗi thành viên chịu trách nhiệm từ khâu nghiên cứu, lập trình đến lúc tích hợp để đảm bảo tính chuyên môn hóa cao nhất.

### 👤 Thành viên 1: Trưởng nhóm / Lập trình Hệ thống Lõi (Core Logic)
**Nhiệm vụ chi tiết:**
- **Kiến trúc phần mềm:** Lập trình file `main.cpp`, thiết kế bộ khung xử lý đa tác vụ (Multitasking) không đồng bộ sử dụng hàm `millis()` thay vì `delay()`, đảm bảo ESP32 không bị treo.
- **Hệ thống cảnh báo (AlertManager):** Xây dựng logic cảnh báo cháy nổ. Nháy đèn LED đỏ và kích hoạt còi Buzzer khi nhiệt độ vượt ngưỡng an toàn (35°C).
- **Hệ thống An ninh (DoorSystem):** Tích hợp module RFID RC522, xử lý logic đối chiếu mã thẻ UID, điều khiển Servo mở cửa và tự động khóa lại sau 5 giây.
- **Tích hợp & Debug:** Hỗ trợ các thành viên khác ghép nối code các module lại với nhau, xử lý xung đột bộ nhớ và gỡ lỗi (Debug) toàn hệ thống.

### 👤 Thành viên 2: Kỹ sư Phần cứng & Giao diện nhúng (Hardware & HMI)
**Nhiệm vụ chi tiết:**
- **Thiết kế mạch:** Lên sơ đồ nguyên lý (Schematic), đấu nối dây điện thực tế (Breadboard/PCB) cho ESP32, DHT11, RFID, LCD, Relay, Servo và Buzzer. Đảm bảo nguồn điện 5V/3.3V ổn định.
- **Quản lý Cảm biến (SensorManager):** Lập trình module đọc dữ liệu nhiệt độ, độ ẩm từ DHT11 với thuật toán lọc nhiễu, đảm bảo dữ liệu gửi đi luôn chính xác.
- **Giao diện HMI (DisplayUI):** Lập trình màn hình LCD I2C 16x2. Thiết kế giao diện hiển thị 2 màn hình, sử dụng ngắt phím mềm (Debounce Button) để chuyển đổi giữa màn hình Nhiệt độ và Thông báo trạng thái.

### 👤 Thành viên 3: Kỹ sư Mạng & Giao tiếp (IoT Network Manager)
**Nhiệm vụ chi tiết:**
- **Module Mạng (NetworkManager):** Quản lý kết nối WiFi cho ESP32 với khả năng tự động khôi phục kết nối (Auto-reconnect).
- **Trang Cài đặt cục bộ (Captive Portal):** Tích hợp thư viện `WiFiManager`, biến ESP32 thành bộ phát WiFi độc lập khi mất mạng. Thiết kế giao diện cài đặt ngay trên điện thoại để người dùng nhập "IP Máy tính", loại bỏ hoàn toàn việc Hard-code WiFi vào ROM.
- **Giao thức truyền tải (HTTP Client):** Xây dựng các hàm đóng gói dữ liệu cảm biến (Sensor) và dữ liệu quẹt thẻ (RFID) thành chuẩn JSON / URL-encoded để bắn lên Server thông qua HTTP POST.

### 👤 Thành viên 4: Lập trình viên Backend & Cơ sở dữ liệu (Backend & Database)
**Nhiệm vụ chi tiết:**
- **Thiết kế Cơ sở dữ liệu:** Phân tích và thiết kế cấu trúc Database `iot_db` trên MySQL. Xây dựng các bảng `temperature_logs` và `rfid_logs` với các ràng buộc về kiểu dữ liệu và thời gian (Timestamp).
- **Lập trình API (PHP):** 
  - `save_data.php`: Viết API tiếp nhận dữ liệu POST từ ESP32, kiểm tra bảo mật đầu vào và ghi vào CSDL.
  - `get_data.php`: Viết RESTful API xuất dữ liệu từ Database ra định dạng JSON để phục vụ cho Web Dashboard.
  - `db_connect.php`: Tối ưu hóa file kết nối với khả năng **Tự động khởi tạo Database và Bảng** (Auto-migration) nếu chưa tồn tại.
- **Cấu hình máy chủ:** Setup môi trường XAMPP / PHP Server cục bộ.

### 👤 Thành viên 5: Thiết kế Giao diện Web (Frontend Web Developer)
**Nhiệm vụ chi tiết:**
- **Thiết kế UI/UX (index.php):** Xây dựng bố cục Web Dashboard chuyên nghiệp mang phong cách Dark Mode hiện đại, sử dụng HTML5, CSS3, Google Fonts và FontAwesome.
- **Biểu đồ thời gian thực (chart.js):** Tích hợp thư viện `Chart.js` để vẽ biểu đồ line (đường) thể hiện xu hướng nhiệt độ và độ ẩm.
- **Lập trình bất đồng bộ (AJAX/Fetch):** Viết script JS liên tục gọi API `get_data.php` (mỗi 2 giây) để cập nhật dữ liệu Biểu đồ và thẻ trạng thái (Cards) mà không cần tải lại trang.
- **Tính năng cảnh báo:** Viết logic Frontend để hiển thị Banner màu đỏ và chớp nháy cảnh báo trực quan trên màn hình Web khi nhiệt độ vượt mức nguy hiểm.

---
**Nhận xét chung:** Sự phân công tuân thủ đúng chuẩn công nghiệp của một dự án IoT thực tế (Phần cứng -> Nhúng -> Mạng -> Backend -> Frontend), đảm bảo khối lượng công việc đồng đều và phát huy tối đa thế mạnh của từng thành viên.
