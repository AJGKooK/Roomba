/*
 * adc.c
 *
 *  Created on: Oct 10, 2019
 *      Author: JSchirm, CStahl, AGoff, HGhitan, RWillis
 */

#include "adc.h"

double curr_irvalue;

void adc_init(void){


    SYSCTL_RCGCGPIO_R  = SYSCTL_RCGCGPIO_R | 0b00000010; // ENABLE PORT B FOR ADC0
    timer_waitMillis(1);// Small delay before accessing device after turning on clock

    GPIO_PORTB_AFSEL_R |= 0b00010000;  // ENABLE DIGITAL PB4

    GPIO_PORTB_DIR_R |= 0b11101111;

    GPIO_PORTB_DEN_R &= 0b11101111; // disable digital en.

    GPIO_PORTB_AMSEL_R = 0b00010000; // ENABLE ANALOG PB4



  //  GPIO_PORTB_ADCCTL_R = 0x00;

    SYSCTL_RCGCADC_R |= 0b1;  //ENABLE CLOCK FOR ADC 0
    timer_waitMillis(5);

    //sample sequencer 1 initialize ss0
    ADC0_ACTSS_R = 0xFFFE; //  disable ss0
    ADC0_EMUX_R |= 0x0; //?? ISSUE HERE!!!!
    ADC0_SSMUX0_R |= 0x000A;
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0  | ADC_SSCTL0_END0); // ie0 or end0
    ADC0_SAC_R |= ADC_SAC_AVG_64X;  // 64x hardware oversampling
    ADC0_ACTSS_R = 0b0001; // enable ss0
}





int adc_read(void){

    //disable ADC0SS0 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
    //set 1st sample to use the channel ADC pin
    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
    //initiate SS0 conversion
    ADC0_PSSI_R = ADC_PSSI_SS0;
    //wait for ADC conversion to be complete
    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){}
    //clear interrupt
    ADC0_ISC_R=ADC_ISC_IN0;

    return ADC0_SSFIFO0_R;

}


double distance() {
    double out;
    int i;
    float avg;
    float rollingAvg;
    int curr;

    for (i = 0; i < 100; i++) {
        curr = adc_read(); // Gets the IR Value each call
        if (i > 25 && !(rollingAvg - curr > -8) && !(rollingAvg - curr < 8)) { // Checks to see if the new value is within tolerance.
            i--; // If not within tolerance redoes the calculation
        } else {
            avg += adc_read(); // adds the current value to the double average
            rollingAvg = avg / i; // Gets the rolling average for tolerance purposes
        }
    }
    avg = avg / 100.0; // Gets the overall average which will be used in the below equation.
    curr_irvalue = avg;
    out = (117067.16965955 / pow(avg, 1.1806375442739)); // The equation that we found
    return out;
}
