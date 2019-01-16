#include <msp430.h> 
#include "clock/clock.h"

typedef void (*process)();

typedef struct {
    process entry_point;
    uint8_t *stack_ptr;
} task;

task tasks[10] = { 0 };
uint8_t task_count = 0;
uint16_t *next_sp = (uint16_t*)(0x2800 + 0x80);

void register_task(process entry_point);

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	clockInit();
	return 0;
}

void register_task(process entry_point) {
    task* current = &tasks[task_count++];
    current->entry_point = entry_point;
    current->stack_ptr = next_sp;
    next_sp += (0x80 / sizeof(uint16_t));
    //TODO initialize stack
}
