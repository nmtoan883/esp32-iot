<?php
// Bắt đầu session
session_start();

// Nếu đã đăng nhập thì tự động chuyển hướng về trang chủ index.php
if (isset($_SESSION['user_id'])) {
    header("Location: index.php");
    exit();
}

// Gọi file kết nối cơ sở dữ liệu
require_once 'api/db_connect.php';

// Khai báo các biến thông báo lỗi / thành công
$error_msg = "";
$success_msg = "";

// Kiểm tra xem có thông báo đăng ký thành công chuyển tiếp qua session không
if (isset($_SESSION['reg_success'])) {
    $success_msg = $_SESSION['reg_success'];
    unset($_SESSION['reg_success']); // Xóa đi để không hiển thị lại khi F5
}

// Xử lý đăng nhập khi người dùng nhấn nút Đăng nhập (gửi form bằng phương thức POST)
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Lấy dữ liệu từ form và loại bỏ khoảng trắng thừa
    $username = trim($_POST['username']);
    $password = $_POST['password'];

    // 1. Kiểm tra dữ liệu đầu vào
    if (empty($username) || empty($password)) {
        $error_msg = "Vui lòng nhập đầy đủ tài khoản và mật khẩu!";
    } else {
        // 2. Tìm kiếm người dùng theo username sử dụng Prepared Statement để bảo mật
        $stmt = $conn->prepare("SELECT id, username, password FROM users WHERE username = ?");
        $stmt->bind_param("s", $username);
        $stmt->execute();
        $result = $stmt->get_result();

        // 3. Kiểm tra xem tài khoản có tồn tại không
        if ($result->num_rows === 1) {
            $user = $result->fetch_assoc();
            
            // 4. Kiểm tra mật khẩu (Sử dụng password_verify để so sánh mật khẩu băm)
            if (password_verify($password, $user['password'])) {
                // Cấp session đăng nhập thành công
                $_SESSION['user_id'] = $user['id'];
                $_SESSION['username'] = $user['username'];

                // Chuyển hướng sang trang chủ index.php
                header("Location: index.php");
                exit();
            } else {
                $error_msg = "Mật khẩu đăng nhập không chính xác!";
            }
        } else {
            $error_msg = "Tên đăng nhập không tồn tại trên hệ thống!";
        }
        $stmt->close();
    }
}
?>
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Đăng nhập - IoT Monitor</title>
    <!-- Google Fonts -->
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;600;800&display=swap" rel="stylesheet">
    <!-- Font Awesome for Icons -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <!-- Custom Styles -->
    <link rel="stylesheet" href="assets/style.css">
</head>
<body>
    <div class="background-anim"></div>
    
    <div class="auth-wrapper">
        <div class="auth-card">
            
            <div class="auth-header">
                <i class="fa-solid fa-microchip"></i>
                <h2>Đăng Nhập</h2>
                <p>Hệ thống giám sát dữ liệu ESP32 IoT</p>
            </div>

            <!-- Hiển thị thông báo lỗi nếu có -->
            <?php if (!empty($error_msg)): ?>
                <div class="auth-alert alert-danger">
                    <i class="fa-solid fa-triangle-exclamation"></i>
                    <span><?php echo htmlspecialchars($error_msg); ?></span>
                </div>
            <?php endif; ?>

            <!-- Hiển thị thông báo thành công nếu có -->
            <?php if (!empty($success_msg)): ?>
                <div class="auth-alert alert-success">
                    <i class="fa-solid fa-circle-check"></i>
                    <span><?php echo htmlspecialchars($success_msg); ?></span>
                </div>
            <?php endif; ?>

            <form action="login.php" method="POST" class="auth-form">
                <div class="form-group">
                    <label for="username">TÊN ĐĂNG NHẬP</label>
                    <div class="input-container">
                        <i class="fa-solid fa-user"></i>
                        <input type="text" id="username" name="username" class="form-control" placeholder="Nhập tên đăng nhập" required value="<?php echo isset($username) ? htmlspecialchars($username) : ''; ?>">
                    </div>
                </div>

                <div class="form-group">
                    <label for="password">MẬT KHẨU</label>
                    <div class="input-container">
                        <i class="fa-solid fa-lock"></i>
                        <input type="password" id="password" name="password" class="form-control" placeholder="Nhập mật khẩu" required>
                    </div>
                </div>

                <button type="submit" class="auth-btn">
                    <i class="fa-solid fa-right-to-bracket"></i> Đăng Nhập
                </button>
            </form>

            <div class="auth-footer">
                Chưa có tài khoản? <a href="register.php">Đăng ký ngay</a>
            </div>

        </div>
    </div>
</body>
</html>
