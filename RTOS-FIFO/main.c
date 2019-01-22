#include <msp430.h> 
#include "clock/clock.h"
#include "os.h"

void idle() {
    //TODO: sleeping beauty
    while(1);
}

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
	
	return 0;
}
