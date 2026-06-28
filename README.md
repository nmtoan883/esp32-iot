# 🏡 Smart Home IoT System - Cảnh báo Nhiệt độ & Kiểm soát Cửa

Chào mừng bạn đến với dự án **Smart Home IoT**. Dự án này là sự kết hợp hoàn hảo giữa phần cứng nhúng (ESP32) và hệ thống Web Server nội bộ để giám sát nhiệt độ và lịch sử ra vào cửa bằng thẻ từ.

---

## 🚀 Hướng dẫn Cài đặt & Khởi động nhanh (Quick Start)

Hệ thống được thiết kế theo cơ chế **Tự động hóa (Auto-Migration)**, bạn không cần phải thao tác tạo Database bằng tay trong phpMyAdmin. Chỉ cần làm theo đúng 4 bước dưới đây để hệ thống chạy 100%.

### Bước 1: Khởi động Cơ sở dữ liệu (Database)
1. Mở phần mềm **XAMPP Control Panel**.
2. Nhấn nút **Start** ở hàng **MySQL** (đảm bảo nó chuyển sang màu xanh lá cây).
*(Lưu ý: Không cần bật Apache vì chúng ta sẽ tự chạy Server riêng cho an toàn).*

### Bước 2: Bật Web Server (Port 8000)
Hệ thống sử dụng cổng `8000` để tránh xung đột với các phần mềm khác.
1. Mở Terminal hoặc Command Prompt.
2. Di chuyển vào thư mục chứa mã nguồn Web:
   ```bash
   cd "D:\Canh bao nhiet\SmartHome_Final\web_dashboard"
   ```
3. Khởi chạy PHP Server cục bộ bằng lệnh sau (đường dẫn `php.exe` phụ thuộc vào nơi bạn cài XAMPP):
   ```bash
   D:\xampp\php\php.exe -S 0.0.0.0:8000
   ```
   *Nếu bạn cài XAMPP ở ổ C, hãy dùng lệnh `C:\xampp\php\php.exe -S 0.0.0.0:8000`.*
4. Sau khi thấy thông báo `Listening on http://0.0.0.0:8000`, **hãy treo nguyên cửa sổ Terminal này** (không được tắt).

### Bước 3: Nạp Code cho mạch ESP32
1. Sử dụng VS Code (đã cài PlatformIO).
2. Kết nối mạch ESP32 vào máy tính qua cáp USB.
3. Mở một Terminal mới (bấm dấu `+`) và chạy lệnh sau để nạp Code:
   ```bash
   cd "D:\Canh bao nhiet\SmartHome_Final\firmware"
   C:\Users\nmtoa\.platformio\penv\Scripts\pio.exe run -t upload
   ```
   *(Nhớ nhấn giữ nút BOOT trên mạch nếu màn hình hiện `Connecting...`)*

### Bước 4: Xem kết quả trên Web Dashboard
1. Mở trình duyệt Web (Chrome / Edge / Safari).
2. Truy cập vào địa chỉ: **http://localhost:8000/**
3. Ngay lập tức, bạn sẽ thấy Giao diện Giám sát (Dashboard) hiện ra. Database `iot_db` sẽ được tự động tạo ngầm nhờ vào script Auto-Migration. Dữ liệu nhiệt độ sẽ hiển thị theo thời gian thực!

---

## 🔧 Các tính năng chính của dự án:
- **Tự động hóa hoàn toàn:** Tự động dò và tạo CSDL MySQL.
- **Biểu đồ Real-time:** Cập nhật liên tục 2 giây/lần mà không cần tải lại trang.
- **Dark Mode chuyên nghiệp:** Giao diện tối ưu cho giám sát an ninh.
- **Cấu hình WiFi thông minh (Captive Portal):** Chỉ cần điện thoại kết nối vào WiFi nội bộ của ESP32 để thiết lập IP Server mà không cần hard-code mật khẩu WiFi vào mạch.

> *Dự án được phát triển và tối ưu hóa phục vụ Đồ án Học phần.*
