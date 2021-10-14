/*
 * uart.c
 *
 *      Author: JSchirm, CStahl, AGoff, HGhitan, RWillis
 */
 
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "uart.h"


volatile char PuttyString[21];
volatile char charIndex = 0;

volatile char interrupt_receive_flag = 0;

char hello[25];
char clear[] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
int i = 0;

volatile char queueBit = ' ';

void uart_init(void){

    SYSCTL_RCGCGPIO_R |= 0b00000010;
    timer_waitMillis(1);// Small delay before accessing device after turning on clock

    GPIO_PORTB_AFSEL_R |= 0b00000011;
    GPIO_PORTB_PCTL_R &= 0xFFFFFFFE;
    GPIO_PORTB_PCTL_R |= 0x00000011;    // Force 1's in the disired locations GPIO_PORTB_PCTL_R |= 0b10001;
    GPIO_PORTB_DEN_R |= 0b00000011;
    GPIO_PORTB_DIR_R &= 0b11111110;
    GPIO_PORTB_DIR_R |= 0b00000010;

    SYSCTL_RCGCUART_R |= 0b00000010;
    timer_waitMillis(1);

    UART1_CTL_R &= ~(UART_CTL_UARTEN); // disable uarten
    UART1_IBRD_R = 8;
    UART1_FBRD_R = 44;
    UART1_LCRH_R = 0b01100000;
    UART1_CC_R = UART_CC_CS_SYSCLK;
    UART1_CTL_R |= UART_CTL_UARTEN; // enable uarten
}

void uart_sendChar(char data){
    while((UART1_FR_R & 0x20));
    UART1_DR_R = data;
}

char uart_receive(void){
    char data = 0;
    while((UART1_FR_R & UART_FR_RXFE)){

    }
    data = (char)(UART1_DR_R & 0xFF);
    return data;

}

void uart_sendStr(const char *data){
    //for reference see lcd_puts from lcd.c file
    while(*data != '\0')
    {
        uart_sendChar(*data);
        data++;
    }
}

void uart_init_interrupts() {

    // Set Baud Rate to something else
    // UART1_IBRD_R = 8;
    // UART1_FBRD_R = 44;

    UART1_CTL_R &= ~(UART_CTL_UARTEN);

    //clear interrupt flags
    UART1_ICR_R = (UART_ICR_TXIC | UART_ICR_RXIC);

    //enable send and receive raw interrupts
    UART1_IM_R |= UART_IM_TXIM | UART_IM_RXIM;

    //enable interrupt for IRQ 6 set bit 6
    NVIC_EN0_R |= 0b01000000;

    //tell cpu to use ISR handler for uart1
    IntRegister(INT_UART1, UART1_Handler);

    //enable global interrupts
    IntMasterEnable();
    //re-enable enable RX, TX, and uart1
    UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}

void UART1_Handler(void)
{
    queueBit = uart_receive();
}

void setQueueBit(char bit) {
    queueBit = bit;
}

char getQueueBit() {
    return queueBit;
}
