#include <msp430.h> 
#include "main.h"
/*
 * main.c
 */

int main(void) {
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

    P1DIR |= BIT6;
	P1REN |= BIT0|BIT3;
	P1OUT &= ~(BIT0|BIT3);
	P2DIR |= BIT7|BIT6;
	P2OUT |= BIT7;  //turn 2.7 on - it powers the light sensors
    P2SEL2 &= ~(BIT6|BIT7);
	P2SEL &= ~(BIT6|BIT7);

	ConfigureSPI();

	write(0x7);
	_delay_cycles(1000000);
	write(BIT2);
	_delay_cycles(1000000);

	StartLightSensor();

	while(1)
	{
		_BIS_SR(LPM0_bits + GIE);
	}


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
	static int readsSinceChange=0;



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
				WriteFlash(5, sensorDataIndex+1, currentVal); //write to temp location
			}
			if(sensorDataIndex == 3 && msgLen == 0)  //first time through, we have the full initialization sequence
			{
				if(sensorData[0] == 0xAA && sensorData[1] == 0xFF)
				{
					//which message we're storing
					msgIndex = sensorData[2];
					//length of message being transmitted
					//TODO: need a temp storage place in flash for incomplete messages - otherwise we'd be using 1/2 our ram for temporary message storage
					msgLen = sensorData[3];
					sensorDataIndex = 0;
					currentVal = 0;
					WriteFlash(5, 0, msgLen);
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
					StartPOV();
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
	write((~dataLow&BIT0) << 1|(~clockLow&BIT0)); //output to shift register for visual indicator


	//not enabled yet - the intention of this block is to indicate that we're done reading
//	if(readsSinceChange == 0xFF)
//	{
//		readSCOverflow++;
//		if(readSCOverflow > 10)
//		{
//			write(0x00);
//			_delay_cycles(1000000);
//			write(0xFF);
//			_delay_cycles(1000000);
//			write(0x00);
//			_delay_cycles(1000000);
//			write(0xFF);
//			_delay_cycles(1000000);
//			write(0x00);
//			_delay_cycles(1000000);
//			write(0xFF);
//			_delay_cycles(1000000);
//		}
//	}
}

void StartLightSensor()
{
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 2000;
	TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode

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

	mode = 0;
}

void DisableLightSensor()
{
    /* POWER: Turn ADC and reference voltage off to conserve power */
    ADC10CTL0 &= ~(ADC10ON | REFON);
}

void ConfigureSPI()
{
	//configure pins for SPI
	P1SEL2 = BIT5 | BIT7;
	P1SEL = BIT5 | BIT7;

	//Initialize USCI B for SPI
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 =  UCMST | UCMODE_0 | UCSYNC |UCCKPH|UCCKPL;
	UCB0CTL1 = UCSSEL_2 | UCSWRST;
	UCB0BR0 = 1;
	UCB0CTL1 &= ~UCSWRST;
}
void StartPOV()
{
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
	else if(SelectedPOVMessage == 3)
	{
		POV_Flash_ptr = (char *) (START3);
		POVStart = START3; //store start position
	}

	POVLen = *POV_Flash_ptr;

	*POV_Flash_ptr++; //move to the first item in the list
	POVCurIndex = 0;
	CCR0 = 2000;
	mode = 1;
}

void NextPOV()
{
	P2OUT &= ~BIT6;
	//_delay_cycles(10);
	write(*POV_Flash_ptr);

	//_delay_cycles(10);

	//_delay_cycles(1);
	P2OUT |= BIT6;

	POVCurIndex++;
	POV_Flash_ptr++;

	if(POVCurIndex >= POVLen)
	{
		POV_Flash_ptr = (char *)(POVStart+1);
		POVCurIndex = 0;
	}
}

void write(char data) {
	P2OUT &= ~BIT6;
	while (!(IFG2 & UCB0TXIFG))
		;
	UCB0TXBUF = data;
	while (!(IFG2 & UCB0TXIFG))
	          ;
	P2OUT |= BIT6;
}

void WriteFlash(char MsgIndex, char ValIndex, char Value)
{
	char *Flash_ptr;                          // Flash pointer
	if(MsgIndex == 1)
		Flash_ptr = (char *) (START0+ValIndex);              // Initialize Flash pointer
	else if(MsgIndex == 2)
		Flash_ptr = (char *) (START1+ValIndex);              // Initialize Flash pointer
	else if (MsgIndex == 3)
		Flash_ptr = (char *) (START2+ValIndex);              // Initialize Flash pointer
	else if (MsgIndex == 4)
		Flash_ptr = (char *) (START3+ValIndex);              // Initialize Flash pointer
	else if (MsgIndex == 5)
		Flash_ptr = (char *) (STARTTMP+ValIndex);              // Initialize Flash pointer

	FCTL1 = FWKEY + ERASE;                      // Set WRT bit for write operation
	FCTL3 = FWKEY;                            // Clear Lock bit
	//*Flash_ptr = 0;                           // Dummy write to erase Flash segment
	FCTL1 = FWKEY + WRT;

	*Flash_ptr = Value;

	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

void CopyFlashFromTmp(char MsgIndex, char Len)
{
	char *Flash_ptr;                          // Flash pointer
	char *Flash_ptr_tmp = (char *) STARTTMP;

	if(MsgIndex == 1)
		Flash_ptr = (char *) START0;              // Initialize Flash pointer
	else if(MsgIndex == 2)
		Flash_ptr = (char *) START1;              // Initialize Flash pointer
	else if (MsgIndex == 3)
		Flash_ptr = (char *) START2;              // Initialize Flash pointer
	else if (MsgIndex == 4)
		Flash_ptr = (char *) START3;              // Initialize Flash pointer

	FCTL1 = FWKEY + ERASE;                      // Set WRT bit for write operation
	FCTL3 = FWKEY;                            // Clear Lock bit
	//*Flash_ptr = 0;                           // Dummy write to erase Flash segment
	FCTL1 = FWKEY + WRT;
	char i;

	for(i = 0;i<Len;i++)
	{
		*Flash_ptr++ = (char)(*Flash_ptr_tmp++);//copy from temporary flash memory
	}



	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR_HOOK(void)
{
	 if(mode == 0)
		 readSensor();
	 else
		 NextPOV();
}
