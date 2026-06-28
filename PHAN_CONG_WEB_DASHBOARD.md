# BẢNG PHÂN CÔNG NHIỆM VỤ PHÁT TRIỂN WEB DASHBOARD (IOT)

---

### 👤 Thành viên 1: Quản trị Cơ sở dữ liệu & Backend API
**Nhiệm vụ chi tiết:**
- **Lập trình ánh xạ Database (Auto-Migration):** Viết logic code (`db_connect.php`) để tự động khởi tạo Database `iot_db` và ánh xạ (mapping) tạo các bảng `temperature_logs`, `rfid_logs` thẳng vào SQL mà không cần người dùng thao tác bằng tay.
- **Viết API Nhận dữ liệu (save_data.php):** Xây dựng điểm cuối (Endpoint) để nhận dữ liệu từ mạch ESP32 gửi lên bằng phương thức POST và ghi an toàn vào DB.
- **Viết API Xuất dữ liệu (get_data.php):** Xây dựng RESTful API trả về dữ liệu định dạng JSON để phục vụ cho giao diện Web (Frontend) vẽ biểu đồ.

### 👤 Thành viên 2: Thiết kế Giao diện Khung (UI Layout & CSS)
**Nhiệm vụ chi tiết:**
- **Xây dựng cấu trúc HTML (index.php):** Lên khung bộ xương cho trang Dashboard với các khu vực: Header (Logo), Thẻ thông số (Cards), Khu vực Biểu đồ, Khu vực Lịch sử ra vào.
- **Phát triển CSS & Dark Mode:** Viết toàn bộ file `style.css` để định dạng màu sắc mang phong cách Dark Mode chuyên nghiệp. Sử dụng CSS Grid/Flexbox để bố cục tự động co giãn.
- **Tích hợp Icon & Font:** Nhúng Google Fonts (Outfit) và FontAwesome để làm các icon nhiệt kế, giọt nước, đồng hồ,...

### 👤 Thành viên 3: Lập trình Trực quan hóa Dữ liệu (Real-time Charts)
**Nhiệm vụ chi tiết:**
- **Tích hợp Chart.js:** Khởi tạo và thiết lập thư viện vẽ biểu đồ `Chart.js` lên khu vực Canvas của trang Web.
- **Lập trình Gọi API (AJAX/Fetch):** Viết mã Javascript (`chart.js`) liên tục gọi API `get_data.php` cứ mỗi 2 giây một lần.
- **Vẽ Biểu đồ Động:** Đẩy mảng dữ liệu lấy được từ API vào biểu đồ. Tạo hiệu ứng mượt mà (smooth curves) và màu sắc Gradient đặc trưng cho đường Nhiệt độ (Đỏ) và Độ ẩm (Xanh).

### 👤 Thành viên 4: Lập trình Module An ninh & Lịch sử Thẻ từ (RFID)
**Nhiệm vụ chi tiết:**
- **Thiết kế Bảng Dữ liệu (Data Table):** Bổ sung khu vực Bảng hiển thị Lịch sử ra vào (Access Logs) vào trang Dashboard chính.
- **Xử lý Dữ liệu Thẻ từ:** Viết Javascript để bóc tách mảng dữ liệu RFID từ API, sau đó render (hiển thị) thành các hàng (rows) trong thẻ HTML `<table>`.
- **Logic Trạng thái Cửa:** Viết logic đổi màu trạng thái dựa trên mã thẻ (Màu Xanh: Thành công / Màu Đỏ: Sai thẻ). 

### 👤 Thành viên 5: Tính năng Cảnh báo & Tối ưu Trải nghiệm (Alerts & UX)
**Nhiệm vụ chi tiết:**
- **Logic Cảnh báo Cháy (Fire Alert):** Viết thuật toán Javascript liên tục giám sát con số Nhiệt độ hiện tại. Nếu vượt quá mức 35°C, lập tức làm xuất hiện Banner màu đỏ nhấp nháy trên Web.
- **Hiệu ứng Trạng thái (Live Status):** Lập trình chấm tròn nháy sáng (Pulse Dot) và thông báo "Live Data Active" để người dùng biết Web vẫn đang kết nối tới Server. Xử lý kịch bản mất mạng (Connection Error).
- **Responsive (Tương thích Mobile):** Bổ sung code CSS Media Queries (`@media screen`) để giao diện Web vẫn hiển thị đẹp và bảng biểu đồ không bị méo khi người dùng xem qua màn hình điện thoại di động.

---
**Nhận xét:** (Từ MySQL -> PHP Backend -> HTML/CSS -> Javascript DOM -> Javascript API).
