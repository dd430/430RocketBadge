//******************************************************************************
// structure.c
//                         MSP430G2553
//                      +---------------+
//                      |
//             C--------|P2.0
//           C----------|P2.1
//               C------|P2.2
//             C--------|P2.3
//           C----------|P2.4
//           C----------|P2.5
//
//             LED1-----|P1.5
//             LED2-----|P1.7
//             LED3-----|P2.6
//******************************************************************************

#include "structure.h"

const struct Element elementL = {

	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT0,
	.maxResponse = 530,
	.threshold = 10
};
const struct Element elementA = {

	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT1,
	.maxResponse = 570,
	.threshold = 10
};
const struct Element elementU = {

	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT2,
	.maxResponse = 510,
	.threshold = 10
};
const struct Element elementN = {

	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT3,
	.maxResponse = 610,
	.threshold = 10
};
const struct Element elementC = {

	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT4,
	.maxResponse = 570,
	.threshold = 10
};
const struct Element elementH = {

	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT5,
	.maxResponse = 475,
	.threshold = 10
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
