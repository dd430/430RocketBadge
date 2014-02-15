/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
// RO_PINOSC_TA0_WDTp_Wheel_Buttons example
// Touch the 6 wheel buttons to turn on/off the center button LED
// RO method capactiance measurement using PinOsc IO, TimerA0, and WDT+
//
//          Schematic Description: 
// 
//                         MSP430G2452
//                      +---------------+
//                      |
//             C--------|P2.4
//           C----------|P2.1
//               C------|P2.3
//             C--------|P2.2
//                      |
//           C----------|P2.5
//                      |
//           C----------|P2.0
// 
//        The WDT+ interval represents the measurement window.  The number of 
//        counts within the TA0R that have accumulated during the measurement
//        window represents the capacitance of the element. This is lowest 
//        power option with either LPM3 (ACLK WDTp source) or LPM0 (SMCLK WDTp 
//        source).
//
//******************************************************************************
#include "CTS_Layer.h"
#include "main.h"

// Uncomment to have this compiler directive run characterization functions only
// Comment to have this compiler directive run example application
//#define ELEMENT_CHARACTERIZATION_MODE	

#define DELAY 5000 		// Timer delay timeout count, 5000*0.1msec = 500 msec

struct Element * keyPressed;			// Pointer to the Element structure

#ifdef ELEMENT_CHARACTERIZATION_MODE
// Delta Counts returned from the API function for the sensor during characterization
unsigned int wheelCnt[5];				// Becuase the Wheel is composed of five elements
#endif

// Sleep Function
// Configures Timer A to run off ACLK, count in UP mode, places the CPU in LPM3 
// and enables the interrupt vector to jump to ISR upon timeout 
void sleep(unsigned int time)
{
    TA0CCR0 = time;
    TA0CTL = TASSEL_1+MC_1+TACLR;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 |= CCIE; 
    __bis_SR_register(LPM3_bits+GIE);
    __no_operation();
}

// Main Function
void main(void)
{ 
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;

	if (CALBC1_1MHZ != 0xFF) {
		/* Follow recommended flow. First, clear all DCOx and MODx bits. Then
		 * apply new RSELx values. Finally, apply new DCOx and MODx bit values.
		 */
		DCOCTL = 0x00;
		BCSCTL1 = CALBC1_1MHZ;      /* Set DCO to 1MHz */
		DCOCTL = CALDCO_1MHZ;
	}
	BCSCTL1 |= XT2OFF | DIVA_0;
	BCSCTL3 = XT2S_0 | LFXT1S_2 | XCAP_1;

	//Configure unused port - not present on DIP
	P3DIR = 0;
	P3OUT = 0;
  
  P1OUT = 0x00;							// Drive all Port 1 pins low
  P1DIR = 0xFF;							// Configure all Port 1 pins outputs
  
  P2SEL &= ~(BIT6 + BIT7);				// Configure XIN (P2.6) and XOUT (P2.7) to GPIO
  P2OUT = 0x00;							// Drive all Port 2 pins low
  P2DIR = 0xFF;							// Configure all Port 2 pins outputs

	//configure pins for SPI
	P1SEL2 = BIT5 | BIT7;
	P1SEL = BIT5 | BIT7;

	//Initialize USCI B for SPI
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 =  UCMST | UCMODE_0 | UCSYNC |UCCKPH|UCCKPL;
	UCB0CTL1 = UCSSEL_2 | UCSWRST;
	UCB0BR0 = 1;
	UCB0CTL1 &= ~UCSWRST;

	write(0xAA);

  // Initialize Baseline measurement
  TI_CAPT_Init_Baseline(&wheel_buttons);
  
  // Update baseline measurement (Average 5 measurements)
  TI_CAPT_Update_Baseline(&wheel_buttons,5);  

  char buttonOutput = 0;
  // Main loop starts here
  while (1)
  {
  	
  	#ifdef ELEMENT_CHARACTERIZATION_MODE
	// Get the raw delta counts for element characterization 
	TI_CAPT_Custom(&wheel_buttons,wheelCnt);
	__no_operation(); 					// Set breakpoint here	
	#endif
	
	
	#ifndef ELEMENT_CHARACTERIZATION_MODE
	// Return the pointer to the element which has been touched
	keyPressed = (struct Element *)TI_CAPT_Buttons(&wheel_buttons);
	
	// If a button has been touched, then take some action
	if(keyPressed)
	{
		buttonOutput = 0;
		// Up Element
		if(keyPressed == &L_element)
		{
			buttonOutput |= BIT2;
		}
		// Down Element
		if(keyPressed == &A_element)
		{
			buttonOutput |= BIT3;
		}
		// Left Element
		if(keyPressed == &U_element)
		{
			buttonOutput |= BIT4;
		}
		// Right Element
		if(keyPressed == &N_element)
		{
			buttonOutput |= BIT5;
		}						
		// Middle Element
		if(keyPressed == &C_element)
		{
			buttonOutput |= BIT6;
		}
		// Middle Element
		if(keyPressed == &H_element)
		{
			buttonOutput |= BIT7;
		}
		write(buttonOutput);
	  }
	  else
	  {
	      write(0x00);
	  }      
        
    // Put the MSP430 into LPM3 for a certain DELAY period
    sleep(DELAY);
    #endif

  }
} // End Main

/******************************************************************************/
// Timer0_A0 Interrupt Service Routine: Disables the timer and exists LPM3  
/******************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  TA0CTL &= ~(MC_1);
  TA0CCTL0 &= ~(CCIE);
  __bic_SR_register_on_exit(LPM3_bits+GIE);
}

//#pragma vector=PORT2_VECTOR,             \
//  PORT1_VECTOR,                          \
//  TIMER0_A1_VECTOR,                      \
//  USI_VECTOR,                            \
//  NMI_VECTOR,COMPARATORA_VECTOR,         \
//  ADC10_VECTOR
//__interrupt void ISR_trap(void)
//{
//  // the following will cause an access violation which results in a PUC reset
//  WDTCTL = 0;
//}

void write(char data) {
	P2OUT &= ~BIT6;
	while (!(IFG2 & UCB0TXIFG))
		;
	UCB0TXBUF = data;
	while (!(IFG2 & UCB0TXIFG))
	          ;
	P2OUT |= BIT6;
}
