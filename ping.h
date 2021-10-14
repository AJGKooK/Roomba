/*
 * ping.h
 *
 *  Created on: Oct 17, 2019
 *      Author: cstahl
 */

#ifndef PING_H_
#define PING_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>


void ping_init(void);

int ping_read(void);

void pulse(void);

void timer3B_init(void);
void TIMER3B_HANDLER();


#endif /* PING_H_ */
