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
#include "main.h"
#include "standardMessages.h"


// Uncomment to have this compiler directive run characterization functions only
// Comment to have this compiler directive run example application
//#define ELEMENT_CHARACTERIZATION_MODE

//#define DELAY 1000 		// Timer delay timeout count, 250*0.1msec = 25 msec +
                        // 24 msec for measurement

#ifdef ELEMENT_CHARACTERIZATION_MODE
// Delta Counts returned from the API function for the sensor during characterization
uint16_t dCnt[6];
#endif

uint8_t onState, blState;



void ConfigureLEDs()
{
	P2DIR |= 0x1F; //bits 0-4
	P2OUT &= !0x1F;
}
void DisableLEDs()
{
	P2DIR &= 0xE0; //disable bits 0-4
}
void EnableLEDs()
{
	P2DIR |= 0x1f; //bits 0-4
}
void write(char data) {

	P2OUT &= !0x1F;//clear first
	P2OUT |= (data & 0x1F);//only take first 5 bits
}

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
	BCSCTL1 |= DIVA_0;                    // ACLK/1 [ACLK/(0:1,1:2,2:4,3:8)] = 1000 kHz
	BCSCTL2 = SELM_0 | DIVM_0 | DIVS_3;                    // SMCLK/8 [SMCLK/(0:1,1:2,2:4,3:8)] = 125 kHz
	BCSCTL3 |= LFXT1S_2;                  // LFXT1 = VLO Clock Source = 12 kHz

	BCSCTL1 |= XT2OFF | DIVA_0;
		BCSCTL2 = SELM_0 | DIVM_0 | DIVS_3;
		BCSCTL3 = XT2S_0 | LFXT1S_2 | XCAP_1;

	P1OUT = 0x00;							// Drive all Port 1 pins low
	P1DIR = 0xFF;							// Configure all Port 1 pins outputs

	//Configure unused port - not present on DIP20
	P3DIR = 0;
	P3OUT = 0;


	P2SEL &= ~(BIT6 + BIT7);				// Configure XIN (P2.6) and XOUT (P2.7) to GPIO
	P2OUT = 0x00;							// Drive all Port 2 pins low
	P2DIR = 0xFF;							// Configure all Port 2 pins outputs

	//light sensor setup
	P1REN |= BIT0|BIT3;
	P1OUT &= ~(BIT0|BIT3);
	P2DIR |= BIT7;
	P2OUT |= BIT7;  //turn 2.7 on - it powers the light sensors


	// Initialize Baseline measurement
	TI_CAPT_Init_Baseline(&slider);

	// Update baseline measurement (Average 5 measurements)
	TI_CAPT_Update_Baseline(&slider,5);



	ConfigureLEDs();

	_delay_cycles(10000);

	StartPOV(0);

	char button, lastButton;
	lastButton = 255;
	char buttons[6];
	char buttonIndex = 0;
	char badCount = 0;
	// Main loop starts here
  while (1)
  {
  	
  	#ifdef ELEMENT_CHARACTERIZATION_MODE
	// Get the raw delta counts for element characterization 
	TI_CAPT_Custom(&slider,dCnt);
	__no_operation(); 					// Set breakpoint here	
	#endif
	
	

	#ifndef ELEMENT_CHARACTERIZATION_MODE
/*
	ReadCapTouch();

	__no_operation();


        

*/
	ReadCapTouch();
	if(mode == MODE_POV)
	{
		if (position == ILLEGAL_SLIDER_WHEEL_POSITION || position == 0)
		{
			if(!povActive)
				StartPOV(SelectedPOVMessage);
			badCount++; //issue with slide, or took their finger off just a bit
			if(badCount > 5)
			{

				buttons[0] = 255;
				buttons[1] = 255;
				buttons[2] = 255;
				buttons[3] = 255;
				buttons[4] = 255;
				buttons[5] = 255;
				buttonIndex = 0;
				lastButton = 255;
			}
		}
		else
		{
			badCount = 0; //reset bad touch count
			StopPOV();

			button = ((255-position)/42);
			if(buttonIndex == 0 && button < 3) //first button touched and L, A or U - switch to that POV message
			{
				write(0x1F);
				_delay_cycles(100000);
				StartPOV(button);
			}
			else
			{
				write(1<<button);
				if(buttonIndex > 0 || (buttonIndex == 0 && button > 4))
				{
					if(buttonIndex == 0 || buttons[buttonIndex-1] != button) //either first button, or different from the last button
						buttons[buttonIndex] = button;
					if(buttonIndex > 0)
					{
						if(!(buttons[buttonIndex - 1] == (button + 1)) && button != lastButton) //they need to be sliding from the bottom up
						{
							StartPOV(SelectedPOVMessage);
							buttons[0] = 255;
							buttons[1] = 255;
							buttons[2] = 255;
							buttons[3] = 255;
							buttons[4] = 255;
							buttons[5] = 255;
							buttonIndex = 0;
						}
						else if(buttonIndex == 4)
						{
							//slid all  the way up the - switch modes
							write(0x1F); //indicate visually that we're switching modes
							_delay_cycles(1000000);
							SetMode(MODE_CAP_DEMO);
						}
						else if(button == lastButton)
						{
							//haven't moved on to the next button, wait for movement
						}
						else
						{
							buttonIndex++;
						}
					}
					else
					{
						buttonIndex++;//bump up to index 1
					}

				}
			}
			lastButton = button;
		}
		if(mode == MODE_POV)
		{
			if(povActive == 1)
				_BIS_SR(LPM0_bits + GIE);
			else
				sleep(DELAY);
		}

	}
	else if(mode == MODE_CAP_DEMO)
	{
		if(position == ILLEGAL_SLIDER_WHEEL_POSITION)
			{
				//Turn Off LEDs
				write(0x00);
				//__no_operation();
				sleep(DELAY);
			}
			else
			{

		        // Process position
				if(oldPosition == ILLEGAL_SLIDER_WHEEL_POSITION)
				{
					__no_operation();
				}
				else
				{
					position = (oldPosition+position)/2;
					// Update LEDs
					write(BIT4>>(position/50));
				}


		        sleep(DELAY);
				//LPM3;
			}
			oldPosition = position;
	}

	#endif
  }
} // End Main

