<?php
// Configuration for Database connection
$servername = "localhost";
$username = "root"; // Default XAMPP username
$password = "";     // Default XAMPP password
$dbname = "iot_db";

// Bước 1: Kết nối trực tiếp vào MySQL (chưa chọn Database)
$conn = new mysqli($servername, $username, $password);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
$conn->set_charset("utf8mb4");

// Bước 2: TỰ ĐỘNG TẠO DATABASE nếu chưa có (Khỏi cần Import bằng tay)
$sql_create_db = "CREATE DATABASE IF NOT EXISTS $dbname";
if ($conn->query($sql_create_db) === FALSE) {
    die("Loi tao database: " . $conn->error);
}

// Bước 3: Chọn database vừa tạo
$conn->select_db($dbname);

// Bước 4: TỰ ĐỘNG TẠO BẢNG temperature_logs nếu chưa có
$sql_create_table = "CREATE TABLE IF NOT EXISTS temperature_logs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    temperature FLOAT NOT NULL,
    humidity FLOAT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
)";
if ($conn->query($sql_create_table) === FALSE) {
    die("Loi tao bang: " . $conn->error);
}
?>
