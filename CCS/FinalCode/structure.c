//******************************************************************************
//	defintion of cap touch elements and calibration values
//******************************************************************************

#include "structure.h"

const struct Element elementL = {

	.inputPxselRegister = (unsigned char *)&P1SEL,
	.inputPxsel2Register = (unsigned char *)&P1SEL2,
	.inputBits = BIT4,
	.maxResponse = 520,
	.threshold = 15
};
const struct Element elementA = {

	.inputPxselRegister = (unsigned char *)&P1SEL,
	.inputPxsel2Register = (unsigned char *)&P1SEL2,
	.inputBits = BIT5,
	.maxResponse = 601,
	.threshold = 15
};
const struct Element elementU = {

	.inputPxselRegister = (unsigned char *)&P1SEL,
	.inputPxsel2Register = (unsigned char *)&P1SEL2,
	.inputBits = BIT6,
	.maxResponse = 681,
	.threshold = 15
};
const struct Element elementN = {

	.inputPxselRegister = (unsigned char *)&P1SEL,
	.inputPxsel2Register = (unsigned char *)&P1SEL2,
	.inputBits = BIT7,
	.maxResponse = 605,
	.threshold = 15
};
const struct Element elementC = {

	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT5,
	.maxResponse = 609,
	.threshold = 15
};
const struct Element elementH = {

	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT6,
	.maxResponse = 235,
	.threshold = 100
};



//*** Sensor   *******************************************************/
// This defines the grouping of sensors, the method to measure change in
// capacitance, and the function of the group

// Proximity Sensor
const struct Sensor slider =
               { 
                  .halDefinition = RO_PINOSC_TA0_WDTp,
                  .numElements = 6,
                  .baseOffset = 0,
                  // Pointer to elements
                  .arrayPtr[0] = &elementH,
                  .arrayPtr[1] = &elementC,
                  .arrayPtr[2] = &elementN,
                  .arrayPtr[3] = &elementU,
                  .arrayPtr[4] = &elementA,
                  .arrayPtr[5] = &elementL,
                  .points = 254,
                  .sensorThreshold = 15,
                  // Timer Information
                  .measGateSource= GATE_WDT_SMCLK,     //  0->SMCLK, 1-> ACLK
                  .accumulationCycles= WDTp_GATE_64             //512
               };
