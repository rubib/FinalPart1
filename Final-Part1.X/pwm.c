/*
 * File:   adc.h
 * Author: MiguelAngel
 *
 * Created on March 23, 2015, 11:49 AM
 */

#include "p24FJ64GA002.h"
#include "pwm.h"
#include "timer.h"

void initPWMLeft(){

    TMR3 = 0;
    PR3 = 1475; //Using 100us timedelay and a Prescalar of 1
    T3CONbits.TCKPS = 0; //Prescalar of 1
    OC1CONbits.OCTSEL = 1;// using timer 3
    OC1CONbits.OCM = 6;
    RPOR0bits.RP1R = 18; //OC1 18 Output Compare 1 Pin 5
    ODCBbits.ODB2 = 1; //Output Drain Collectore Pin 6

    OC1R = 0; //very important
    OC1RS = 14;
    T3CONbits.TON = 1;
}

void initPWMRight(){
    OC2CONbits.OCTSEL = 1; // using timer 3
    OC2CONbits.OCM = 6;

    RPOR1bits.RP3R = 19; //OC2 18 Output Compare 2 on Pin 7
    ODCBbits.ODB4 = 1; //Output Drain Collectore on Pin 11

    OC2R = 0; //very important
    OC2RS = 14;
}

void spinForward(){
    /*
     FOR LEFT
     */
    RPOR1bits.RP2R = 0;  // Sets Pin 6 PWM to NULL. This stops Pin 6 from using PWM
    ODCBbits.ODB1 = 0;  // Pin 5
    RPOR0bits.RP1R = 18; // Pin 5
    ODCBbits.ODB2 = 1; // Pin 6
    OC1RS = (unsigned int) (PR3); //for left. Full Speed.
    // TODO: Check this line

    /*
     FOR RIGHT
     */
    RPOR2bits.RP4R = 0; // Pin 11
    ODCBbits.ODB3 = 0;  // Pin 7
    RPOR1bits.RP3R = 19; // Pin 7
    ODCBbits.ODB4 = 1; // Pin 11
    OC2RS = (unsigned int) (PR3); // for right. Full Speed.
    // TODO: check this line

}


void turnRight(){
    /*
     FOR LEFT
     */
    RPOR1bits.RP2R = 0;  // Sets Pin 6 PWM to NULL. This stops Pin 6 from using PWM
    ODCBbits.ODB1 = 0;  // Pin 5
    RPOR0bits.RP1R = 18; // Pin 5
    ODCBbits.ODB2 = 1; // Pin 6

    /*
     FOR RIGHT
     */
    RPOR2bits.RP4R = 0; // Pin 11
    ODCBbits.ODB3 = 0;  // Pin 7
    RPOR1bits.RP3R = 19; // Pin 7
    ODCBbits.ODB4 = 1; // Pin 11

}

void turnLeft(){
    /*
     FOR LEFT
     */
    RPOR1bits.RP2R = 0;  // Sets Pin 6 PWM to NULL. This stops Pin 6 from using PWM
    ODCBbits.ODB1 = 0;  // Pin 5
    RPOR0bits.RP1R = 18; // Pin 5
    ODCBbits.ODB2 = 1; // Pin 6

    /*
     FOR RIGHT
     */
    RPOR2bits.RP4R = 0; // Pin 11
    ODCBbits.ODB3 = 0;  // Pin 7
    RPOR1bits.RP3R = 19; // Pin 7
    ODCBbits.ODB4 = 1; // Pin 11

}

//NOT NECESSARY FOR LINE FOLLOWING ------------------------------------
void spinBackward(){
    RPOR0bits.RP1R = 0; // Pin 5. set forward to 0
    ODCBbits.ODB2 = 0;  // Pin 6

    RPOR1bits.RP2R = 18; // Pin 6 set forward to 0
    ODCBbits.ODB1 = 1; //Pin 5


    RPOR1bits.RP3R = 0; // PIN 7
    ODCBbits.ODB4 = 0; // Pin 11
    RPOR2bits.RP4R = 19; // Pin 11
    ODCBbits.ODB3 = 1; // Pin 7
}

void idleFunction(){
    OC1RS = 0;
    OC2RS = 0;
}
