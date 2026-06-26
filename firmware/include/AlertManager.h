#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include <Arduino.h>

class AlertManager {
public:
    void init();
    void loop();
    
    void beepSuccess();
    void triggerAlarm();
    void stopAlarm();
    bool getAlarmState();

private:
    bool isAlarming = false;
    unsigned long lastAlarmToggleTime = 0;
    bool alarmState = false;
};

#endif

