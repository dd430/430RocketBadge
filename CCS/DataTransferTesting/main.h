/*
 * main.h
 *
 *  Created on: Feb 3, 2014
 *      Author: Christopher
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <msp430.h>
#define LOW_THRESHOLD 60
#define HIGH_THRESHOLD 270
#define FLASH_STORAGE_START 0xEFE0
#define FLASH_STORAGE_LENGTH 0X1000




volatile char lightSensor = 0; //0 = data, 1 = clock
volatile int dataSensorVal = 0;
volatile int clockSensorVal = 0;
volatile int clockSensorValLast = 0;

volatile unsigned int sensorDataIndex = 0;
volatile char sensorData[20];
volatile int sensorReads[20];
volatile char mode = 0;  //0 = light sensor read, 1 = POV

volatile char SelectedPOVMessage = 0;
volatile int POVStart = 0;
volatile unsigned int POVCurIndex = 0;
volatile unsigned int POVLen = 5;
char *POV_Flash_ptr;

volatile unsigned int msgLen = 0;

void write(char data);

void switchSensor(char sensorNeeded);
void readSensor();

void ConfigureSPI();
void StartLightSensor();
void DisableLightSensor();

void WriteFlash(char MsgIndex, unsigned int ValIndex, char Value);
void CopyFlashFromTmp(char MsgIndex, unsigned int Len);
void EraseFlashGroup(char MsgIndex);

void StartPOV();

void NextPOV();

#endif /* MAIN_H_ */
