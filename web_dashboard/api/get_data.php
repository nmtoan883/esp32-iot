<?php
require 'db_connect.php';

// Set headers for JSON response and CORS
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

// Determine limit (default 20 latest records for the chart)
$limit = isset($_GET['limit']) ? intval($_GET['limit']) : 20;

// Fetch latest data, ordered by time descending, then reverse it for the chart
$sql = "SELECT * FROM (SELECT id, temperature, humidity, created_at FROM temperature_logs ORDER BY id DESC LIMIT ?) AS sub ORDER BY id ASC";
$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $limit);
$stmt->execute();
$result = $stmt->get_result();

$data = array();

while($row = $result->fetch_assoc()) {
    $data[] = $row;
}

echo json_encode($data);

$stmt->close();
$conn->close();
?>
