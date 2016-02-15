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
#define MAXTONES 2 //used for autostop of tones in the playTone methods. More tones means more memory used!

int _ID = 0;

SoftwareSerial _shield(2, 3);

const InstrumentShieldController::Tone *tones[MAXTONES];


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

/**
 * Responsible for choosing the bank of instruments
 * GM1: is the main bank of melodic instruments
 * GM2: is the percussion bank
 */
void InstrumentShieldController::chooseBank(ToneBank bank) {

    switch (bank) {
        case 0:
            return talkMIDI(0xB0, 0, 0x00); //Default bank GM1
        case 1:
            return talkMIDI(0xB0, 0, 0x78); //Bank select drums
        default:
            return talkMIDI(0xB0, 0, 0x79); //Bank select Melodic
    }

}

/**
 * Takes an instrument number from the table in the datasheet and makes the VS1053 choose that instrument
 */
void InstrumentShieldController::chooseInstrument(byte instrumentnumber) {

    talkMIDI(0xC0, instrumentnumber, 0); //Set instrument number. 0xC0 is a 1 data byte command
}


/**
 * 
 *  * @param note from F#-0 (30) to F#-5 (90):
 */
void InstrumentShieldController::startTone(byte note, byte velocity) {

    noteOn(0, note, velocity);

}

/*
*/
void InstrumentShieldController::endTone(byte note, byte velocity) {

    noteOff(0, note, velocity);

}


/**
 ** Plays the tone in question for a given number of seconds. Max number of concurrent tones can be set by MAXTONES in the top.
 * requires that refresh is called minimum every millisecond
 * @param note from F#-0 (30) to F#-5 (90):
 */
void InstrumentShieldController::playtone_seconds(int seconds, byte note, byte velocity) {

    playtone_milis(seconds * 1000, note, velocity);

}

/**
 ** Plays the tone in question for a given number of milli. Max number of concurrent tones can be set by MAXTONES in the top.
 * requires that refresh is called minimum every millisecond
 * @param note from F#-0 (30) to F#-5 (90):
 */
void InstrumentShieldController::playtone_milis(int milliseconds, byte note, byte velocity) {

    if (note > 128 && velocity > 128) return;

    Tone tone = {_ID, millis(), milliseconds, velocity, note}; // make new tone object
    _ID++;




    bool success = insertToneInArray(tone);


    tone = *tones[0];

    Serial.println("PLADS 0 : :: : :: :: : :: : :: ");
    Serial.print("ID: ");
    Serial.println(tone.ID);
    Serial.print("NOTE: ");
    Serial.println(tone.note);
    Serial.print("VELOCITY: ");
    Serial.println(tone.velocity);


    if(success) {

        startTone(note, velocity); // start playing tone

    }

}

/**
 * Places the tone in the array
 */
bool InstrumentShieldController::insertToneInArray(Tone tone) {

    int index = findFirstAvailableIndex();

    Serial.print("index: ");
    Serial.println(index);

    if(index >= 0 ) {

        tones[index] = &tone; // save a reference to the pointer to tone so we can find it again and kill it in time

        return true;

    }

    return false;

}

/**
 * Finds the first empty slot int the currentPlaying array
 * Returns -1 if no empty place is found
 */
int InstrumentShieldController::findFirstAvailableIndex() {


    for (int i = 0; i < MAXTONES; i++) {

        if (tones[i]) { // the pointer is not null and therefore the place in the array is full

        } else { // There is an empty place

            return i; // we found it
        }
    }

    return -1; // Array completely full

}

/**
 * Looks throiugh all playing tones and ends those that are not suposed to be playing
 */
void InstrumentShieldController::refresh() {

    unsigned long time = millis(); //saves current timestamp

    for (int i = 0; i < MAXTONES; ++i) {

        if(!tones[i]) continue;

        Tone tone = *tones[i];


        Serial.println("AFTER: ::: :: :: : : :: : ");
        Serial.print("ID: ");
        Serial.println(tone.ID);
        Serial.print("NOTE: ");
        Serial.println(tone.note);
        Serial.print("VELOCITY: ");
        Serial.println(tone.velocity);


        if(time - tone.timeStamp  > tone.deadlineMillis) { // The tone has been running for to long and should be killed

            endTone(tone.note, tone.velocity);

            noteOff(0, tone.note, tone.velocity);


        }

    }


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