void StartLightSensor()
{
	EraseFlashGroup(5); //clear out destination flash first


	TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
	TA1CCR0 = 250;
	TA1CTL = TASSEL_2 + MC_1;                  // SMCLK, upmode

	ADC10CTL0 &= ~ENC;

	/*
	 * Control Register 0
	 *
	 * ~ADC10SC -- No conversion
	 * ~ENC -- Disable ADC
	 * ~ADC10IFG -- Clear ADC interrupt flag
	 * ~ADC10IE -- Disable ADC interrupt
	 * ADC10ON -- Switch On ADC10
	 * REFON -- Enable ADC reference generator
	 * REF2_5V -- Set reference voltage generator = 2.5V
	 * ~MSC -- Disable multiple sample and conversion
	 * ~REFBURST -- Reference buffer on continuously
	 * ~REFOUT -- Reference output off
	 * ~ADC10SR -- Reference buffer supports up to ~200 ksps
	 * ADC10SHT_1 -- 8 x ADC10CLKs
	 * SREF_1 -- VR+ = VREF+ and VR- = VSS
	 *
	 * Note: ~<BIT> indicates that <BIT> has value zero
	 */
	ADC10CTL0 = ADC10ON | REFON | ADC10SHT_3 | SREF_0;

	/*
	 * Control Register 1
	 *
	 * ~ADC10BUSY -- No operation is active
	 * CONSEQ_2 -- Repeat single channel
	 * ADC10SSEL_0 -- ADC10OSC
	 * ADC10DIV_0 -- Divide by 1
	 * ~ISSH -- Input signal not inverted
	 * ~ADC10DF -- ADC10 Data Format as binary
	 * SHS_0 -- ADC10SC
	 * INCH_1 -- ADC Channel 1
	 *
	 * Note: ~<BIT> indicates that <BIT> has value zero
	 */
	ADC10CTL1 = CONSEQ_0 | ADC10SSEL_0 | ADC10DIV_0 | SHS_0 | INCH_0;

	/* Analog (Input) Enable Control Register 0 */
	ADC10AE0 = BIT0;


	/* Software delay for REFON to settle */
	__delay_cycles(30000);

	/* enable ADC10 */
	ADC10CTL0 |= ENC;

	SetMode(MODE_PROGRAM);
}

