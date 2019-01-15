#include <msp430.h> 
#include "clock/clock.h"

void wdtInit() {
    WDTCTL = WDTPW |
            WDTTMSEL | // INTERVAL TIMER MODE
            WDTCNTCL | // CLEAR COUNTER
            WDTIS_5 | // 2^13 (250ms @ 32768Hz)
            0;

    SFRIFG1 &= ~WDTIFG;
    SFRIE1 |= WDTIE;
}

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P4DIR |= BIT7;

	clockInit();
	wdtInit();
	__enable_interrupt();

	return 0;
}

__attribute__ ((interrupt(WDT_VECTOR)))
void wdt_isr() {
    P4OUT ^= BIT7;
}
