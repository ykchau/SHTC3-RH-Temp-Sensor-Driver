#ifndef SHTC3_H
#define SHTC3_H

#include <arduino.h>
#include <Wire.h>

#define SHTC3_ADDRESS       0x70
#define SHTC3_TIMEOUT       15  // 15ms, typically it responds in 12~14 ms in normal mode

// Acknowledge command
#define HEADER_WRITE    0xE0
#define HEADER_READ     0xE1

/* Wakup command */
#define WAKE_H          0x35
#define WAKE_L          0x17

/* Sleep mode command */
#define SLEEP_H         0xB0
#define SLEEP_L         0x98

/* Soft reset command */
#define SOFT_RST_H      0x80
#define SOFT_RST_L      0x5D

/* measurement command */
// Clock Streching Enabled, Normal Mode
#define CSE_TEMP_H      0x7C
#define CSE_TEMP_L      0xA2
#define CSE_RH_H        0x5C
#define CSE_RH_L        0x24

// Clock Streching Enabled, Low Power Mode
#define CSE_LP_TEMP_H   0x64
#define CSE_LP_TEMP_L   0x58
#define CSE_LP_RH_H     0x44
#define CSE_LP_RH_L     0xDE

// Clock Streching Disabled, Normal Mode
#define CSD_TEMP_H      0x78
#define CSD_TEMP_L      0x66
#define CSD_RH_H        0x58
#define CSD_RH_L        0xE0

// Clock Streching Disabled, Low Power Mode
#define CSD_LP_TEMP_H   0x60
#define CSD_LP_TEMP_L   0x9C
#define CSD_LP_RH_H     0x40
#define CSD_LP_RH_L     0x1A

// mode
#define MODE_NORMAL         0
#define MODE_LOWPOWER       1
#define CS_DISABLED         0
#define CS_ENABLED          1

// First reading of feedback
#define SHTC3_TEMPFIRST     0
#define SHTC3_RHFIRST       1


class SHTC3 {
    private:
        byte firstReading = SHTC3_TEMPFIRST;
        byte clockStretching = CS_DISABLED;
        
        uint8_t CRC_CHECK(uint16_t DAT, uint8_t CRC_DAT);
        void I2C_Write(byte command_H, byte command_L);
        void I2C_Read();
    public:
        float Temp, RH;
        void wakeup();
        void sleep();
        void getTempFirst(byte mode, byte clockStretch);
        void getRHFirst(byte mode, byte clockStretch);

};

#endif // SHTC3_H