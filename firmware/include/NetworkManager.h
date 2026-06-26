#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>

class NetworkManager {
public:
    void init();
    void loop();
    void sendDataToServer(float temp, float hum);

private:
    unsigned long lastConnectionCheck = 0;
    const unsigned long connectionCheckInterval = 5000; // Kiểm tra mỗi 5 giây
};

#endif

