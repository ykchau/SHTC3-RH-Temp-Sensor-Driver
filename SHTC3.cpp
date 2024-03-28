#include "SHTC3.h"

/**************************
   Private Functions
**************************/
void SHTC3::I2C_Write(byte command_H, byte command_L) {
    Wire.beginTransmission(SHTC3_ADDRESS);
    Wire.write(HEADER_WRITE);
    Wire.endTransmission();
    Wire.beginTransmission(SHTC3_ADDRESS);
    Wire.write(byte(command_H));
    Wire.write(byte(command_L));
    Wire.endTransmission();
}

void SHTC3::I2C_Read() {
    Wire.beginTransmission(SHTC3_ADDRESS);
    Wire.write(HEADER_READ);
    Wire.endTransmission();

    uint16_t reading_0 = 0;
    uint16_t reading_1 = 0;
    uint16_t reading_0_CRC = 0;
    uint16_t reading_1_CRC = 0;

    byte rec = 0;
    unsigned long startTime = millis();

    if (clockStretching == CS_DISABLED) {
        // Because slave response NACK, it will keep silence until the measurement complete, 
        // so we need to keep sending request until it response
        while (rec == 0 ) {
            rec = Wire.requestFrom(SHTC3_ADDRESS, 6);

            if ((millis() - startTime) > SHTC3_TIMEOUT) {
                break; // quit if timeout
            }
        }
    } else {
        Wire.requestFrom(SHTC3_ADDRESS, 6);
    }

    if (2 <= Wire.available()) {
        reading_0 = Wire.read();
        reading_0 = reading_0 << 8;
        reading_0 |= Wire.read();
        reading_0_CRC = Wire.read();
    }

    if (2 <= Wire.available()) {
        reading_1 = Wire.read();
        reading_1 = reading_1 << 8;
        reading_1 |= Wire.read();
        reading_1_CRC = Wire.read();
    }

    if (firstReading == SHTC3_TEMPFIRST) {
        Temp = CRC_CHECK(reading_0, reading_0_CRC) ? (float(reading_0) * 175 / 65536) - 45 : 255;
        RH = CRC_CHECK(reading_1, reading_1_CRC) ? float(reading_1) * 100 / 65536 : 255;
    } else {
        RH = CRC_CHECK(reading_0, reading_0_CRC) ? float(reading_0) * 100 / 65536 : 255;
        Temp = CRC_CHECK(reading_1, reading_1_CRC) ? (float(reading_1) * 175 / 65536) - 45 : 255;
    }
}

uint8_t SHTC3::CRC_CHECK(uint16_t DAT, byte CRC_DAT)
{
    byte temp = (DAT >> 8) & 0xFF;
    byte CRC_BYTE = 0xFF;
    
    for (byte t = 0; t < 2; t++) {
        CRC_BYTE ^= temp;
        for (byte i = 0; i < 8; i++) {
            if (CRC_BYTE & 0x80) {
                CRC_BYTE <<= 1;
                CRC_BYTE ^= 0x31;
            } else {
                CRC_BYTE <<= 1;
            }
        }

        if (t == 0) {
            temp = DAT & 0xFF;
        }
    }

    return ( CRC_BYTE == CRC_DAT ) ? 1 : 0;
}



/**************************
   Public Functions
**************************/
void SHTC3::wakeup() {
    I2C_Write(WAKE_H, WAKE_L);
}

void SHTC3::sleep() {
    I2C_Write(SLEEP_H, SLEEP_L);
}

void SHTC3::getTempFirst(byte mode, byte cs_mode) {
    firstReading = SHTC3_TEMPFIRST;
    clockStretching = cs_mode;
    byte command_H = CSD_TEMP_H;
    byte command_L = CSD_TEMP_L;

    if (mode == MODE_LOWPOWER) {
        command_H = (cs_mode == CS_ENABLED) ? CSE_LP_TEMP_H : CSD_LP_TEMP_H;
        command_L = (cs_mode == CS_ENABLED) ? CSE_LP_TEMP_L : CSD_LP_TEMP_L;
    } else {  // MODE_NORMAL
        command_H = (cs_mode == CS_ENABLED) ? CSE_TEMP_H : CSD_TEMP_H;
        command_L = (cs_mode == CS_ENABLED) ? CSE_TEMP_L : CSD_TEMP_L;
    }

    I2C_Write(command_H, command_L);
    I2C_Read();
}

void SHTC3::getRHFirst(byte mode, byte cs_mode) {
    firstReading = SHTC3_RHFIRST;
    clockStretching = cs_mode;
    byte command_H = CSD_RH_H;
    byte command_L = CSD_RH_L;

    if (mode == MODE_LOWPOWER) {
        command_H = (cs_mode == CS_ENABLED) ? CSE_LP_RH_H : CSD_LP_RH_H;
        command_L = (cs_mode == CS_ENABLED) ? CSE_LP_RH_L : CSD_LP_RH_L;
    } else {  // MODE_NORMAL
        command_H = (cs_mode == CS_ENABLED) ? CSE_RH_H : CSD_RH_H;
        command_L = (cs_mode == CS_ENABLED) ? CSE_RH_L : CSD_RH_L;
    }

    I2C_Write(command_H, command_L);
    I2C_Read();
}

