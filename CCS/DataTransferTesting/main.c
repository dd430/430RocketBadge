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

	//configure pins for SPI
	P1SEL2 = BIT5 | BIT7;
	P1SEL = BIT5 | BIT7;

	//Initialize USCI B for SPI
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 =  UCMST | UCMODE_0 | UCSYNC |UCCKPH|UCCKPL;
	UCB0CTL1 = UCSSEL_2 | UCSWRST;
	UCB0BR0 = 1;
	UCB0CTL1 &= ~UCSWRST;

	write(0x7);
	_delay_cycles(1000000);
	write(BIT2);
	_delay_cycles(1000000);

	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 5000;
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

	while(1)
	{
		_BIS_SR(LPM0_bits + GIE);
	}

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
	static int readsSinceChange=0;
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
			sensorData[sensorDataIndex] = currentVal;
			sensorReads[sensorDataIndex] = readsSinceChange;
			readsSinceChange= 0;
			sensorDataIndex++;
			if(sensorDataIndex > 19)
				sensorDataIndex = 0;
			currentVal = 0;
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

void write(char data) {
	P2OUT &= ~BIT6;
	while (!(IFG2 & UCB0TXIFG))
		;
	UCB0TXBUF = data;
	while (!(IFG2 & UCB0TXIFG))
	          ;
	P2OUT |= BIT6;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR_HOOK(void)
{
	 //ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	readSensor();
}
