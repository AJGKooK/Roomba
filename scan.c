/*
 * scan.c
 *
 *  Created on: Nov 21, 2019
 *      Author: JSchirm, CStahl, AGoff, HGhitan, RWillis
 */

#include <math.h>
#include "timer.h"
#include "lcd.h"
#include "servo.h"
#include "button.h"
#include "open_interface.h"
#include "uart.h"
#include "ping.h"
#include "adc.h"
#include "movement.h"

float firstFlag = 0;
float secondFlag = 0;

struct Object arr[10];
int currArrIndex = 0;
char data[100];

bool lastObject = false;
char map[50][50];
int attempt = 0;

struct Object
{
    float firstFlag;
    float secondFlag;
    double distAverageFirst;
    double distAverageSecond;
};

struct Package
{
    int index;
    double distance;
    float width;
    float angle;
};

void process_entry(float degrees, double irDist, int sonarDist)
{



    if (fabs(irDist - sonarDist) <= 9)
    {
        lastObject = true;
        attempt = 0;
        double n = (irDist + sonarDist) / 2.0;

        double x = cos(degrees * ((22.0 / 7.0) / 180.0)) * n;
        double y = sin(degrees * ((22.0 / 7.0) / 180.0)) * n;

        x /= 2;
        y /= 2;

        x += 25;

        map[(int) x][(int) y] = 'X';

//            sprintf(data, "%0.0f angle %d X %d Y  MATCH %f N\r\n", degrees, (int) x, (int) y, n);
//            uart_sendStr(data);
        if (firstFlag == 0)
        {
            arr[currArrIndex].firstFlag = degrees + 1;
            arr[currArrIndex].distAverageFirst = ((irDist + sonarDist) / 2);
            firstFlag = degrees;
        }
        else
        {
            arr[currArrIndex].secondFlag = degrees - 1;
            arr[currArrIndex].distAverageSecond = ((irDist + sonarDist) / 2);
            secondFlag = degrees;
        }
    }
    else
    {
        if (lastObject == true)
        {
            irDist = distance();
            sonarDist = ping_read();
            attempt++;
            if (attempt > 3)
            {
                lastObject = false;
            }
            process_entry(degrees, irDist, sonarDist);
        }
        else
        {
            attempt = 0;
            lastObject = false;
            firstFlag = 0;
            secondFlag = 0;
            if (arr[currArrIndex].firstFlag != 0
                    && arr[currArrIndex].secondFlag != 0)
            {
                currArrIndex++;
            }
        }
    }
}

struct Package find_smallest()
{
    struct Package p;
    double curr_smallest_size = 1000;
    int i;
    for (i = 0; i < 10; i++)
    {
        if (arr[i].firstFlag == 0)break;
        float angle = fabs(arr[i].firstFlag - arr[i].secondFlag);

        double size = sin(angle * ((22.0 / 7.0) / 180));

        float distance = ((arr[i].distAverageFirst + arr[i].distAverageSecond) / 2);

        angle = (angle / 2)  + arr[i].firstFlag;

        if (size * distance < curr_smallest_size)
        {
            curr_smallest_size = size * distance;
            p.angle = angle;
            p.index = i + 1;
            p.distance = distance;
            p.width = curr_smallest_size;
        }
        sprintf(data, "OBJECT @ \n%0.0f angle %0.3f distance (cm) %0.3f width\r\n", angle, distance,
                        (size * distance));
        uart_sendStr(data);
    }
    sprintf(data, "Smallest @ \n%0.0f angle   %0.3f dist    %0.3f\r\n", p.angle, p.distance, p.width);
    uart_sendStr(data);
    return p;
}

int print_to_putty(void)
{
    int sonarDist = 0;
    float degrees = 0;
    double irDist = 0;

    int i;
    int j;
    for (i = 0; i < 10; i++) {
        arr[i].distAverageFirst = 0;
        arr[i].distAverageSecond = 0;
        arr[i].firstFlag = 0;
        arr[i].secondFlag = 0;
    }

    for (j = 0; j < 50; j++) {
        for (i = 0; i < 50; i++) {
            map[i][j] = ' '; // Fill map with empty to start.
        }
    }

    sprintf(data, "New Data Now :\r\n");
    uart_sendStr(data);


    for (degrees = 0; degrees <= 180;)
    {
        servo_move(degrees);
        sonarDist = ping_read();
        irDist = distance();
        process_entry(degrees, irDist, sonarDist);
        timer_waitMillis(30);  //***
        if (firstFlag != 0) {
            degrees++;
        } else {
            degrees += 2;
        }
    }

    timer_waitMillis(1000);
    servo_move(0);

    for (j = 49; j > 0; j--) {
        for (i = 0; i < 50; i++) {
            uart_sendChar(map[i][j]);
            uart_sendChar(' ');
        }
        sprintf(data, "|\r\n");
        uart_sendStr(data);
    }
    for (i = 0; i < 50; i++) {
        if (i == 18) {
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('^');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
            uart_sendChar('_');
        } else {
            uart_sendChar(' ');
            uart_sendChar(' ');
        }
    }

    for (i = 0; i < 10; i++) {
            arr[i].distAverageFirst = 0;
            arr[i].distAverageSecond = 0;
            arr[i].firstFlag = 0;
            arr[i].secondFlag = 0;
    }

    firstFlag = 0;
    secondFlag = 0;
    currArrIndex = 0;

    timer_waitMillis(1000);
    return 1;
}

int auto_move_request() {
    print_to_putty();
    int i = 0;
    int j = 0;
    for (i = 0; i < 11; i++) {
        for (j = 0; j < 50; j++) {
//            map[i + 18][j] = 'L';
            if (map[i + 18][j] == 'X') {
                return 0;
            }
        }
    }
    return 1;
}

