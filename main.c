#include <msp430g2553.h>

#ifndef TIMER0_A1_VECTOR
#define TIMER0_A1_VECTOR    TIMERA1_VECTOR
#define TIMER0_A0_VECTOR    TIMERA0_VECTOR
#endif

volatile unsigned int i;
void main(void)
{
	WDTCTL = WDT_ADLY_1000; 		// WDT 1s interval timer
	IE1 |= WDTIE; 					// Enable WDT interrupt
	P1DIR = 0xFF; 					// All P1.x outputs
	P1OUT = 0; 						// All P1.x reset
	P2DIR = 0xFF; 					// All P2.x outputs
	P2OUT = 0; 						// All P2.x reset
									// An immediate Osc Fault will occur next
	IE1 |= OFIE; 					// Enable Osc Fault
	while(1)
	{
		P1OUT ^= 0x01; 					// Toggle P1.0 using exclusive-OR
		_BIS_SR(LPM3_bits + GIE); 		// Enter LPM3 w/interrupt
	}
}
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
	_BIC_SR_IRQ(LPM3_bits); 		// Clear LPM3 bits from 0(SR)
}
#pragma vector=NMI_VECTOR
__interrupt void nmi_ (void)
{
	do
	{
		IFG1 &= ~OFIFG; 				// Clear OSCFault flag
		for (i = 0xFFF; i > 0; i--); 	// Time for flag to set
		P1OUT ^= 0x01; 					// Toggle P1.0 using exclusive-OR
	}
	while (IFG1 & OFIFG); 			// OSCFault flag still set?
	IE1 |= OFIE; 					// Enable Osc Fault
}

