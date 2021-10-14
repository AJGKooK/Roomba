/*
 * ping.c
 *
 *  Created on: Oct 17, 2019
 *      Author: JSchirm, CStahl, AGoff, HGhitan, RWillis
 */

#include "ping.h"
#include "timer.h"
#include "lcd.h"
#include <stdbool.h>
#include "stdint.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

volatile enum {LOW, HIGH, DONE} state;
volatile unsigned rising_time; // start time of the return pulse
volatile unsigned falling_time; // end time of the return pulse

void ping_init(void){

    SYSCTL_RCGCGPIO_R  |= 0b00000010; // ENABLE PORT B FOR ADC0
    timer_waitMillis(1);// Small delay before accessing device after turning on clock

    GPIO_PORTB_DEN_R |= 0b00001000; // enable digital en.
    GPIO_PORTB_PCTL_R  |= 0x7000; // pin 3 will select either I2c0sda(3) or T3ccp1(7


    // Enables and provides a clock to 16/32-bit general-purpose timer module 3 in run mode.
    SYSCTL_RCGCTIMER_R |= 0b001000;
    timer_waitMillis(1);// Small delay before accessing device after turning on clock

    //: GPTM (General Purpose Timer) Control
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;  // bit 8 = GPTM TIMER B ENABLE : 0 = disable : 1 = enable.

    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH; // sets tbevent to both edges... just positive edge = 00    negatice edge = 01;

    //: GPTM Configuration
    TIMER3_CFG_R |= TIMER_CFG_16_BIT; //For 16/32-bit timer, this value selects the 16-bit timer config.
    //: GPTM Timer n Mode (n is A or B)

    TIMER3_TBMR_R |= (TIMER_TBMR_TBCMR | TIMER_TBMR_TBMR_CAP); // bit 3 = counts down (0b0)  bit 2 = count time mode(0b1)   bit 1-0 = capture mode (0x3) //KEEP CHECK LATER
    //: GPTM (General Purpose Timer) Control


    //: GPTM Timer n Interval Load
    TIMER3_TBILR_R = 0xFFFF;  // 32 bits all used.
    //: Prescaller
    TIMER3_TBPR_R = 0xFF; // assign FF


    //: GPTM Interrupt Clear Register
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

    //: GPTM Interrupt Mask Register
    TIMER3_IMR_R  |= TIMER_IMR_CBEIM; //bit 10 set to 1 (interrupt is enabled)

    NVIC_PRI9_R |= 0x20;

    NVIC_EN1_R |= 0b10000;

    IntRegister(INT_TIMER3B,TIMER3B_HANDLER);
    TIMER3_CTL_R |= TIMER_CTL_TBEN;
    IntMasterEnable();


}

void pulse(void){

    TIMER3_IMR_R &= ~TIMER_IMR_CBEIM;

    GPIO_PORTB_AFSEL_R &= 0b11110111;  // disABLE DIGITAL PB3
    GPIO_PORTB_DIR_R |= 0b00001000; // pin 3 = output


    GPIO_PORTB_DATA_R &= 0b11110111;
    GPIO_PORTB_DATA_R |= 0b00001000;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= 0b11110111;

    GPIO_PORTB_DIR_R &= ~0b00001000; // pin 3 = input

    GPIO_PORTB_AFSEL_R |= ~0b11110111;  // enable PCTL
    GPIO_PORTB_PCTL_R  |= 0x7000;

    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

    TIMER3_IMR_R |= TIMER_IMR_CBEIM;

}



int ping_read(void){

    pulse();

    while(state != DONE){

    }
    state = LOW;




    int distance = (rising_time - falling_time) * 0.0010625;    //* 0.0000000625) / 2) * 0.034)           * 0.0010625



    return distance;
}



void TIMER3B_HANDLER(){

    if(state == LOW){
        state = HIGH;
        rising_time = TIMER3_TBR_R;
    }

    else if(state == HIGH){
        state = DONE;
        falling_time = TIMER3_TBR_R;
    }

    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

}









