#include "SensorManager.h"
#include "config.h"

void SensorManager::init() {
    // Khoi tao DHT11
    dht = new DHT(DHTPIN, DHTTYPE);
    dht->begin();

    // Thiet lap cac chan cam bien
    pinMode(PIR_PIN, INPUT);
    pinMode(LDR_PIN, INPUT);
    
    // MC-38 hoat dong theo dang cong tac tu, thuong xai INPUT_PULLUP de day on dinh
    pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
    
    currentTemp = 0.0;
    currentHum = 0.0;
    lastDHTReadTime = 0;
}

void SensorManager::loop() {
    // Doc cam bien DHT moi 2 giay de loc nhieu
    if (millis() - lastDHTReadTime >= dhtReadInterval) {
        lastDHTReadTime = millis();
        
        float tempRead = dht->readTemperature();
        float humRead = dht->readHumidity();

        // Loc nhieu bang isnan: neu sai thi khong ghi de
        bool hasError = false;
        if (!isnan(tempRead)) {
            currentTemp = tempRead;
        } else {
            hasError = true;
        }

        if (!isnan(humRead)) {
            currentHum = humRead;
        } else {
            hasError = true;
        }

        if (hasError) {
            Serial.println("SensorManager: Doc DHT bi loi (NaN)!");
        }
    }
}

float SensorManager::getTemperature() {
    return currentTemp;
}

float SensorManager::getHumidity() {
    return currentHum;
}

bool SensorManager::isDark() {
    // Ngoc doc Analog tu 0 - 4095
    int ldrValue = analogRead(LDR_PIN);
    return (ldrValue > LIGHT_DARK_THRESHOLD);
}

bool SensorManager::isMotionDetected() {
    return (digitalRead(PIR_PIN) == HIGH);
}

bool SensorManager::isDoorPhysicallyOpen() {
    // MC-38: Cua mo -> HIGH, Cua dong -> LOW
    return (digitalRead(DOOR_SENSOR_PIN) == HIGH);
}

