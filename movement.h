/*
 * movement.h
 *
 *  Created on: Sep 12, 2019
 *      Author: cstahl
 */
#include "open_interface.h"
#include "uart.h"
#ifndef MOVEMENT_H_
#define MOVEMENT_H_

void move_forwards(oi_t *sensor,int centimeters);
void back_up(oi_t *sensor, int centimeters);
void lateral(oi_t *sensor, int centimeters);
void turn_clockwise(oi_t *sensor,int centimeters);
void turn_cclockwise(oi_t *sensor,int centimeters);
void onBumperLeft(oi_t * sensor, int centi);
void onBumperRight(oi_t * sensor, int centi);

int edge_detector(oi_t *sensor);

void unsafe_clockwise(oi_t *sensor, int degrees);
void unsafe_counterclockwise(oi_t *sensor, int degrees);


#endif /* MOVEMENT_H_ */
