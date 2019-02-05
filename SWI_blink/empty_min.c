/*
 *  ======== empty_min.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <msp430.h>
#include <xdc/cfg/global.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header file */
#include "Board.h"

Void isr_blink_red(UArg arg0, UArg arg1){
    GPIO_toggle(Board_LED0);
}

void isr_hwi1(UArg arg0){
    GPIO_toggle(Board_LED1);
    Swi_post(blink_red);
}

/*
 *  ======== main ========
 */
int main(void)
{

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    // Board_initWatchdog();

    TA1CCR0 = 32768;
    TA1CTL = TASSEL__SMCLK |
             TACLR  |
             MC__UP;
    TA1CCTL0 = CCIE;

    /* Start BIOS */
    BIOS_start();

    return (0);
}
