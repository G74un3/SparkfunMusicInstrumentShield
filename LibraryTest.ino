#include <Arduino.h>
#include "InstrumentShieldController.h"

//FOr choosing instrument bank
#define GM1 0
#define GM2 1

InstrumentShieldController controller;




void setup() {

    controller.start();

    controller.chooseBank(GM1);

}



void loop() {

    controller.chooseInstrument(1);

    controller.chooseInstrument(65);
    controller.startTone(60, 127);

    delay(1000);

    controller.endTone(60, 127);
    delay(1000);


}
