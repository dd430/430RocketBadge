/*
 * main.h
 *
 *  Created on: Feb 3, 2014
 *      Author: Christopher
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <msp430.h>
#define LOW_THRESHOLD 100
#define HIGH_THRESHOLD 500

volatile char lightSensor = 0; //0 = data, 1 = clock
volatile int dataSensorVal = 0;
volatile int clockSensorVal = 0;
volatile int clockSensorValLast = 0;

volatile char sensorDataIndex = 0;
volatile char sensorData[20];
volatile int sensorReads[20];

void write(char data);

void switchSensor(char sensorNeeded);
void readSensor();


#endif /* MAIN_H_ */
