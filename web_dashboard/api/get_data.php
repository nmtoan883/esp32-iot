<?php
require 'db_connect.php';

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

// =============================================
// get_data.php — Trả dữ liệu về cho ESP32 và Dashboard
//
// Chế độ 1: ?cmd=1  → Trả lệnh chưa đọc (cho ESP32)
// Chế độ 2: ?type=sensor&limit=N → Trả lịch sử cảm biến (cho Dashboard)
// Chế độ 3: ?type=rfid&limit=N   → Trả lịch sử RFID (cho Dashboard)
// Chế độ mặc định (không có param) → Tương thích ngược: trả sensor_logs
// =============================================

$limit = isset($_GET['limit']) ? intval($_GET['limit']) : 20;

// =============================================
// Chế độ 1: ESP32 hỏi có lệnh mới không?
// URL: get_data.php?cmd=1
// =============================================
if (isset($_GET['cmd']) && $_GET['cmd'] == '1') {
    // Lấy tất cả lệnh chưa được đọc
    $stmt = $conn->prepare(
        "SELECT id, command FROM server_commands WHERE is_read = 0 ORDER BY created_at ASC"
    );
    $stmt->execute();
    $result = $stmt->get_result();

    $commands = [];
    $ids      = [];
    while ($row = $result->fetch_assoc()) {
        $commands[] = $row['command'];
        $ids[]      = $row['id'];
    }
    $stmt->close();

    // Đánh dấu đã đọc (is_read = 1)
    if (!empty($ids)) {
        $placeholders = implode(',', array_fill(0, count($ids), '?'));
        $types        = str_repeat('i', count($ids));
        $stmtUpdate   = $conn->prepare("UPDATE server_commands SET is_read = 1 WHERE id IN ($placeholders)");
        $stmtUpdate->bind_param($types, ...$ids);
        $stmtUpdate->execute();
        $stmtUpdate->close();
    }

    echo json_encode($commands); // Trả mảng lệnh, ví dụ: ["LOCK", "ALARM_ON"]
    $conn->close();
    exit;
}

// =============================================
// Chế độ 2: Dashboard lấy lịch sử cảm biến
// URL: get_data.php?type=sensor&limit=20
// =============================================
if (isset($_GET['type']) && $_GET['type'] === 'sensor') {
    $sql  = "SELECT * FROM (SELECT id, temperature, humidity, light, created_at 
             FROM sensor_logs ORDER BY id DESC LIMIT ?) AS sub ORDER BY id ASC";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("i", $limit);
    $stmt->execute();
    $result = $stmt->get_result();

    $data = [];
    while ($row = $result->fetch_assoc()) {
        $data[] = $row;
    }
    $stmt->close();
    echo json_encode($data);
    $conn->close();
    exit;
}

// =============================================
// Chế độ 3: Dashboard lấy lịch sử RFID
// URL: get_data.php?type=rfid&limit=20
// =============================================
if (isset($_GET['type']) && $_GET['type'] === 'rfid') {
    $sql  = "SELECT id, uid, status, created_at FROM rfid_logs ORDER BY id DESC LIMIT ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("i", $limit);
    $stmt->execute();
    $result = $stmt->get_result();

    $data = [];
    while ($row = $result->fetch_assoc()) {
        $data[] = $row;
    }
    $stmt->close();
    echo json_encode($data);
    $conn->close();
    exit;
}

// =============================================
// Chế độ mặc định — Tương thích ngược với dashboard cũ
// URL: get_data.php hoặc get_data.php?limit=N
// =============================================
$sql  = "SELECT * FROM (SELECT id, temperature, humidity, created_at 
         FROM temperature_logs ORDER BY id DESC LIMIT ?) AS sub ORDER BY id ASC";
$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $limit);
$stmt->execute();
$result = $stmt->get_result();

$data = [];
while ($row = $result->fetch_assoc()) {
    $data[] = $row;
}
$stmt->close();
echo json_encode($data);
$conn->close();
?>
