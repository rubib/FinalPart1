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

#define changeThreshold 1000 //The threshold at which the black line is detected

//Pins used for the sensors:
//left 23, middle 24, right 25
//Rb 12-14

//FSM STATES--------------------
typedef enum stateTypeEnum
{
        forward,
        turnLeftState,
        turnRightState,
        //TODO: Make the robot turn around 180 degrees
        //turnAround, 
        wait,

} stateType;


//Waiting until the switch is pressed.
volatile stateType currState = wait;

volatile int sensorLeft;
volatile int sensorMiddle;
volatile int sensorRight;

volatile int linesDetected = 0 ;


int main(void) {
    //Initialize components
    initPWMLeft();
    initPWMRight();
    initADC();
    initLCD();
    initSW1();
    clearLCD();


    while (1){
       //Make the LCD display the reading from all three sensors.
       moveCursorLCD(0,0);
       printStringLCD((char) sensorLeft);
       moveCursorLCD(0,1);
       printStringLCD((char) sensorMiddle);
       moveCursorLCD(0,2);
       printStringLCD((char) sensorRight);
       
       delayMs(10);

       //Case Statement: FSM
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
    IFS0bits.AD1IF = 0; //Put the interrupt flag down

 //Set threshold to turn readings from ADC buffer into a 0 or 1-------------

    if(ADC1BUFA < changeThreshold){ //Buffer of pin 25
        sensorRight = black;
    }
    else {
        sensorRight = white;
    }

    if(ADC1BUFB < changeThreshold){ //Buffer of pin 24
        sensorMiddle = black;
    }
    else {
        sensorMiddle = white;
    }

    if(ADC1BUFC < changeThreshold){ //Buffer of pin 23
        sensorLeft = black;
    }
    else {
        sensorLeft = white;
    }
    
//-------------------------------------------------------------------------

//    sensorRight = ADC1BUFA;
//    sensorMiddle = ADC1BUFB;
//    sensorLeft = ADC1BUFC;

//Decide on state based on sensor reading. Right sensor has priority.---------
    if(currState != wait){
        //If the sensors are exactly on the line and outside the line.
        if (sensorMiddle == black && sensorLeft == white && sensorRight == white){ 
            currState = forward;    //keep moving forward
        }

        /*TODO: Threshold check for 180 turn
        else if(sensorMiddle == black && sensorLeft == black && sensorRight == white){
            linesDetected++;
            if (linesDetected == 3){
                currState = turnAround;
            }
        }
         */

        //If the sensor on the right detects a curve.
        else if (sensorLeft == white && sensorRight == black){
            currState = turnRightState;
        }
        //If the sensor on the left detects a curve.
        else if (sensorLeft == black && sensorRight == white){
            currState = turnLeftState;
        }

    }
//----------------------------------------------------------------------------

}