/*
 *  ======== empty_min.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <msp430.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Log.h>

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

int16_t globalVar = 20;

void readerFunc(UArg arg0){
    int16_t val = globalVar;
    Log_info2("Reader %d - %d", arg0, val);
}

void writerFunc(UArg arg0){
    Semaphore_pend(semaphore0, BIOS_WAIT_FOREVER);
    globalVar = arg0 ? globalVar + 1 : globalVar - 1;
    Log_info2("Writer %d - %d", arg0,globalVar);
    Semaphore_post(semaphore0);
}

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
