/*
 * scan.h
 *
 *  Created on: Nov 21, 2019
 *      Author: cstahl
 */

#ifndef SCAN_H_
#define SCAN_H_


void process_entry(float degrees, double irDist, int sonarDist);
struct Package find_smallest();
int print_to_putty(void);
int auto_move_request(void);



#endif /* SCAN_H_ */
