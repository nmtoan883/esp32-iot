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

// Xử lý khi người dùng nhấn nút Đăng ký (gửi form bằng phương thức POST)
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Lấy dữ liệu từ form và loại bỏ khoảng trắng thừa
    $username = trim($_POST['username']);
    $password = $_POST['password'];
    $confirm_password = $_POST['confirm_password'];

    // 1. Kiểm tra xem các trường có bị bỏ trống hay không
    if (empty($username) || empty($password) || empty($confirm_password)) {
        $error_msg = "Vui lòng nhập đầy đủ tất cả các thông tin!";
    }
    // 2. Kiểm tra độ dài tài khoản (tối thiểu 3 ký tự)
    elseif (strlen($username) < 3) {
        $error_msg = "Tên đăng nhập phải chứa ít nhất 3 ký tự!";
    }
    // 3. Kiểm tra độ dài mật khẩu (tối thiểu 6 ký tự để bảo mật)
    elseif (strlen($password) < 6) {
        $error_msg = "Mật khẩu phải chứa ít nhất 6 ký tự!";
    }
    // 4. Kiểm tra xem mật khẩu nhập lại có khớp nhau không
    elseif ($password !== $confirm_password) {
        $error_msg = "Xác nhận mật khẩu không trùng khớp!";
    } else {
        // 5. Kiểm tra xem tên đăng nhập đã tồn tại trong hệ thống chưa
        // Sử dụng Prepared Statement để tránh lỗi SQL Injection
        $stmt_check = $conn->prepare("SELECT id FROM users WHERE username = ?");
        $stmt_check->bind_param("s", $username);
        $stmt_check->execute();
        $stmt_check->store_result();

        if ($stmt_check->num_rows > 0) {
            $error_msg = "Tên đăng nhập này đã tồn tại, vui lòng chọn tên khác!";
        } else {
            // 6. Mã hóa mật khẩu bảo mật trước khi lưu vào database
            $hashed_password = password_hash($password, PASSWORD_DEFAULT);

            // 7. Thực hiện lưu thông tin tài khoản mới vào database
            $stmt_insert = $conn->prepare("INSERT INTO users (username, password) VALUES (?, ?)");
            $stmt_insert->bind_param("ss", $username, $hashed_password);

            if ($stmt_insert->execute()) {
                $success_msg = "Đăng ký tài khoản thành công! Đang chuyển hướng...";
                $_SESSION['reg_success'] = "Đăng ký tài khoản thành công! Hãy đăng nhập.";
                // Chuyển hướng sang trang đăng nhập sau 2 giây
                header("refresh:2;url=login.php");
            } else {
                $error_msg = "Đã xảy ra lỗi hệ thống: " . $conn->error;
            }
            $stmt_insert->close();
        }
        $stmt_check->close();
    }
}
?>
<!DOCTYPE html>
<html lang="vi">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Đăng ký tài khoản - IoT Monitor</title>
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
                <i class="fa-solid fa-user-plus"></i>
                <h2>Đăng Ký</h2>
                <p>Tạo tài khoản để giám sát dữ liệu phòng IoT</p>
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

            <form action="register.php" method="POST" class="auth-form">
                <div class="form-group">
                    <label for="username">TÊN ĐĂNG NHẬP</label>
                    <div class="input-container">
                        <i class="fa-solid fa-user"></i>
                        <input type="text" id="username" name="username" class="form-control"
                            placeholder="Nhập tên đăng nhập" required
                            value="<?php echo isset($username) ? htmlspecialchars($username) : ''; ?>">
                    </div>
                </div>

                <div class="form-group">
                    <label for="password">MẬT KHẨU</label>
                    <div class="input-container">
                        <i class="fa-solid fa-lock"></i>
                        <input type="password" id="password" name="password" class="form-control"
                            placeholder="Nhập mật khẩu (tối thiểu 6 ký tự)" required>
                    </div>
                </div>

                <div class="form-group">
                    <label for="confirm_password">XÁC NHẬN MẬT KHẨU</label>
                    <div class="input-container">
                        <i class="fa-solid fa-shield-halved"></i>
                        <input type="password" id="confirm_password" name="confirm_password" class="form-control"
                            placeholder="Nhập lại mật khẩu" required>
                    </div>
                </div>

                <button type="submit" class="auth-btn">
                    <i class="fa-solid fa-user-plus"></i> Đăng Ký Tài Khoản
                </button>
            </form>

            <div class="auth-footer">
                Đã có tài khoản? <a href="login.php">Đăng nhập ngay</a>
            </div>

        </div>
    </div>
</body>

</html>