#include <Arduino.h>
#include "InstrumentShieldController.h"


InstrumentShieldController controller;

void setup() {

    controller.start();

    controller.chooseBank(InstrumentShieldController::GM1);

    Serial.begin(9600);
}



void loop() {

    controller.chooseInstrument(1);

    controller.playtone_milis(1, 30, 120);

    controller.refresh();

    delay(2000);



    /*
    controller.chooseInstrument(65);
    controller.startTone(60, 127);

    delay(1000);

    controller.endTone(60, 127);
    delay(1000);
     */

}
