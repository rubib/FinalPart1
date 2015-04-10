/*
 * File:   adc.h
 * Author: Miguel
 *
 * Created on February 24, 2015, 9:24 AM
 */

#include "p24FJ64GA002.h"
#include "adc.h"

void initADC(){

    //AD1PCFGbits.PCFG0 = 0;     //Set pin 2 to analog

    AD1PCFGbits.PCFG10 = 0; // Configure pin 25 to analog
    AD1PCFGbits.PCFG11 = 0; // Configure pin 24 to analog
    AD1PCFGbits.PCFG12 = 0; // Configure pin 23 to analog

    AD1CON1bits.FORM = 0;      //Data Output Format bits set as integers
    AD1CON1bits.SSRC = 7;      //Conversion Trigger Source Select bits, Internal counter ends sampling and starts conversion (auto-convert)
    AD1CON1bits.ASAM = 1;      //sample automatically
    AD1CON2bits.VCFG = 0;
    AD1CON2bits.CSCNA = 0;     //no scan
    AD1CON2bits.SMPI = 0b1111; //Interrupts at the completion of conversion for each 16th sample/convert sequence
    AD1CON2bits.ALTS = 0;      //Always uses MUX A input multiplexer settings
    AD1CON3bits.ADRC = 0;      //Clock derived from system clock
    AD1CON3bits.SAMC = 1;      //Auto-Sample Time bits
    AD1CON3bits.ADCS = 10;     //A/D Conversion Clock Select bits
    AD1CHS = 0;                //Configure input channels,
    AD1CSSL = 0;               //Channel scanning is disabled anyway.
    AD1CON1bits.ADON = 1;      // Turn on A/D


    IEC0bits.AD1IE = 1;     //Enable the interrupt
    IFS0bits.AD1IF = 0;     //Put the flag down
}