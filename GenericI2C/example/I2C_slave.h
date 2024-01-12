#pragma once

#include <Arduino.h>

#define MAX_LENGTH_MESSAGE              96        // max length of a message is 96 bytes due to limitations of the CMDmessenger

class I2C_slave {

    public:
        I2C_slave();
        bool message_available();
        void init(uint8_t i2c_adr, uint32_t clock = 100000);
        int16_t getMessage(char* message);

    private:
        uint8_t _i2cAdr;
};