void DisableLightSensor()
{
    /* POWER: Turn ADC and reference voltage off to conserve power */
    ADC10CTL0 &= ~(ADC10ON | REFON);
}
void switchSensor(char sensorNeeded)
{
	ADC10CTL0 &= ~ENC;
	if(lightSensor == 0 && sensorNeeded == 1)
	{
		lightSensor = 1;
		ADC10CTL1 &= ~(INCH_0);
		ADC10CTL1 |= (INCH_3);
		ADC10AE0 = BIT3;
	}
	else if(lightSensor == 1 && sensorNeeded == 0)
	{
		lightSensor = 0;
		ADC10CTL1 &= ~(INCH_3);
		ADC10CTL1 |= (INCH_0);
		ADC10AE0 = BIT0;
	}
	ADC10CTL0 |= ENC;
}
void readSensor()
{

	static char currentVal = 0;
	static char currentIndex = 0;
	static char clockLow = 0;
	static char dataLow = 0;
	static unsigned int readsSinceChange=0;



	static char msgIndex = 0;


	//static int readSCOverflow=0;
	char ReadVal = 0x0;


	//Read Clock Sensor

	ADC10CTL0 |= ENC + ADC10SC;
	while (ADC10CTL1 & ADC10BUSY); //wait for completion
	clockSensorVal = ADC10MEM+0;
	//Read Data Sensor
	switchSensor(0);
	ADC10CTL0 |= ENC + ADC10SC;
	while (ADC10CTL1 & BUSY); //wait for completion
	dataSensorVal = ADC10MEM+0;

	if(dataSensorVal < LOW_THRESHOLD)
		dataLow = 1;
	else if(dataSensorVal > HIGH_THRESHOLD)
		dataLow = 0;
	switchSensor(1);


	readsSinceChange++;

	if(clockSensorVal < LOW_THRESHOLD && clockLow == 0) //first time the clock has dropped low since last read
	{
		clockLow = 1;
	}
	//read from data sensor


	//Clock dropped and went high again - we have a bit to read
	if(clockLow == 1 && clockSensorVal > HIGH_THRESHOLD)
	{


		//reset variable - helps us keep track of when the clock drops to low
		clockLow = 0;
		//clock transition from low to high - read value
		if(dataLow == 1)
			ReadVal = 0x0;
		else
			ReadVal = BIT0;
		currentVal |= (ReadVal & BIT0) << 7 - currentIndex; //MSB first
		currentIndex++;
		if(currentIndex > 7)
		{
			currentIndex = 0; //start next value
			if(msgLen == 0)
				sensorData[sensorDataIndex] = currentVal;
			sensorReads[sensorDataIndex] = readsSinceChange;
			readsSinceChange= 0;

			if(msgLen > 0)
			{
				WriteFlash(5, sensorDataIndex+2, currentVal); //write to temp location
			}
			if(sensorDataIndex == 4 && msgLen == 0)  //first time through, we have the full initialization sequence
			{
				if(sensorData[0] == 0xAA && sensorData[1] == 0xFF)
				{
					//which message we're storing
					msgIndex = sensorData[2];
					//length of message being transmitted
					msgLen = (sensorData[3]<<8) | sensorData[4]; //length is two bytes
					sensorDataIndex = 0;
					currentVal = 0;
					WriteFlash(5, 0, (char)((msgLen>>8)&0xFF));
					WriteFlash(5, 1, (char)(msgLen&0xFF));
				}
				else
				{
					//error in transmission - indicate error
					write(0xFF);
					_delay_cycles(1000000);
					write(0x00);
					_delay_cycles(1000000);
					write(0xFF);
					LPM4; //turn off MCU
				}
			}
			else if(msgLen > 0 && sensorDataIndex == msgLen) //reached anticipated message length
			{
				if(currentVal == 0xAA) //last bit is stop
				{
					//Yeah!  We've got a full message
					write(BIT0);
					_delay_cycles(1000000);
					CopyFlashFromTmp(msgIndex, msgLen+1); //length + 1 to grab the message length as well

					SelectedPOVMessage = msgIndex-1;
					StartPOV(SelectedPOVMessage);
				}
				else //oops - transmission problem
					LPM4;
			}
			else
			{
				sensorDataIndex++;
				dataLow = 1;
				//if(sensorDataIndex > 19)
				//	sensorDataIndex = 0;
				currentVal = 0;
			}


		}

	}
}

void SetMode(char NewMode)
{
	mode = NewMode;
	switch(NewMode)
	{
		case 0://cap touch demo
			StopPOV();
			ConfigureLEDs();
			break;
		case 1: //POV
			ConfigureLEDs();
			break;
		case 2: //POV Program
			StartLightSensor();
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
	}
}


void StartPOV(char whichMessage)
{
	SelectedPOVMessage = whichMessage;
	TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
	TA1CTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
	if(SelectedPOVMessage == 0)
	{
		POV_Flash_ptr = (char *) (START0);
		POVStart = START0; //store start position
	}
	else if(SelectedPOVMessage == 1)
	{
		POV_Flash_ptr = (char *) (START1);
		POVStart = START1; //store start position
	}
	else if(SelectedPOVMessage == 2)
	{
		POV_Flash_ptr = (char *) (START2);
		POVStart = START2; //store start position
	}


	POVLen = (*POV_Flash_ptr++)<<8;
	POVLen |= (*POV_Flash_ptr);

	*POV_Flash_ptr++; //move to the first item in the list
	POVCurIndex = 0;
	TA1CCR0 = 250;
	SetMode(MODE_POV);
	povActive = 1;
}
void StopPOV()
{
	TA1CCTL0 &= ~CCIE; //disable POV interrupt
	povActive = 0;
}

