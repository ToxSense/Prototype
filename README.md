# Learning Atmospheres - ToxSense 

## ESP32 - CAM (Headband)

The main headband processor consists of an ESP32-CAM-module. The combination of a camera (OV2640) , a combined wifi/bluetooth chip and enough general purpose input output (GPIO) pins to connect the amplifiers with the body bass exciters made us opt for the microcontroller . Its programming language can be c, c++ or Assembler, but with the often used Arduino IDE it is mostly common to use predefined functions already implemented for usage and correlating libraries. The rough program concept is split in a setup and a loop part. In the setup function all used parts are initiated, so that they are ready to use in the following loop part. There the script reruns in a defined interval, repeating its tasks continuously. 

For usage, the finished, programmed script is flashed into the board's memory via an USB-to-serial adapter.

To use the built in camera module different pre definitions are set, like the picture format in CIF, which stands for a resolution of 352 x 288 Pixel. This format is used to limit the computing resources to a minimum, whilst maintaining a quality high enough for inference actions made by the AIs. For the bluetooth connection between the ESP32-CAM module and the smartphone a serialport is established, with an intern arduino library. This form of connections allows it to stream simple data strings in an efficient way. To receive such strings, a callback listener is implemented, which detects an incoming stream right away.

The magnetic sensor, which provides the users orientation, is connected to two GPIO pins of the microprocessor. It provides its data through an I2C protocol connection, which also has to be set up in the beginning. For the initialisation of the magnetic sensor a library is used to set its register. As the last setup step, the ledcWrite channels, whose define the PWM signals for the body bass exciter are defined, with a custom frequency and bit resolution for the duty cycles. 

In the following loop part of the script, the bluetooth listener detects commands sent by the app and executes special functions connected to them. The most important triggers the camera to take a picture . This picture is encoded to a base64 string, enabling it to be sent over the serialized bluetooth connection to the smartphone. Another command provides the direction based aqi values and updates the PWM channels according to the users heading.

All used hardware parts take in a power supply about five voltages, which can be easily provided over USB, connected to a power bank.

## Credits

This project is imagined and created by Timo Bilhöfer, Markus Pfaff and Maria Rădulescu.

As part of the seminar *Learning Atmospheres* in the winter-semester 2020/21 it is supported by Irina Auernhammer, Silas Kalmbach and Prof. Lucio Blandini from the[ Institute for Lightweight Structures and Conceptual Design (**ILEK**)](https://www.ilek.uni-stuttgart.de/) and is also part of the[ Collaborative Research Centre 1244 (**SFB 1244**)](https://www.sfb1244.uni-stuttgart.de/).