<?php
require 'db_connect.php';

// Set headers for JSON response
header('Content-Type: application/json');

// Check if request is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    
    // Get raw POST data
    $json = file_get_contents('php://input');
    
    // Decode JSON
    $data = json_decode($json, true);

    // Check if JSON decoding was successful and data exists
    if ($data !== null && isset($data['temperature']) && isset($data['humidity'])) {
        
        $temperature = floatval($data['temperature']);
        $humidity = floatval($data['humidity']);

        // Prepare and bind
        $stmt = $conn->prepare("INSERT INTO temperature_logs (temperature, humidity) VALUES (?, ?)");
        $stmt->bind_param("dd", $temperature, $humidity);

        if ($stmt->execute()) {
            echo json_encode(["status" => "success", "message" => "Data saved successfully"]);
        } else {
            echo json_encode(["status" => "error", "message" => "Database error: " . $stmt->error]);
        }

        $stmt->close();
    } else {
        // If not JSON, try standard POST fields just in case (fallback)
        if(isset($_POST['temperature']) && isset($_POST['humidity'])) {
            $temperature = floatval($_POST['temperature']);
            $humidity = floatval($_POST['humidity']);
            
            $stmt = $conn->prepare("INSERT INTO temperature_logs (temperature, humidity) VALUES (?, ?)");
            $stmt->bind_param("dd", $temperature, $humidity);

            if ($stmt->execute()) {
                echo json_encode(["status" => "success", "message" => "Data saved successfully"]);
            } else {
                echo json_encode(["status" => "error", "message" => "Database error: " . $stmt->error]);
            }

            $stmt->close();
        } else {
             echo json_encode(["status" => "error", "message" => "Invalid data format. Expected JSON with 'temperature' and 'humidity'"]);
        }
    }
} else {
    echo json_encode(["status" => "error", "message" => "Invalid request method. Only POST is allowed."]);
}

$conn->close();
?>
