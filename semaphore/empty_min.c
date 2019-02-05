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
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header file */
#include "Board.h"

Void task1(UArg arg0, UArg arg1) {
    while(1){
        Semaphore_pend(semaphore_500, BIOS_WAIT_FOREVER);
        GPIO_toggle(Board_LED0);
        Semaphore_pend(semaphore_1000, BIOS_WAIT_FOREVER);
        GPIO_toggle(Board_LED1);
    }
}

void clock_1sec() {
    Semaphore_post(semaphore_1000);
}

void clock_500msec(){
    Semaphore_post(semaphore_500);

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

    /* Start BIOS */
    BIOS_start();

    return (0);
}
