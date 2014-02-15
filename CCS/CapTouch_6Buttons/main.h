/*
 * main.h
 *
 *  Created on: Nov 30, 2013
 *      Author: Christopher
 */

#ifndef MAIN_H_
#define MAIN_H_

#define P1LEDBITS BIT0|BIT1|BIT2|BIT3
#define P2LEDBITS BIT0|BIT4|BIT1

void setLEDs(char value);

void setLEDsRandom();

void write(char data);


#endif /* MAIN_H_ */
