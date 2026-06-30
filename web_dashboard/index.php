<?php
// Bắt đầu session
session_start();

// Kiểm tra nếu chưa đăng nhập thì đá về trang login.php
if (!isset($_SESSION['user_id'])) {
    header("Location: login.php");
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IoT Room Monitor Dashboard</title>
    <!-- Google Fonts -->
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;600;800&display=swap" rel="stylesheet">
    <!-- Font Awesome for Icons -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <!-- Chart.js -->
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <!-- Custom Styles -->
    <link rel="stylesheet" href="assets/style.css">
</head>
<body>
    <div class="background-anim"></div>
    <div class="dashboard-container">
        
        <header class="dashboard-header">
            <div class="logo">
                <i class="fa-solid fa-microchip"></i>
                <h1>IoT Monitor</h1>
            </div>
            <div class="header-right">
                <div class="status-indicator">
                    <span class="pulse-dot"></span>
                    <span id="connection-status">Live Data Active</span>
                </div>
                <a href="logout.php" class="logout-btn">
                    <i class="fa-solid fa-right-from-bracket"></i>
                    <span>Đăng xuất (<?php echo htmlspecialchars($_SESSION['username']); ?>)</span>
                </a>
            </div>
        </header>

        <div id="alert-banner" class="alert-banner hidden">
            <i class="fa-solid fa-triangle-exclamation"></i>
            <span>Warning: High Temperature Detected!</span>
        </div>

        <main class="dashboard-content">
            <!-- Top Cards -->
            <div class="cards-wrapper">
                <div class="card stat-card temp-card">
                    <div class="card-icon">
                        <i class="fa-solid fa-temperature-half"></i>
                    </div>
                    <div class="card-info">
                        <h3>Temperature</h3>
                        <div class="value">
                            <span id="current-temp">--</span>°C
                        </div>
                    </div>
                </div>

                <div class="card stat-card hum-card">
                    <div class="card-icon">
                        <i class="fa-solid fa-droplet"></i>
                    </div>
                    <div class="card-info">
                        <h3>Humidity</h3>
                        <div class="value">
                            <span id="current-hum">--</span>%
                        </div>
                    </div>
                </div>

                <div class="card stat-card time-card">
                    <div class="card-icon">
                        <i class="fa-regular fa-clock"></i>
                    </div>
                    <div class="card-info">
                        <h3>Last Updated</h3>
                        <div class="value time-value">
                            <span id="last-update">--:--:--</span>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Chart Area -->
            <div class="card chart-card">
                <div class="card-header">
                    <h2>Real-time Analytics</h2>
                    <div class="chart-legend">
                        <span class="legend-item"><span class="color-box temp-color"></span> Temp</span>
                        <span class="legend-item"><span class="color-box hum-color"></span> Hum</span>
                    </div>
                </div>
                <div class="chart-container">
                    <canvas id="sensorChart"></canvas>
                </div>
            </div>
        </main>
        
        <footer>
            <p>ESP32 IoT Real-time Dashboard &copy; 2026</p>
        </footer>
    </div>

    <!-- Application Script -->
    <script src="assets/chart.js"></script>
</body>
</html>
