#include <msp430.h> 

/*
 * main.c
 */
char msgLen = 16;
char message[20] = {0x00,0x00,0x00,0x80,0x80,0xF8,0x80,0x80,0x00,0x88,0x88,0xF8,0x88,0x88,0x00,0x00};
//char message[20] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x00};

void write(char data) {
	while (!(IFG2 & UCB0TXIFG))
		;
	UCB0TXBUF = data;
	while (!(IFG2 & UCB0TXIFG))
	          ;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    //clock initialization
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

	//Configure pins for Launchpad - NOT FOR FINAL RELEASE
	P1DIR |= BIT0|BIT6;
	P1OUT &= ~(BIT0|BIT6);

	//configure pins for SPI
	P1SEL2 = BIT5 | BIT7;
	P1SEL = BIT5 | BIT7;

    //not using 2.6 for external crystal
    P2SEL2 &= ~(BIT6|BIT7);
	P2SEL &= ~(BIT6|BIT7);
	P2DIR |= BIT6;

	//Initialize USCI B for SPI
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 =  UCMST | UCMODE_0 | UCSYNC |UCCKPH|UCCKPL;
	UCB0CTL1 = UCSSEL_2 | UCSWRST;
	UCB0BR0 = 1;
	UCB0CTL1 &= ~UCSWRST;




	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 2000;
	TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode


	while(1)
	{
		_BIS_SR(LPM0_bits + GIE);
	}

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR_HOOK(void)
{
	P1OUT ^= BIT0;
	static char msgIndex = 0;
	P2OUT &= ~BIT6;
	//_delay_cycles(10);
	write(message[msgIndex]);
	//_delay_cycles(10);

	//_delay_cycles(1);
	P2OUT |= BIT6;

	msgIndex++;
	if (msgIndex >= msgLen)
		msgIndex = 0;

}

//// ======== Timer0_A3 Interrupt Service Routine ========
//
//#pragma vector=TIMER0_A1_VECTOR
//__interrupt void TIMER0_A1_ISR_HOOK(void)
//{
//	P1OUT ^= BIT0;
//	static char msgIndex = 0;
//	P2OUT &= ~BIT6;
//	//_delay_cycles(10);
//	write(message[msgIndex]);
//	//_delay_cycles(10);
//
//	//_delay_cycles(1);
//	P2OUT |= BIT6;
//
//	msgIndex++;
//	if (msgIndex >= msgLen)
//		msgIndex = 0;
//
//}
