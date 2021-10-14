/*
*
*   uart.h
*
*   Used to set up the UART
*   uses UART1 at 115200
*
*
*   @author Dane Larson
*   @date 07/18/2016
*   Phillip Jones updated 9/2019, removed WiFi.h
*/

#ifndef UART_H_
#define UART_H_

#include "Timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "movement.h"
#include "scan.h"

void uart_init(void);

void uart_sendChar(char data);

char uart_receive(void);

void uart_sendStr(const char *data);

void UART1_Handler(void);

void uart_init_interrupts(void);

char getQueueBit();

void setQueueBit(char bit);

#endif /* UART_H_ */
