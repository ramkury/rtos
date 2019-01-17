#include <msp430.h> 
#include "clock/clock.h"

typedef void (*process)();

typedef struct {
    process entry_point;
    uint16_t *stack_ptr;
} task;

task tasks[10] = { 0 };
uint8_t task_count = 0;
uint8_t running_task = 0;

uint16_t *next_sp = (uint16_t*)(0x2800 + 0x80);

uint16_t *scheduler_sp = (uint16_t*)0x2800;

void register_task(process entry_point);

void startERTOS();

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
	startERTOS();
	return 0;
}

void register_task(process entry_point) {
    task* current = &tasks[task_count++];
    current->entry_point = entry_point;
    current->stack_ptr = next_sp;
    next_sp += (0x80 / sizeof(uint16_t));
    uint32_t pc = (uint32_t)entry_point;
    *(--current->stack_ptr) = (uint16_t)((pc & 0xf0000) >> 4 | GIE);
    *(--current->stack_ptr) = (uint16_t)(pc & 0xffff);
    current->stack_ptr -= 12;
}

void scheduler() {
    running_task++;
    if (running_task >= task_count) {
        running_task = 0;
    }
}

__attribute__((naked))
__attribute__((interrupt(WDT_VECTOR)))
void dispatcher() {
    asm("pushm.a #12, R15");    // Saving current context to stack
    asm("movx.a  SP, %0" : "=m" (tasks[running_task].stack_ptr));
    asm("movx.a  %0, SP" :: "m" (scheduler_sp));
    scheduler();
    asm("movx.a  SP, %0" : "=m" (scheduler_sp));
    asm("movx.a  %0, SP" :: "m" (tasks[running_task].stack_ptr));
    asm("popm.a  #12, R15");
}

void startERTOS() {
    clockInit();

    tasks[running_task].stack_ptr += 12;
    //TODO: update PC value

    wdtInit();
    __enable_interrupt();
}
