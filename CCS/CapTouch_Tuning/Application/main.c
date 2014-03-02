//******************************************************************************
// RO_PINOSC_TA0_WDTp_Proximity_Sensor example
// Trigger the Proximity Sensor to turn on/off the center button LED
// RO method capactiance measurement using PinOsc IO, TimerA0, and WDT+
//
//          Schematic Description: 
// 
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
// 
//        The WDT+ interval represents the measurement window.  The number of 
//        counts within the TA0R that have accumulated during the measurement
//        window represents the capacitance of the element. This is lowest 
//        power option with either LPM3 (ACLK WDTp source) or LPM0 (SMCLK WDTp 
//        source).
//
//******************************************************************************
#include "CTS_Layer.h"

// Uncomment to have this compiler directive run characterization functions only
// Comment to have this compiler directive run example application
//#define ELEMENT_CHARACTERIZATION_MODE

#define DELAY 250 		// Timer delay timeout count, 250*0.1msec = 25 msec +
                        // 24 msec for measurement

#ifdef ELEMENT_CHARACTERIZATION_MODE
// Delta Counts returned from the API function for the sensor during characterization
uint16_t dCnt[6];
#endif

uint16_t position, oldPosition;
uint8_t onState, blState;

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
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  BCSCTL1 = CALBC1_1MHZ;                // Set DCO to 1, 8, 12 or 16MHz
  DCOCTL = CALDCO_1MHZ;
  BCSCTL1 |= DIVA_0;                    // ACLK/1 [ACLK/(0:1,1:2,2:4,3:8)]
  BCSCTL2 |= DIVS_3;                    // SMCLK/8 [SMCLK/(0:1,1:2,2:4,3:8)]
  BCSCTL3 |= LFXT1S_2;                  // LFXT1 = VLO Clock Source
  
  P1OUT = 0x00;							// Drive all Port 1 pins low
  P1DIR = 0xFF;							// Configure all Port 1 pins outputs
  
  P2SEL &= ~(BIT6 + BIT7);				// Configure XIN (P2.6) and XOUT (P2.7) to GPIO
  P2OUT = 0x00;							// Drive all Port 2 pins low
  P2DIR = 0xFF;							// Configure all Port 2 pins outputs

  // Initialize Baseline measurement
  TI_CAPT_Init_Baseline(&slider);
  
  // Update baseline measurement (Average 5 measurements)
  TI_CAPT_Update_Baseline(&slider,5);

  // Main loop starts here
  while (1)
  {
  	
  	#ifdef ELEMENT_CHARACTERIZATION_MODE
	// Get the raw delta counts for element characterization 
	TI_CAPT_Custom(&slider,dCnt);
	__no_operation(); 					// Set breakpoint here	
	#endif
	
	
	#ifndef ELEMENT_CHARACTERIZATION_MODE

	position = TI_CAPT_Slider(&slider);
	__no_operation();

	if(position == ILLEGAL_SLIDER_WHEEL_POSITION)
	{
		/* Turn Off LEDs */
		__no_operation();
	}
	else
	{
        /* Process position */
		if(oldPosition == ILLEGAL_SLIDER_WHEEL_POSITION)
		{
			__no_operation();
		}
		else
		{
			position = (oldPosition+position)/2;
			/* Update LEDs */
		}
        sleep(DELAY);
	}
	oldPosition = position;
        
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

#pragma vector=PORT2_VECTOR,             \
  PORT1_VECTOR,                          \
  TIMER0_A1_VECTOR,                      \
  NMI_VECTOR,COMPARATORA_VECTOR,         \
  ADC10_VECTOR
__interrupt void ISR_trap(void)
{
  // the following will cause an access violation which results in a PUC reset
  WDTCTL = 0;
}