char NextPOV()
{
	if(readingCapTouch == 0)
	{
		write((*POV_Flash_ptr));
		POVCurIndex++;
		POV_Flash_ptr++;
	}
	if(POVCurIndex >= POVLen)
	{
		//ReadCapTouch();
		POV_Flash_ptr = (char *)(POVStart+2);
		POVCurIndex = 0;
		return 1;  //triggers a wake-up to act on the value of the cap touch slider
	}
	return 0;
}

//Note - expects flash segment to be erased first
void WriteFlash(char MsgIndex, unsigned int ValIndex, char Value)
{
	char *Flash_ptr;                          // Flash pointer
	if(MsgIndex == 1)
		Flash_ptr = (char *) (START0+ValIndex);              // Initialize Flash pointer
	else if(MsgIndex == 2)
		Flash_ptr = (char *) (START1+ValIndex);              // Initialize Flash pointer
	else if (MsgIndex == 3)
		Flash_ptr = (char *) (START2+ValIndex);              // Initialize Flash pointer
	else if (MsgIndex == 5)
		Flash_ptr = (char *) (STARTTMP+ValIndex);              // Initialize Flash pointer

	FCTL1 = FWKEY + ERASE;                      // Set WRT bit for write operation
	FCTL3 = FWKEY;                            // Clear Lock bit

	FCTL1 = FWKEY + WRT;

	*Flash_ptr = Value;

	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}


//Note - expects flash segment to be erased first
void CopyFlashFromTmp(char MsgIndex, unsigned int Len)
{
	char *Flash_ptr;                          // Flash pointer
	char *Flash_ptr_tmp = (char *) STARTTMP;

	EraseFlashGroup(MsgIndex); //clean out destination first

	if(MsgIndex == 1)
		Flash_ptr = (char *) START0;              // Initialize Flash pointer
	else if(MsgIndex == 2)
		Flash_ptr = (char *) START1;              // Initialize Flash pointer
	else if (MsgIndex == 3)
		Flash_ptr = (char *) START2;              // Initialize Flash pointer


	FCTL1 = FWKEY + ERASE;                      // Set WRT bit for write operation
	FCTL3 = FWKEY;                            // Clear Lock bit

	FCTL1 = FWKEY + WRT;
	unsigned int i;

	for(i = 0;i<Len;i++)
	{
		*Flash_ptr++ = (char)(*Flash_ptr_tmp++);//copy from temporary flash memory
	}



	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

void EraseFlashGroup(char MsgIndex)
{
	char *Flash_ptr;                          // Flash pointer
	FCTL1 = FWKEY + ERASE;                      // Set WRT bit for write operation
	FCTL3 = FWKEY;                            // Clear Lock bit


	if(MsgIndex == 1)
		Flash_ptr = (char *) START0;              // Initialize Flash pointer
	else if(MsgIndex == 2)
		Flash_ptr = (char *) START1;              // Initialize Flash pointer
	else if (MsgIndex == 3)
		Flash_ptr = (char *) START2;              // Initialize Flash pointer
	if (MsgIndex == 5)
		Flash_ptr = (char *) STARTTMP;              // Initialize Flash pointer


	//need to erase 64 flash segments to cover the 1024 bytes (erases 16 bytes at a time)
	char i;
	for (i=0;i<64;i++)
	{
		*Flash_ptr = 0;
		Flash_ptr+= 16;
	}


	FCTL1 = FWKEY;                            // Clear ERASE bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

void ReadCapTouch()
{
	readingCapTouch = 1;
	DisableLEDs();
	_delay_cycles(500);
	position = TI_CAPT_Slider(&slider);
	EnableLEDs();
	readingCapTouch = 0;
}

/******************************************************************************/
// Timer0_A0 Interrupt Service Routine: Disables the timer and exits LPM3
/******************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  TA0CTL &= ~(MC_1);
  TA0CCTL0 &= ~(CCIE);
  __bic_SR_register_on_exit(LPM3_bits+GIE);
}


// Timer1_A0 ISR - In charge of POV, and light sensor timing functions
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR_HOOK(void)
{
	if(mode == MODE_CAP_DEMO)
		__no_operation();
	 if(mode == MODE_POV)
		 if(NextPOV())
			 LPM0_EXIT;
		 //readSensor();

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

