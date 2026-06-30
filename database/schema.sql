-- Create the database if it doesn't exist
CREATE DATABASE IF NOT EXISTS iot_db;

-- Use the database
USE iot_db;

-- Create the temperature_logs table
CREATE TABLE IF NOT EXISTS temperature_logs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    temperature FLOAT NOT NULL,
    humidity FLOAT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create the users table
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Note: We include humidity because DHT11/DHT22 sensors also provide humidity data.
