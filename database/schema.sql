-- =============================================
-- Database Schema cho Smart Home IoT Project
-- iot_db
-- =============================================

-- Tạo database nếu chưa có
CREATE DATABASE IF NOT EXISTS iot_db;
USE iot_db;

-- =============================================
-- Bảng 1: Lưu dữ liệu cảm biến môi trường
-- Dùng bởi: Nhóm 1 (sendSensorData) + Nhóm 3
-- =============================================
CREATE TABLE IF NOT EXISTS sensor_logs (
    id          INT AUTO_INCREMENT PRIMARY KEY,
    temperature FLOAT         NOT NULL COMMENT 'Nhiệt độ (°C)',
    humidity    FLOAT         NOT NULL COMMENT 'Độ ẩm (%)',
    light       FLOAT         NOT NULL DEFAULT 0 COMMENT 'Cường độ ánh sáng (ADC 0-4095)',
    created_at  TIMESTAMP     DEFAULT CURRENT_TIMESTAMP
);

-- =============================================
-- Bảng 2: Lưu lịch sử quẹt thẻ RFID
-- Dùng bởi: Nhóm 1 (sendRFIDLog) + Nhóm 2
-- =============================================
CREATE TABLE IF NOT EXISTS rfid_logs (
    id         INT AUTO_INCREMENT PRIMARY KEY,
    uid        VARCHAR(50)   NOT NULL COMMENT 'Mã UID thẻ từ, ví dụ: A1 21 34 16',
    status     VARCHAR(20)   NOT NULL DEFAULT 'unknown' COMMENT 'granted / denied / unknown',
    created_at TIMESTAMP     DEFAULT CURRENT_TIMESTAMP
);

-- =============================================
-- Bảng 3: Lệnh điều khiển từ Dashboard → ESP32
-- Dùng bởi: Nhóm 1 (fetchServerCommand) + Nhóm 5
-- =============================================
CREATE TABLE IF NOT EXISTS server_commands (
    id         INT AUTO_INCREMENT PRIMARY KEY,
    command    VARCHAR(100)  NOT NULL COMMENT 'Lệnh, ví dụ: LOCK, UNLOCK, ALARM_ON',
    is_read    TINYINT(1)    NOT NULL DEFAULT 0 COMMENT '0 = chưa ESP32 đọc, 1 = đã đọc',
    created_at TIMESTAMP     DEFAULT CURRENT_TIMESTAMP
);

-- =============================================
-- Tương thích ngược: Giữ lại bảng cũ temperature_logs
-- (để web_dashboard hiện tại không bị lỗi)
-- =============================================
CREATE TABLE IF NOT EXISTS temperature_logs (
    id          INT AUTO_INCREMENT PRIMARY KEY,
    temperature FLOAT         NOT NULL,
    humidity    FLOAT         NOT NULL,
    created_at  TIMESTAMP     DEFAULT CURRENT_TIMESTAMP
);
