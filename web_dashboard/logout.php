<?php
// Bắt đầu session
session_start();

// Hủy bỏ toàn bộ biến session
$_SESSION = array();

// Nếu sử dụng cookie session, xóa cookie đó đi
if (ini_get("session.use_cookies")) {
    $params = session_get_cookie_params();
    setcookie(session_name(), '', time() - 42000,
        $params["path"], $params["domain"],
        $params["secure"], $params["httponly"]
    );
}

// Hủy hoàn toàn phiên đăng nhập (session)
session_destroy();

// Chuyển hướng người dùng về trang đăng nhập
header("Location: login.php");
exit();
?>
