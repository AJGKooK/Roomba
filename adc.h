/*
 * adc.h
 *
 *  Created on: Oct 10, 2019
 *      Author: cstahl
 */

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "timer.h"
#include <math.h>


void adc_init(void);
int adc_read(void);
double distance(void);



#endif /* ADC_H_ */
