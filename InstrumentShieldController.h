/*InstrumentShieldController.h
 * 14/02/2016
 * Library written for the course Fysisk Design on Aarhus University by Daniel "Graunefar" Graungaard
 *
 * Based on the 2011 Sparkfun Library by Nathan Seidle & Marc "Trench" Tschudin: https://dlnmh9ip6v2uc.cloudfront.net/assets/4/9/1/2/0/512276a2ce395f9712000000.zip
 */


#ifndef SHIELD_LIBRARY_INSTRUMENTSHIELDCONTROLLER_H
#define SHIELD_LIBRARY_INSTRUMENTSHIELDCONTROLLER_H


class InstrumentShieldController {


public:

    enum ToneBank {

        GM1, GM2

    };

    struct Tone {

        unsigned long timeStamp;
        double deadlineMillis;
        int velocity;
        int note;
        int x;

    };

    InstrumentShieldController();


    void talkMIDI(byte cmd, byte data1, byte data2);

    void chooseInstrument(int instrumentnumber);

    void chooseBank(ToneBank bank);

    void startTone(int pitch, int velocity);

    void endTone(int pitch, int velocity);

    void playtone_milis(int miliseconds, int pitch, int velocity);

    void playtone_seconds(int seconds, int pitch, int velocity);

    void start();

private:
    void noteOn(byte channel, byte note, byte attack_velocity);

    void noteOff(byte channel, byte note, byte release_velocity);




};


#endif //SHIELD_LIBRARY_INSTRUMENTSHIELDCONTROLLER_H
