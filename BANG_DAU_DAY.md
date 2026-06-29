# 🔌 BẢNG HƯỚNG DẪN ĐẤU NỐI PHẦN CỨNG (WIRING GUIDE)
**Dự án:** Smart Home IoT - Cảnh báo Nhiệt độ & Kiểm soát Cửa
**Vi điều khiển chính:** ESP32 (38 chân hoặc 30 chân)

> ⚠️ **CẢNH BÁO CAO ĐỘ CHỐNG CHÁY NỔ:**
> Đọc kỹ cột **Nguồn Điện (VCC)**. Tuyệt đối không cắm linh kiện 3.3V vào chân 5V của ESP32 (chân 5V thường ký hiệu là `VIN` hoặc `5V`). Sai lầm này sẽ làm cháy linh kiện ngay lập tức.

---

## 1. CẢM BIẾN (SENSORS)

### 🌡 Cảm biến Nhiệt độ / Độ ẩm (DHT11 - Dạng Module 3 chân)
* **VCC (Nguồn +):** Nối vào chân **3.3V** hoặc **5V (VIN)** của ESP32.
* **GND (Nguồn -):** Nối vào chân **GND** của ESP32.
* **DATA / OUT (Tín hiệu):** Nối vào chân **D4**.

### 💡 Cảm biến Ánh sáng (LDR Quang trở - Dạng Module)
* **VCC:** Nối vào chân **3.3V** của ESP32.
* **GND:** Nối vào chân **GND**.
* **AO (Analog Out):** Nối vào chân **D34** (Chân chuyên đọc tín hiệu Analog).

### 🏃 Cảm biến Chuyển động (PIR HC-SR501)
* **VCC:** Khuyên dùng chân **5V (VIN)** để cảm biến hoạt động ổn định.
* **GND:** Nối vào chân **GND**.
* **OUT (Tín hiệu):** Nối vào chân **D14**.

### 🚪 Cảm biến Cửa từ (Từ chuẩn MC-38)
*Cảm biến này chỉ có 2 dây, không phân biệt âm dương.*
* **Dây 1:** Nối vào chân **D25**.
* **Dây 2:** Nối vào chân **GND**.
*(Mạch ESP32 sẽ tự động kéo điện trở nội bên trong để nhận diện trạng thái đóng/mở).*

---

## 2. GIAO TIẾP & HIỂN THỊ (HMI)

### 📺 Màn hình LCD 1602 (Kèm Module I2C đằng sau)
* **VCC:** Bắt buộc nối chân **5V (VIN)** để màn hình đủ độ sáng.
* **GND:** Nối vào chân **GND**.
* **SDA (Dữ liệu I2C):** Nối vào chân **D21**.
* **SCL (Xung nhịp I2C):** Nối vào chân **D22**.

### 🔘 Nút nhấn (Push Buttons)
*Nút nhấn có 2 chân chéo nhau (hoặc 4 chân nhưng xài 2 chân cùng 1 phía).*
* **Nút MODE (Chuyển màn hình):**
  * Chân 1: Nối vào **D26**.
  * Chân 2: Nối thẳng vào **GND**.
* **Nút DOOR (Mở cửa từ trong):**
  * Chân 1: Nối vào **D27**.
  * Chân 2: Nối thẳng vào **GND**.

---

## 3. AN NINH & CẢNH BÁO (SECURITY & ALERTS)

### 💳 Đầu đọc Thẻ từ (RFID RC522)
*⚠️ Đây là linh kiện dễ cháy nhất, hãy đấu dây 2 lần để kiểm tra!*
* **3.3V (Nguồn +):** TUYỆT ĐỐI nối vào chân **3.3V**. (Cắm 5V là cháy mạch).
* **RST (Reset):** Nối vào chân **D15**. (⚠️ *Rút dây này ra khi nạp code*).
* **GND (Nguồn -):** Nối vào chân **GND**.
* **IRQ:** (Bỏ trống, không xài).
* **MISO:** Nối vào chân **D19**.
* **MOSI:** Nối vào chân **D23**.
* **SCK:** Nối vào chân **D18**.
* **SDA (SS):** Nối vào chân **D5**. (⚠️ *Rút dây này ra khi nạp code*).

### ⚙️ Động cơ Cửa (Servo SG90)
* **Dây Đỏ (Nguồn +):** Nối vào chân **5V (VIN)**. (Servo cần dòng điện cao để kéo cửa).
* **Dây Nâu / Đen (Nguồn -):** Nối vào chân **GND**.
* **Dây Cam / Vàng (Tín hiệu):** Nối vào chân **D33**.

### 🌬️ Relay Điều khiển Quạt (Tự động bật khi nóng)
* **IN (Tín hiệu):** Nối vào chân **D16**. (ESP32 sẽ đóng ngắt Relay qua chân này).
* **VCC (Nguồn của Relay):** Nối ra **Dải Đỏ Testboard (5V)**.
* **GND (Mass của Relay):** Nối ra **Dải Xanh Testboard (GND)**.
* **COM (Chân chung nối dòng 5V lớn):** Nối ra **Dải Đỏ Testboard (5V)**.
* **NO (Chân Thường Mở):** Nối dây chạy thẳng vào chân **V+** của Mạch Điều tốc phần cứng của bạn. (Chân V- của mạch điều tốc cắm thẳng ra GND Dải Xanh).

### 🚨 Còi Hú (Buzzer)
* **Chân Dài (+) hoặc Ký hiệu VCC:** Nối vào chân **D13**.
* **Chân Ngắn (-) hoặc Ký hiệu GND:** Nối vào chân **GND**.

### 🔴🟢 Đèn LED (Báo hiệu)
*Đèn LED cần hàn thêm Điện trở (khoảng 220 Ohm - 330 Ohm) ở chân Dương để không bị cháy.*
* **LED ĐỎ (Báo Cháy/Sai thẻ):** Chân dương (+) nối vào **D12** (⚠️ *Rút ra khi nạp code*). Chân âm (-) nối **GND**.
* **LED XANH (An toàn/Đúng thẻ):** Chân dương (+) nối vào **D32**. Chân âm (-) nối **GND**.
* **LED TRẮNG (Đèn chiếu sáng):** Chân dương (+) nối vào **D17**. Chân âm (-) nối **GND**.

---

## 📝 3 NGUYÊN TẮC CỐT LÕI KHI HÀN / ĐẤU MẠCH
1. **Chung Mass (GND):** Tất cả các dây GND của tất cả các linh kiện phải được nối chung với nhau và đi về chân GND của ESP32. Nên xài 1 cái Testboard (Breadboard) để câu chung đường GND này.
2. **Nguyên tắc "3 Dây Tử Thần":** Chân **D12, D5, D15** là các chân đặc biệt (Strapping Pins) của mạch ESP32. Nếu lúc đang nạp code (chữ `Connecting...` hiện ra) mà 3 chân này đang cắm, điện thế sai sẽ làm mạch bị kẹt, nạp thất bại. Hãy tháo jumper của 3 chân này ra, nạp xong rồi cắm lại.
3. **Cẩn thận nguồn dòng:** Nếu hú còi Buzzer + vặn Servo cùng lúc có thể làm mạch ESP32 bị hụt nguồn (sập nguồn sụt áp) dẫn đến việc tự khởi động lại (Reset). Nếu xảy ra, hãy cấp nguồn điện 5V rời (qua củ sạc điện thoại hoặc Module nguồn Testboard) thay vì cắm qua cổng USB máy tính.
