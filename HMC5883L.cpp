// HMC Sensor Libs by Timo Bilhöfer based on:
// I2Cdev library collection - HMC5883L I2C device class
// Based on Honeywell HMC5883L datasheet, 10/2010 (Form #900405 Rev B)
// 6/12/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2021-03-10 - simplification for ToxSense by Timo Bilhöfer
//     2012-06-12 - fixed swapped Y/Z axes
//     2011-08-22 - small Doxygen comment fixes
//     2011-07-31 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "HMC5883L.h"
#include "Wire.h"

#ifndef BUFFER_LENGTH
// band-aid fix for platforms without Wire-defined BUFFER_LENGTH (removed from some official implementations)
#define BUFFER_LENGTH 32
#endif

HMC5883L::HMC5883L() {
    devAddr = HMC5883L_DEFAULT_ADDRESS;
}

void HMC5883L::initialize() {	
    writeByte(devAddr, HMC5883L_RA_CONFIG_A,
        (HMC5883L_AVERAGING_8 << (HMC5883L_CRA_AVERAGE_BIT - HMC5883L_CRA_AVERAGE_LENGTH + 1)) |
        (HMC5883L_RATE_15     << (HMC5883L_CRA_RATE_BIT - HMC5883L_CRA_RATE_LENGTH + 1)) |
        (HMC5883L_BIAS_NORMAL << (HMC5883L_CRA_BIAS_BIT - HMC5883L_CRA_BIAS_LENGTH + 1)));
    setGain(HMC5883L_GAIN_1090);
    setMode(HMC5883L_MODE_SINGLE);
}

void HMC5883L::setGain(uint8_t gain) {
    writeByte(devAddr, HMC5883L_RA_CONFIG_B, gain << (HMC5883L_CRB_GAIN_BIT - HMC5883L_CRB_GAIN_LENGTH + 1));
}

void HMC5883L::setMode(uint8_t newMode) {
    writeByte(devAddr, HMC5883L_RA_MODE, newMode << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    mode = newMode; // track to tell if we have to clear bit 7 after a read
}

void HMC5883L::getHeading(int16_t *x, int16_t *y, int16_t *z) {
    readBytes(devAddr, HMC5883L_RA_DATAX_H, 6, buffer);
    if (mode == HMC5883L_MODE_SINGLE) writeByte(devAddr, HMC5883L_RA_MODE, HMC5883L_MODE_SINGLE << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[4]) << 8) | buffer[5];
    *z = (((int16_t)buffer[2]) << 8) | buffer[3];
}

bool HMC5883L::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    return writeBytes(devAddr, regAddr, 1, &data);
}

bool HMC5883L::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data) {
    uint8_t status = 0;
    Wire.beginTransmission(devAddr);
    Wire.write((uint8_t) regAddr); // send address
    for (uint8_t i = 0; i < length; i++) {
            Wire.write((uint8_t) data[i]);
    }
    status = Wire.endTransmission();
    return status == 0;
}

int8_t HMC5883L::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout) {
    int8_t count = 0;
    uint32_t t1 = millis();
    Wire.begin(3,1);
    for (uint8_t k = 0; k < length; k += min((int)length, BUFFER_LENGTH)) {
        Wire.beginTransmission(devAddr);
        Wire.write(regAddr);
        Wire.endTransmission();
        Wire.beginTransmission(devAddr);
        Wire.requestFrom(devAddr, (uint8_t)min(length - k, BUFFER_LENGTH));
        for (; Wire.available() && (timeout == 0 || millis() - t1 < timeout); count++) {
            data[count] = Wire.read();
        }
    }
    if (timeout > 0 && millis() - t1 >= timeout && count < length) count = -1; // timeout
    return count;
}