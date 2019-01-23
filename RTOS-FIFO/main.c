#include <msp430.h> 
#include "clock/clock.h"
#include "os.h"

void idle() {
    //TODO: sleeping beauty
    while(1);
}

void task1() {
    P1DIR |= BIT0;
    volatile uint16_t delay;

    uint16_t coisa = 100;
    while(coisa--) {
        delay = 10000;
        while(delay--);
        P1OUT ^= BIT0;
    }

    exitOS();
}

void task2() {
    P4DIR |= BIT7;
    volatile uint16_t delay;

    uint8_t coisa = 255;
    while(coisa--) {
        delay = 10000;
        while(delay--);
        P4OUT ^= BIT7;
    }

    exitOS();
}

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	startERTOS();
	register_task(task1, 2);
	register_task(task2, 1);
	register_task(idle,  0);
	while(1);
	return 0;
}
