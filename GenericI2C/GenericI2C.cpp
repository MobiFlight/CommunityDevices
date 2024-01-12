#include "GenericI2C.h"
#include <Wire.h>

#define END_OF_I2C_PAYLOAD              0x00
#define END_OF_I2C_MESSAGE_ID           0x0D
#define END_OF_I2C_PARTIAL_MESSAGE      0x0A
// mega has only 32 bytes buffer for receiving via I2C
// so limit the maximal number of bytes
// keep one for additional message marker and one for stop message
#define SEND_MAX_I2C_BYTES              30

GenericI2C::GenericI2C(uint8_t addrI2C)
{
    _addrI2C = addrI2C;
    _initialised = true;
}

void GenericI2C::begin()
{
    Wire.begin();
    Wire.setClock(400000);
}

void GenericI2C::attach()
{
}

void GenericI2C::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}


void GenericI2C::set(int16_t messageID, char *payload)
{
    /* **********************************************************************************
        MessageID and payload will be send via I2C
        For AVR's the I2C buffer is only 32 bytes, so the complete message gets spilt up if exceeding
        max. length of a message could be 96 bytes due to limitation from the CMDmessenger
        Important Remark!
        MessageID == -1 will be send from the connector when Mobiflight is closed
        MessageID == -2 will be send from the connector when PowerSavingMode is entered
    ********************************************************************************** */
    char buffer[7] = {0};
    uint8_t countChar = 0;
	uint8_t countI2C = 0;

    itoa(messageID, buffer, 10);
    Wire.beginTransmission(_addrI2C);
    Wire.print(buffer);
    // send a marker for end of messageID
    Wire.write(END_OF_I2C_MESSAGE_ID);
    // count already transferred bytes incl. marker for end of messageID
    countI2C = strlen(buffer) + 1;
    while (countChar < strlen(payload)) {
        Wire.write(payload[countChar++]);
        countI2C++;
        // if buffer will be exceeded on next characater
        if (countI2C >= SEND_MAX_I2C_BYTES) {
            // send a marker for next part of payload
            Wire.write(END_OF_I2C_PARTIAL_MESSAGE);
            // write buffer to I2C display
            Wire.endTransmission();
            // and prepare a new transmission
            Wire.beginTransmission(_addrI2C);
            // start new Byte counting
            countI2C = 0;
        }
    }
    // send a NULL to mark end of messageID
    Wire.write(END_OF_I2C_PAYLOAD);
    Wire.endTransmission();
}

void GenericI2C::update()
{
    // Do something which is required regulary
}
