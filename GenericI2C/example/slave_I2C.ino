#include <Arduino.h>
#include "I2C_slave.h"

#define I2C_MOBIFLIGHT_ADDR             0x27

int16_t messageID = 0;                  // will be set to the messageID coming from the connector
char message[MAX_LENGTH_MESSAGE];       // contains the message which belongs to the messageID
I2C_slave i2c_slave;

void setup() {
  Serial.begin(115200);

  // init I2C busses
  i2c_slave.init(I2C_MOBIFLIGHT_ADDR, 400000);
}

void loop() {

  if (i2c_slave.message_available()) {
    messageID = i2c_slave.getMessage(message);
    switch (messageID)
    {
    case -1:
      // received messageID is 0
      // data is a string in message[] and 0x00 terminated
      // do something with your received data
      Serial.print("MessageID is -1 and Payload is: "); Serial.println(message);
      break;

    case 0:
      // received messageID is 0
      // data is a string in message[] and 0x00 terminated
      // do something with your received data
      Serial.print("MessageID is 0 and Payload is: "); Serial.println(message);
      break;

    case 1:
      // received messageID is 1
      // data is a string in message[] and 0x00 terminated
      // do something with your received data
      Serial.print("MessageID is 1 and Payload is: "); Serial.println(message);
      break;

    case 2:
      // received messageID is 2
      // data is a string in message[] and 0x00 terminated
      // do something with your received data
      Serial.print("MessageID is 2 and Payload is: "); Serial.println(message);
      break;
    
    default:
      break;
    }
  }
}
