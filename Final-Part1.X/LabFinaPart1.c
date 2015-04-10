/* 
 * File:   LabFinaPart1.c
 * Author: Rubi Ballesteros
 *
 * Created on April 8, 2015, 12:40 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24FJ64GA002.h"
#include "lcd.h"
#include "timer.h"
#include "adc.h"
#include "pwm.h"
#include "initSW.h"
#include <stdio.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

#define true 1   // define true to use with bool data type.
#define false 0  // define false for bool data type.
#define bool int // defines the boolean data type.

#define PRESSED 0 //DOUBLE CHECK THIS it is for switch rb15
#define RELEASED 1 // Define the released state of the switch.

#define black 0
#define white 1


//Thresholds that come from what the sensors read based on their position
//random three digit numbers assigned right now as place holders (will change)
//should serve as guides for the position of the car with respect to the line.
#define changeThreshold 1000
#define OnWhiteLeft_Sensor 230
#define OnBlackMiddle_Sensor 250
#define OnWhiteRight_Sensor 230

//Pins used for the sensors:
//left 23, middle 24, right 25
//Rb 12-14

typedef enum stateTypeEnum
{
    // Define states by name
        forward,
        turnLeftState,
        turnRightState,
        //turnAround, //TODO: Make the robot turn around 180 degrees
        wait,

} stateType;

/*
 * 
 */
//Waiting until the switch is pressed
volatile stateType currState = wait;

volatile int sensorLeft;
volatile int sensorMiddle;
volatile int sensorRight;


int main(void) {
    //Initialize components
    initPWMLeft();
    initPWMRight();
    initADC();
    initLCD();
    initSW1();
    clearLCD();


    while (1){
       //make the LCD display the reading from all three sensors

       //Case Statement
       switch (currState){
            case wait:
                idleFunction();
                break;
            case forward:
                spinForward();
                break;
            case turnRightState:
                turnRight(); //change the function
                break;
            case turnLeftState:
                turnLeft();  //change function
                break;
//            case turnAround:
//                turnAround(); //change function
//                break;
       }

    }
}

//Interrupt to get out of the Wait state
void _ISR _CNInterrupt(void) {
    IFS1bits.CNIF = 0; //put the flag down
//    delayMs(10);

     if(_RB5 == PRESSED){
         if (currState == wait){
            currState = forward;
         }
         if (currState != wait){
             currState = wait;
         }
     }
     
}

/*
    This interrupts is the one that "listens" the infrared sensors and decides which way to turn
 depending on which sensors are picking up signals.
 */
void _ISR _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0;
    if(ADC1BUFA < changeThreshold){
        sensorRight = black;
    }
    else {
        sensorRight = white;
    }

    if(ADC1BUFB < changeThreshold){
        sensorMiddle = black;
    }
    else {
        sensorMiddle = white;
    }

    if(ADC1BUFC < changeThreshold){
        sensorLeft = black;
    }
    else {
        sensorLeft = white;
    }


//    sensorRight = ADC1BUFA;
//    sensorMiddle = ADC1BUFB;
//    sensorLeft = ADC1BUFC;


    if(currState != wait){
        if (sensorMiddle == black && sensorLeft == white && sensorRight == white){ //If we are looking at the black line
            currState = forward;    //keep moving forward
        }
        else if (sensorLeft == white && sensorRight == black){
            currState = turnRightState;
        }
        else if (sensorLeft == black && sensorRight == white){
            currState = turnLeftState;
        }
    }


}