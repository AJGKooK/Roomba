/*
 * servo.h
 *
 *  Created on: Oct 24, 2019
 *      Author: cstahl
 */

#ifndef SERVO_H_
#define SERVO_H_


void servo_init(void);
int servo_move(float degrees);
void servo_move_with_button(uint8_t button);

#endif /* SERVO_H_ */
