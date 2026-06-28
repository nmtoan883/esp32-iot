# BẢNG PHÂN CÔNG NHIỆM VỤ PHÁT TRIỂN WEB DASHBOARD (IOT)

> *Để đảm bảo tính thực tế hệ thống Web Dashboard được nâng cấp với tính năng **Xác thực người dùng (Authentication)**. Chỉ những người có tài khoản mới được phép xem dữ liệu Smart Home.*

---

### 👤 Thành viên 1: Quản trị Cơ sở dữ liệu & Bảo mật (Database & Security)
**Nhiệm vụ chi tiết:**
- **Thiết kế Database (MySQL):** Xây dựng cấu trúc CSDL `iot_db` gồm 3 bảng: `users` (tài khoản đăng nhập), `temperature_logs` (lưu nhiệt độ), `rfid_logs` (lưu lịch sử quẹt thẻ).
- **Mã hóa dữ liệu:** Sử dụng thuật toán Hash (Bcrypt hoặc MD5) để mã hóa mật khẩu người dùng trước khi lưu vào CSDL.
- **Viết API Xác thực:** Lập trình các file backend xử lý logic Đăng nhập (`login_process.php`) và Đăng ký (`register_process.php`).

### 👤 Thành viên 2: Lập trình Backend API Dữ liệu (Data API)
**Nhiệm vụ chi tiết:**
- **Lập trình ánh xạ Database (Auto-Migration):** Viết logic code (`db_connect.php`) tự động tạo các bảng nếu chưa có.
- **Viết API Nhận dữ liệu (save_data.php):** Nhận dữ liệu từ mạch ESP32 gửi lên và ghi an toàn vào DB.
- **Viết API Xuất dữ liệu (get_data.php):** Xây dựng RESTful API trả về dữ liệu JSON để giao diện Web vẽ biểu đồ.

### 👤 Thành viên 3: Thiết kế Giao diện Đăng nhập & Đăng ký (Auth Frontend)
**Nhiệm vụ chi tiết:**
- **Xây dựng UI Đăng nhập/Đăng ký:** Thiết kế 2 trang `login.php` và `register.php` mang phong cách hiện đại (Form nhập liệu, nút bấm, hiệu ứng hover).
- **Xử lý Session (Phiên đăng nhập):** Viết code PHP để cấp `$_SESSION` khi đăng nhập thành công.
- **Bảo vệ Dashboard (Route Protection):** Chèn đoạn code kiểm tra Session vào đầu trang `index.php`, nếu ai chưa đăng nhập mà cố tình truy cập sẽ bị đá văng về trang `login.php`.

### 👤 Thành viên 4: Thiết kế Giao diện Dashboard & Thẻ từ (Dashboard UI)
**Nhiệm vụ chi tiết:**
- **Xây dựng cấu trúc HTML (index.php):** Lên khung bộ xương cho trang Dashboard: Header (Hiển thị tên User đang đăng nhập), Thẻ thông số (Cards), và Khu vực Biểu đồ.
- **Bảng Lịch sử Thẻ từ (RFID Logs):** Thiết kế bảng dữ liệu hiển thị lịch sử quẹt thẻ mở cửa. Viết CSS đổi màu trạng thái (Thành công/Thất bại).
- **Phát triển CSS & Dark Mode:** Viết toàn bộ file `style.css` để định dạng màu sắc mang phong cách Dark Mode chuyên nghiệp. Tích hợp nút Đăng xuất (Logout).

### 👤 Thành viên 5: Trực quan hóa Dữ liệu & Cảnh báo (Real-time Charts & Alerts)
**Nhiệm vụ chi tiết:**
- **Tích hợp Chart.js:** Khởi tạo thư viện `Chart.js`, viết Javascript liên tục gọi API `get_data.php` (mỗi 2 giây) bằng công nghệ AJAX/Fetch.
- **Vẽ Biểu đồ Động:** Đẩy mảng dữ liệu lấy được từ API vào biểu đồ. Tạo hiệu ứng mượt mà (smooth curves) và màu sắc Gradient đặc trưng.
- **Logic Cảnh báo Cháy (Fire Alert):** Viết thuật toán JS giám sát Nhiệt độ hiện tại. Nếu vượt quá mức 35°C, lập tức làm xuất hiện Banner màu đỏ nhấp nháy trên Web. Xử lý kịch bản mất mạng (Connection Error).

---
**Nhận xét:** Bảng phân công này tạo ra một hệ thống Web hoàn chỉnh (Từ Auth -> API -> Frontend). 
