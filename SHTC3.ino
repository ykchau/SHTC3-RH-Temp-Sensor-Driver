#include "SHTC3.h"
#define DELAY 2000

unsigned long startTime, timeUsed;
SHTC3 shtc3;


void setup() {
    Serial.begin(115200);
    Wire.begin();

    startTime = millis();
}


void loop() {
    Serial.print("LP CSD - ");
    getSHTC3Data(MODE_LOWPOWER, CS_DISABLED);
    delay(DELAY);

    Serial.print("LP CSE - ");
    getSHTC3Data(MODE_LOWPOWER, CS_ENABLED);
    delay(DELAY);

    Serial.print("NM CSD - ");
    getSHTC3Data(MODE_NORMAL, CS_DISABLED);
    delay(DELAY);

    Serial.print("NM CSE - ");
    getSHTC3Data(MODE_NORMAL, CS_ENABLED);
    delay(DELAY);
}

void getSHTC3Data(byte mode, byte clockstretch) {
    shtc3.wakeup();     // wake up the device
    shtc3.getTempFirst(mode, clockstretch);     // mesaure temp & RH
    shtc3.sleep();      // put it sleep mode for power saving after measurement

    Serial.print(String("Humidity : ") + shtc3.RH + "% Temperature : " + shtc3.Temp + " Deg C");
    timeUsed = millis() - startTime;
    startTime = millis();
    Serial.println(String(" - ") + String((timeUsed- DELAY)) + "ms");
}