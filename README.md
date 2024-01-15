## Community Device GNC255

This custom device supports a 256x64 pixel OLED display with predifined outputs like the original GNC255.
The display uses hardware SPI for communication, so you MUST use the following pins:

Mega
* CLK: Pin 52
* Data: Pin 51
* CS: Pin 53 for the first display, for more choose as you want
* Pin 50 can NOT be used for any other device

Pico
* CLK: Pin 18
* Data: Pin 19
* CS: Pin 17 for the first display, for more choose as you want
* Pin 16 can NOT be used for any other device

For both boards D/C and Reset can be choosen as you want.

** Important Remark **
If you have already used this before, delete your config from the board before updating!
Once you have updated your board configure the display agin. CLK and Data pins must not be configured anymore as they are fixed (as before).
Also make sure to use the updated `mobiflight.gnc255.device.json` file.

Connect your display accordingly the above used pins. Depending on your display, you might need to adjust some 
jumpers on the controller board to enable the SPI interface - below is one example of such display that works
once the zero ohm resistors were moved to the positions marked with yellow. Some boards might be already configured
for SPI, so check the datasheet or sales listing carefully.

![image](https://github.com/MobiFlight/MobiFlight-CustomDevices/assets/2587818/c2368dea-6da4-47e7-84b9-33f550802159)

## Community Device GenericI2C

This device forwardes the defined messages from mobiflight.genericI2C.device.json to I2C.
If you need more than 2 messages, extend the device.json file accordingly.

Connect an I2C device to the 2wire bus to receive the informations. This device must handle the messageID and the message.
The messageID and the message is send as a string.

In the folder /example an .ino is available which shows how to receive messageID and message and how to handle this.
To get it compiled the files I2C_slave.cpp and I2C_slave.h have to be in the same folder.
