# 📖 HƯỚNG DẪN CHẠY DỰ ÁN TỪ A-Z (DÀNH CHO NGƯỜI CHẤM ĐIỂM HOẶC BÁO CÁO)

Tài liệu này hướng dẫn các bước nhanh gọn nhất để khởi động toàn bộ hệ thống Smart Home. Vui lòng làm theo đúng thứ tự.

---

## BƯỚC 1: KHỞI ĐỘNG CƠ SỞ DỮ LIỆU (DATABASE)
Hệ thống sử dụng cơ chế tự động tạo CSDL (Auto-Migration), nên bạn **không cần** vào phpMyAdmin để tạo bảng bằng tay.
1. Mở phần mềm **XAMPP Control Panel**.
2. Bấm nút **Start** ở dòng **MySQL**.
*(Chỉ cần bật MySQL, không cần bật Apache vì nhóm tự viết Server riêng).*

---

## BƯỚC 2: KHỞI ĐỘNG WEB SERVER CỤC BỘ
Thay vì phải gõ lệnh phức tạp, nhóm đã tạo sẵn một file chạy tự động.
1. Mở thư mục `web_dashboard` trên máy tính.
2. Nhấn đúp chuột (mở) file có tên là **`start_server.bat`**.
3. Một cửa sổ màu đen sẽ hiện lên báo hiệu Server đã chạy. **Vui lòng giữ nguyên cửa sổ này, không được tắt!**
4. Mở Google Chrome, truy cập vào đường link: `http://localhost:8000/`

---

## BƯỚC 3: XÓA TRẮNG VÀ NẠP CODE XUỐNG ESP32
Khi cần nạp lại code cho mạch phần cứng, làm theo các bước sau trong phần mềm **VS Code**:

1. Mở thư mục `firmware` trong VS Code.
2. Cắm cáp USB nối mạch ESP32 với máy tính.
3. Rút 3 sợi dây ở các chân **D12, D5, D15** ra khỏi mạch (nếu đang cắm) để tránh bị lỗi khóa nạp.
4. Mở Terminal trong VS Code (chọn menu `Terminal` -> `New Terminal`).
5. **Để xóa code cũ trong mạch**, gõ lệnh sau rồi nhấn Enter:
   ```bash
   pio run -t erase
   ```
   *(Nếu thấy chữ `Connecting...`, hãy nhấn giữ nút BOOT trên mạch khoảng 3 giây).*

6. **Để nạp code mới vào mạch**, gõ lệnh sau rồi nhấn Enter:
   ```bash
   pio run -t upload
   ```
   *(Nếu thấy chữ `Connecting...`, lại tiếp tục nhấn giữ nút BOOT cho đến khi phần trăm % chạy).*

7. Sau khi báo `SUCCESS`, cắm lại 3 sợi dây ban nãy vào mạch, rồi nhấn nút **EN** (hoặc **RESET**) trên mạch ESP32 1 lần để mạch bắt đầu chạy.

---

## BƯỚC 4: KẾT NỐI WIFI CHO MẠCH (CAPTIVE PORTAL)
*(Chỉ làm bước này nếu mạch chớp đèn liên tục do chưa có WiFi).*
1. Lấy điện thoại mở dò WiFi, tìm WiFi tên là **`SmartHome_Setup`**.
2. Nhấn kết nối (không có mật khẩu).
3. Điện thoại sẽ tự động nhảy lên một trang Web cài đặt (hoặc bạn tự mở trình duyệt gõ `192.168.4.1`).
4. Nhấn **Configure WiFi**.
5. Chọn tên WiFi của nhà bạn, nhập mật khẩu WiFi.
6. Ở ô **Server IP**, nhập địa chỉ IP của máy tính đang chạy Web Server (ví dụ: `192.168.1.10`).
7. Nhấn **Save**. Mạch sẽ tự động khởi động lại và kết nối vào hệ thống. Hoàn tất!
