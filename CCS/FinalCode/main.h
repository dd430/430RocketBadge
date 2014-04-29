/*
 * main.h
 *
 *  Created on: Feb 3, 2014
 *      Author: Christopher
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <stdint.h>
#include <msp430.h>

#define LOW_THRESHOLD 60
#define HIGH_THRESHOLD 200
#define FLASH_STORAGE_START 0xEFE0
#define FLASH_STORAGE_LENGTH 0X1000

#define MODE_CAP_DEMO 0
#define MODE_POV 1
#define MODE_PROGRAM 2

volatile char mode = 0;  //0 = Cap Touch Demo, 1 = POV,
volatile char povActive = 0;
volatile char readingCapTouch;
void SetMode(char NewMode);


volatile char lightSensor = 0; //0 = data, 1 = clock
volatile int dataSensorVal = 0;
volatile int clockSensorVal = 0;
volatile int clockSensorValLast = 0;

volatile char sensorDataIndex = 0;
volatile char sensorData[20];
volatile int sensorReads[20];


volatile char SelectedPOVMessage = 0;
volatile int POVStart = 0;
volatile char POVCurIndex = 0;
volatile char POVLen = 5;
char *POV_Flash_ptr;

volatile char msgLen = 0;

void write(char data);

void switchSensor(char sensorNeeded);
void readSensor();

void ConfigureLEDs();
void DisableLEDs();
void EnableLEDs();
void StartLightSensor();
void DisableLightSensor();

void WriteFlash(char MsgIndex, unsigned int ValIndex, char Value);
void CopyFlashFromTmp(char MsgIndex, unsigned int Len);
void EraseFlashGroup(char MsgIndex);

void ReadCapTouch();

void StartPOV(char whichMessage);
void StopPOV();


char NextPOV();

//Elements from Slider examples
#define DELAY 1000 		// Timer delay timeout count, 250*0.1msec = 25 msec +
                        // 24 msec for measurement
volatile uint16_t position, oldPosition;
uint8_t onState, blState;

#endif /* MAIN_H_ */
