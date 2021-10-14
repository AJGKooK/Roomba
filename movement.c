/*
 * movement.c
 *
 *      Author: JSchirm, CStahl, AGoff, HGhitan, RWillis
 */

#include "open_interface.h"
#include "movement.h"

char data[100];

int currX = 0;
int currY = 0;

char full_map[500][500];


void move_forwards(oi_t *sensor, int centimeters) {

    double sum = 0;

    // move forward; full speed

    centimeters = (int) (centimeters / 2.0) + 1;


    while (sum < (9.4 * centimeters)) { // This runs the while loop with a 9.4 calibration
        oi_setWheels(125, 125);
        oi_update(sensor);
        if (edge_detector(sensor)) {
            break;
        }
        if (sensor->bumpLeft) {

            back_up(sensor, 10);
            turn_clockwise(sensor, 15);
            sprintf(data, "\n\rLeft bumper HIT!\n\r");
            uart_sendStr(data);

            //onBumperLeft(sensor, restOfDistance);
            break;
        }
        if (sensor->bumpRight) {

            back_up(sensor, 10);
            turn_cclockwise(sensor, 15);
            sprintf(data, "\n\rRight bumper HIT!\n\r");
            uart_sendStr(data);
            //onBumperRight(sensor, restOfDistance);
            break;
        }

        sum += sensor->distance;



    }

    oi_setWheels(0, 0); // stop
    //oi_free(sensor_data);

}



/**
 * This handles the event where the left bumper is hit.
 */
void onBumperLeft(oi_t *sensor, int restOfDistance) {

    back_up(sensor, 15);

    turn_clockwise(sensor, 90);

    move_forwards(sensor, 25);

    turn_cclockwise(sensor, 90);

    move_forwards(sensor, restOfDistance);

}

/**
 * This handles the event where the right bumper is hit.
 */
void onBumperRight(oi_t *sensor, int restOfDistance) {

    back_up(sensor, 15);

    turn_cclockwise(sensor, 90);

    move_forwards(sensor, 25);

    turn_clockwise(sensor, 90);

    move_forwards(sensor, restOfDistance);

}



void turn_cclockwise(oi_t *sensor, int degrees){ // degress = 66 ==> 90 degree turn


    double sum = 0;


    while (sum < degrees) {             // start here.
        oi_setWheels(125,-125); // turn wheels

        oi_update(sensor);
        if (edge_detector(sensor))
        {
            unsafe_clockwise(sensor, degrees);
        }
        sum += sensor->angle;
    }
    oi_setWheels(0, 0); // stop

}


void turn_clockwise(oi_t *sensor, int degrees){  // degress = 66 ==> 90 degree turn

    double sum = 0;

    while (sum < degrees) {// start here.
        oi_setWheels(-125,125); // move forward; full speed

        oi_update(sensor);
        if (edge_detector(sensor))
        {
            unsafe_counterclockwise(sensor, degrees);
        }
        sum -= sensor->angle;
    }

    oi_setWheels(0, 0); // stop

}

void unsafe_counterclockwise(oi_t *sensor, int degrees)
{
    double sum = 0;


    while (sum < degrees) {             // start here.
        oi_setWheels(125,-125); // turn wheels

        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0, 0); // stop

}


void unsafe_clockwise(oi_t *sensor, int degrees) {
    double sum = 0;

    while (sum < degrees)
    { // start here.
        oi_setWheels(-125, 125); // move forward; full speed

        oi_update(sensor);
        sum -= sensor->angle;
    }
    oi_setWheels(0, 0); // stop
}


void back_up(oi_t *sensor, int centimeters){

    double sum = 0;


    while (sum < centimeters * 10) {
        oi_setWheels(-125, -125); // move backwards; full speed
        oi_update(sensor);
        //edge_detector(sensor);
        sum -= sensor->distance;
    }
    oi_setWheels(0, 0); // stop
}


int edge_detector(oi_t *sensor){



    if(sensor->cliffLeftSignal >= 2700 || sensor->cliffLeftSignal < 100){
        oi_setWheels(0, 0);
        back_up(sensor, 15);
        turn_clockwise(sensor, 5);
        sprintf(data, "\n\rEdge detected! Cliff Left Sensor (turn right)\n\r");
            uart_sendStr(data);
        return 1;
    }
    else if(sensor->cliffRightSignal >= 2500 || sensor->cliffRightSignal < 100){
        oi_setWheels(0, 0);
        back_up(sensor, 15);
        turn_cclockwise(sensor, 5);
        sprintf(data, "\n\rEdge detected! Cliff Right Sensor (turn left)\n\r");
            uart_sendStr(data);
        return 1;
    }
    else if(sensor->cliffFrontLeftSignal >= 2500 || sensor->cliffFrontLeftSignal < 100){
        oi_setWheels(0, 0);
        back_up(sensor, 15);
        turn_clockwise(sensor, 5);
        sprintf(data, "\n\rEdge detected! Cliff Left Sensor (turn right)\n\r");
            uart_sendStr(data);
        return 1;
    }
    else if(sensor->cliffFrontRightSignal >= 2400 || sensor->cliffFrontRightSignal < 100){
        oi_setWheels(0, 0);
        back_up(sensor, 15);
        turn_cclockwise(sensor, 5);
        sprintf(data, "\n\rEdge detected! Cliff Right Sensor (turn left)\n\r");
            uart_sendStr(data);
        return 1;
    }


    oi_update(sensor);


    return 0;

}



