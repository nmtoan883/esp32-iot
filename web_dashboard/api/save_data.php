<?php
require 'db_connect.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

// =============================================
// save_data.php — Nhận dữ liệu từ ESP32
// Hỗ trợ hai loại POST:
//   type=sensor → Lưu cảm biến môi trường
//   type=rfid   → Lưu lịch sử quẹt thẻ RFID
// Content-Type: application/x-www-form-urlencoded
// =============================================

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    echo json_encode(["status" => "error", "message" => "Chi chap nhan POST request."]);
    $conn->close();
    exit;
}

// Đọc trường 'type' để phân loại yêu cầu
$type = isset($_POST['type']) ? trim($_POST['type']) : '';

// =============================================
// Xử lý type=sensor — Dữ liệu cảm biến môi trường
// Payload: type=sensor&temperature=27.8&humidity=62.1&light=300.5
// =============================================
if ($type === 'sensor') {
    if (!isset($_POST['temperature']) || !isset($_POST['humidity']) || !isset($_POST['light'])) {
        echo json_encode(["status" => "error", "message" => "Thieu truong du lieu: can temperature, humidity, light."]);
        $conn->close();
        exit;
    }

    $temperature = floatval($_POST['temperature']);
    $humidity    = floatval($_POST['humidity']);
    $light       = floatval($_POST['light']);

    // Lưu vào bảng sensor_logs (bảng chính mới)
    $stmt = $conn->prepare("INSERT INTO sensor_logs (temperature, humidity, light) VALUES (?, ?, ?)");
    $stmt->bind_param("ddd", $temperature, $humidity, $light);

    if ($stmt->execute()) {
        // Đồng thời lưu vào temperature_logs để tương thích ngược với dashboard cũ
        $stmtLegacy = $conn->prepare("INSERT INTO temperature_logs (temperature, humidity) VALUES (?, ?)");
        $stmtLegacy->bind_param("dd", $temperature, $humidity);
        $stmtLegacy->execute();
        $stmtLegacy->close();

        echo json_encode([
            "status"  => "success",
            "message" => "Da luu du lieu cam bien.",
            "data"    => ["temperature" => $temperature, "humidity" => $humidity, "light" => $light]
        ]);
    } else {
        echo json_encode(["status" => "error", "message" => "Loi database: " . $stmt->error]);
    }
    $stmt->close();

// =============================================
// Xử lý type=rfid — Lịch sử quẹt thẻ RFID
// Payload: type=rfid&uid=A1+21+34+16
// =============================================
} elseif ($type === 'rfid') {
    if (!isset($_POST['uid'])) {
        echo json_encode(["status" => "error", "message" => "Thieu truong 'uid'."]);
        $conn->close();
        exit;
    }

    $uid    = trim($_POST['uid']);
    // Trạng thái mặc định: unknown (sẽ được xác định bởi ESP32 nếu cần)
    $status = isset($_POST['status']) ? trim($_POST['status']) : 'unknown';

    $stmt = $conn->prepare("INSERT INTO rfid_logs (uid, status) VALUES (?, ?)");
    $stmt->bind_param("ss", $uid, $status);

    if ($stmt->execute()) {
        echo json_encode([
            "status"  => "success",
            "message" => "Da luu lich su quet the RFID.",
            "data"    => ["uid" => $uid, "rfid_status" => $status]
        ]);
    } else {
        echo json_encode(["status" => "error", "message" => "Loi database: " . $stmt->error]);
    }
    $stmt->close();

// =============================================
// Loại không xác định
// =============================================
} else {
    echo json_encode([
        "status"  => "error",
        "message" => "Truong 'type' khong hop le. Can 'sensor' hoac 'rfid'.",
        "received" => $type
    ]);
}

$conn->close();
?>
