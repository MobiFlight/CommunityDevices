#include "I2C_slave.h"
#include "Wire.h"

#define END_OF_I2C_PAYLOAD              0x00
#define END_OF_I2C_MESSAGE_ID           0x0D
#define END_OF_I2C_PARTIAL_MESSAGE      0x0A


enum {
    NOT_SYNCHRONIZED = 0,
    RECEIVE_MESSAGE_ID,
    RECEIVE_PAYLOAD
};

int16_t _messageID = 0;
char _message[MAX_LENGTH_MESSAGE];
bool _message_received = false;


void onReceiveI2C(int received_bytes);

I2C_slave::I2C_slave() {}

void I2C_slave::init(uint8_t i2cAdr, uint32_t clock) {
    _i2cAdr = i2cAdr;
    // setup i2C receive callback
    Wire.onReceive(onReceiveI2C);

    // init I2C busses
    Wire.begin(_i2cAdr);
    if (clock != 400000)
        clock = 100000;
    Wire.setClock(clock);
}

bool I2C_slave::message_available() {
    return _message_received;
}

int16_t I2C_slave::getMessage(char* message) {
    strncpy(message, _message, MAX_LENGTH_MESSAGE);
    _message_received = false;
    return _messageID;
}

// callback function for I2C receive
void onReceiveI2C(int received_bytes) {

  static uint8_t byte_counter = 0;
  static uint8_t state = RECEIVE_MESSAGE_ID;
  // range is -32768 ... 32767 -> max. 6 character plus terminating NULL
  char buffer[7] = {0};

  for (uint8_t i = 0; i < received_bytes; i++) {
    switch (state) {
    case NOT_SYNCHRONIZED:
      // wait for end of message to get synchronized
      if (Wire.read() == END_OF_I2C_PAYLOAD) {
        state = RECEIVE_MESSAGE_ID;
        byte_counter = 0;
      }
      break;

    case RECEIVE_MESSAGE_ID:
      buffer[i] = Wire.read();
      if (buffer[i] == END_OF_I2C_MESSAGE_ID) {
        // terminate string
        buffer[i] = 0x00;
        _messageID = atoi(buffer);
        // next bytes are Data Bytes
        state = RECEIVE_PAYLOAD;
      }
      // buffer overflow for messageID
      if (i >= 6) {
        // something went wrong, get a new synchronization
        state = NOT_SYNCHRONIZED;
        byte_counter = 0;
        return;
      }
      break;

    case RECEIVE_PAYLOAD:
      _message[byte_counter] = Wire.read();
      // end of message detected, prepare for receiving next messageID
      if (_message[byte_counter] == END_OF_I2C_PAYLOAD) {
        byte_counter = 0;
        state = RECEIVE_MESSAGE_ID;
        _message_received = true;
        return;
      } else if(_message[byte_counter] == END_OF_I2C_PARTIAL_MESSAGE) {
        // end of partial message detected, next transmission will be rest of message
        // so keep receiving data
        return;
      } else {
        // get the next byte
        byte_counter++;
      }
      break;

    default:
      break;
    }
  }
  // We shouldn't come here, something went wrong
  state = NOT_SYNCHRONIZED;
}
