#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <DHT.h>

class SensorManager {
public:
    void init();
    void loop();
    
    float getTemperature();
    float getHumidity();
    bool isDark();
    bool isMotionDetected();
    bool isDoorPhysicallyOpen();

private:
    DHT* dht = nullptr;
    float currentTemp = 0.0;
    float currentHum = 0.0;
    unsigned long lastDHTReadTime = 0;
    const unsigned long dhtReadInterval = 2000; // Đọc DHT mỗi 2 giây
};

#endif

