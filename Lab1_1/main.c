#include <msp430.h>
#include <stdint.h>
#include "clock/clock.h"

/**
 * main.c
 */
int main(void)
{
    volatile uint32_t i;

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P4DIR |= BIT7;              // Configura o LED verde como sa�da

	clockInit();

	while(1) {                  // Entra em loop infinito
	    asm("mov.w #25000, %0" : "=m" (i));
	    asm("mov.w #0, 2(%0)": "=m" (i));
	    while(--i);             // em intervalos peri�dicos
	    P4OUT ^= BIT7;
	}
	
	return 0;
}
