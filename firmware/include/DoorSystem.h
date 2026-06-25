#ifndef DOOR_SYSTEM_H
#define DOOR_SYSTEM_H

#include <Arduino.h>

// Nhom 2 (Nguoi 2): An ninh Ra Vao - Access Control & RFID
//
// Huong dan tich hop cho main.cpp (Nhom 5):
//   setup() -> door.init();
//   loop()  -> door.checkRFID();  // doc the RFID
//             door.loop();        // tu dong khoa sau DOOR_OPEN_TIME
//   Kich ban trom -> !door.getDoorState() khi cua vat ly mo (SensorManager)
//   Kich ban quet the -> door.getLastScanResult() de goi AlertManager / DisplayUI

#define RFID_NO_CARD   0
#define RFID_ACCEPTED  1
#define RFID_REJECTED  2

class DoorSystem {
public:
    void init();
    void checkRFID();
    void unlockDoor();
    void lockDoor();
    void loop();

    bool getDoorState();
    int getLastScanResult();

private:
    bool doorState = false;
    unsigned long doorOpenTime = 0;
    int lastScanResult = RFID_NO_CARD;
};

#endif
