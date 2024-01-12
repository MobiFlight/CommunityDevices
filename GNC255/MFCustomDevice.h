#pragma once

#include <Arduino.h>
#include "GNC255.h"

class MFCustomDevice
{
public:
    MFCustomDevice();
    void attach(int16_t adrPin, uint16_t adrType, uint16_t adrConfig);
    void detach();
    void update();
    void set(uint8_t messageID, char *setPoint);

private:
    bool    getStringFromEEPROM(uint16_t addreeprom, char *buffer);
    bool    _initialized = false;
    GNC255 *_mydevice;
};