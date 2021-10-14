/*
 * servo.c
 *
 *  Created on: Oct 24, 2019
 *      Author: JSchirm, CStahl, AGoff, HGhitan, RWillis
 */

#include "timer.h"
#include "lcd.h"
#include <stdbool.h>
#include "stdint.h"
#include <inc/tm4c123gh6pm.h>
#include "servo.h"
#include "button.h"
#include "open_interface.h"

float oneDeg = 146.2;

volatile int cycle = 180;
volatile int pulse_width;
int flag = 0;
float new, old;

volatile enum {CLOCKWISE, COUNTERCLOCKWISE} state;

void servo_init(void)
{

    SYSCTL_RCGCGPIO_R |= 0b00000010; // Enable and provide a clock to GPIO Port B in Run mode
    GPIO_PORTB_DEN_R |= 0b00100000; // enable digital en.
    GPIO_PORTB_DIR_R |= 0b00100000; //  0 Corresponding pins is an output... pin 5.
    GPIO_PORTB_AFSEL_R |= 0b00100000;
    GPIO_PORTB_PCTL_R |= 0x00700000;

    // Enables and provides a clock to 16/32-bit general-purpose timer module 1 in run mode.
    SYSCTL_RCGCTIMER_R |= 0b000010;
    //: GPTM (General Purpose Timer) Control
    TIMER1_CTL_R &= ~TIMER_CTL_TBEN; // bit 8 = GPTM TIMER B ENABLE : 0 = disable : 1 = enable.
    //: GPTM Configuration
    TIMER1_CFG_R |= TIMER_CFG_16_BIT; //For 16/32-bit timer, this value selects the 16-bit timer config.
    //: GPTM Timer n Mode (n is A or B)
    TIMER1_TBMR_R |= 0xA; //(TIMER1_TBMR_R & 0b1110110) | 0b00001010; // bit 3 = counts down (0b1)  bit 2 = count time mode(0b0)   bit 1-0 = capture mode (0x2)

    TIMER1_TBPR_R = 0x04; // assign FF
    TIMER1_TBILR_R = 0xE200;  // 32 bits all used.

    TIMER1_TBPMR_R = 0x04;
    TIMER1_TBMATCHR_R = 0x8DA0; // 90 degrees

    servo_move(0);

    TIMER1_CTL_R |= TIMER_CTL_TBEN;

    state = CLOCKWISE;

}

int servo_move(float cake)
{
//    unsigned int newval;
//    newval = (TIMER1_TBPMR_R << 16) + TIMER1_TBMATCHR_R - (160 * cake);
//
//    TIMER1_TBMATCHR_R = (newval);
//    TIMER1_TBPMR_R = (newval) >> 16;
//
//    return 0;
    TIMER1_TBMATCHR_R  = -146.2 * (cake) + 50000;
    return 0;
}

void servo_move_with_button(uint8_t button)
{

    if (button == 1)
    {
        TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R + oneDeg;
        if(TIMER1_TBMATCHR_R > 50000){
            servo_move(0);
        }
        else if(TIMER1_TBMATCHR_R < 20256){
            servo_move(180);
        }
        lcd_printf("%d", TIMER1_TBMATCHR_R);


    }
    else if (button == 2)
    {
        TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R + oneDeg * 5;
        if(TIMER1_TBMATCHR_R > 50000){
            servo_move(0);
        }
        else if(TIMER1_TBMATCHR_R < 20256){
            servo_move(180);
        }
        lcd_printf("%d", TIMER1_TBMATCHR_R);

    }
    else if (button == 3)
    {
        if(!flag){
        oneDeg = (oneDeg * (-1));
        lcd_printf("%0.2f", oneDeg);
        }



    }
    else if (button == 4)
    {
        if(oneDeg > 0){
           servo_move(0);
        }
        else{
            servo_move(180);
        }
    }

}

