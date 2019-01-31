/*
 *  ======== empty.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Board Header file */
#include "Board.h"

/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */
Void blink_red()
{
    uint16_t seconds = 6;
    while (seconds--) {
        Task_sleep((unsigned int)1000);
        GPIO_toggle(Board_LED0);
    }
}

Void blink_green()
{
    uint16_t seconds = 12;
    while (seconds--) {
        Task_sleep((unsigned int)100);
        GPIO_toggle(Board_LED1);
    }
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
