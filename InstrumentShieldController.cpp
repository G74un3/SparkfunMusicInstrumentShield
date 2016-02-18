/*InstrumentShieldController.cpp
 * 14/02/2016
 * Library written for the course Fysisk Design on Aarhus University by Daniel "Graunefar" Graungaard
 *
 * Based on the 2011 Sparkfun Library by Nathan Seidle & Marc "Trench" Tschudin: https://dlnmh9ip6v2uc.cloudfront.net/assets/4/9/1/2/0/512276a2ce395f9712000000.zip
 */

#include <Arduino.h>
#include "InstrumentShieldController.h"
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>


#define resetMIDI 4
#define ledPin 13


int _ID = 0;

SoftwareSerial _shield(2, 3);

InstrumentShieldController::InstrumentShieldController() {


}

void InstrumentShieldController::start() {

    //Setup soft serial for MIDI control
    _shield.begin(31250);

    //Reset the VS1053
    pinMode(resetMIDI, OUTPUT);
    digitalWrite(resetMIDI, LOW);
    delay(100);
    digitalWrite(resetMIDI, HIGH);
    delay(100);
    talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)

}


void InstrumentShieldController::chooseBank(int bank) {

    chooseBank(bank, 0);

}


/**
 * Responsible for choosing the bank of instruments
 * GM1: is the main bank of melodic instruments
 * GM2: is the percussion bank
 */
void InstrumentShieldController::chooseBank(byte bank, byte channel) {

    switch (bank) {
        case 0:
            return talkMIDI(0xB0, 0, 0x00); //Default bank GM1
        case 1:
            return talkMIDI(0xB0, 0, 0x78); //Bank select drums
        default:
            return talkMIDI(0xB0, 0, 0x79); //Bank select Melodic
    }

}


void InstrumentShieldController::chooseInstrument(byte instrumentnumber) {

    cooseInstrument(instrumentnumber, 0);

}

/**
 * Takes an instrument number from the table in the datasheet and makes the VS1053 choose that instrument
 */
void InstrumentShieldController::chooseInstrument(byte instrumentnumber, byte channel) {

    talkMIDI(0xC0, instrumentnumber, 0); //Set instrument number. 0xC0 is a 1 data byte command
}


void InstrumentShieldController::startTone(byte note, byte velocity) {

    startTone(note, velocity, 0);

}


/**
 * 
 *  * @param note from F#-0 (30) to F#-5 (90):
 */
void InstrumentShieldController::startTone(byte note, byte velocity, byte channel) {

    noteOn(channel, note, velocity);

}


void InstrumentShieldController::endTone(byte note, byte velocity) {

    endTone(note, velocity, 0);

}

/*
*/
void InstrumentShieldController::endTone(byte note, byte velocity, byte channel) {

    noteOff(channel, note, velocity);

}


//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void InstrumentShieldController::noteOn(byte channel, byte note, byte attack_velocity) {
    talkMIDI((0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void InstrumentShieldController::noteOff(byte channel, byte note, byte release_velocity) {
    talkMIDI((0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void InstrumentShieldController::talkMIDI(byte cmd, byte data1, byte data2) {
    digitalWrite(ledPin, HIGH);
    _shield.write(cmd);
    _shield.write(data1);

    //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
    //(sort of: http://253.ccarh.org/handout/midiprotocol/)
    if ((cmd & 0xF0) <= 0xB0)
        _shield.write(data2);

    digitalWrite(ledPin, LOW);
}