#include "os.h"
#include "clock/clock.h"

fifo fifos[PRIORITIES] = {0};

task * running_task;

task tasks[MAX_TASKS * PRIORITIES] = { 0 };

uint16_t *next_sp = (uint16_t*)(0x3200 + 0x80);

uint16_t *scheduler_sp = (uint16_t*)0x3200;

task* fifoGet(fifo *f){

    task* t = f->tasks[f->head];
    f->head = (f->head+1)%MAX_TASKS;
    f->count--;
    return t;
}

task* addTask(fifo* f) {
    task* t = f->tasks[f->tail];
    f->tail = (f->tail + 1)%MAX_TASKS;
    f->count++;
    return t;
}

void register_task(process entry_point, uint8_t priority) {
    static uint8_t pid = 1;
    task* current = findAvailableTask();
    current->pid = pid++;
    current->status = priority;
    current->quantum = (priority << 1) + 1;
    current->entry_point = entry_point;
    uint32_t pc = (uint32_t)entry_point;
    *(--current->stack_ptr) = (uint16_t)(pc & 0xffff);
    *(--current->stack_ptr) = (uint16_t)(((pc & 0xf0000) >> 4) | GIE);
    current->stack_ptr -= 24;
    fifoPut(&fifos[priority], current);
}

task* findAvailableTask() {
    task* t;
    const task* last = tasks + MAX_TASKS * PRIORITIES;
    for(t = tasks; t < last; t++) {
        if(t->pid == 0) {
            return t;
        }
    }
    return 0;
}

void initTasks() { // prepare task vector - allocate stacks
    uint16_t * stack_ptr = (uint16_t*)0x2880;
    uint8_t i;
    for(i = 0; i < MAX_TASKS * PRIORITIES; ++i) {
        tasks[i].pid = 0;
        tasks[i].stack_ptr = stack_ptr;
        tasks[i].stack_init = stack_ptr;
        stack_ptr += (0x80 / sizeof(uint16_t));
    }
}

void fifoPut(fifo *f, task *t){

    f->tasks[f->tail] = t;
    f->tail = (f->tail + 1)%MAX_TASKS;
    f->count++;
}

void scheduler() {
    static uint8_t quantum;
    if (running_task->status & (1 << 7)) { // Task is finished
        // Clear task
        running_task->pid = 0;
        running_task->status = 0;
        running_task->stack_ptr = running_task->stack_init;
    }
    else if (quantum--) {
        return;
    }
    else {
        fifoPut(&fifos[running_task->status], running_task);
    }

    // get new task
    uint8_t i;
    for(i = 2; i >= 0; --i) {
        if(fifos[i].count != 0) {
            break;
        }
    }

    running_task = fifoGet(&fifos[i]);
    quantum = running_task->quantum;
}

__attribute__((naked))
__attribute__((interrupt(WDT_VECTOR)))
void dispatcher() {
    asm("pushm.a #12, R15");    // Saving current context to stack
    asm("movx.a  SP, %0" : "=m" (running_task->stack_ptr)); // Saves current stack pointer to task struct
    asm("movx.a  %0, SP" :: "m" (scheduler_sp)); // Restores scheduler stack pointer
    scheduler(); // Running scheduler
    asm("movx.a  SP, %0" : "=m" (scheduler_sp)); // Stores scheduler stack pointer
    asm("movx.a  %0, SP" :: "m" (running_task->stack_ptr)); // Restores next task's stack pointer
    asm("popm.a  #12, R15"); // Restores next task's context
    asm("reti"); // Ends dispatcher
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

void startERTOS() {
    clockInit();
    initTasks();

    running_task = findAvailableTask();
    running_task->pid = -1;
    running_task->status = (1 << 7) | 2;

    wdtInit();
    __enable_interrupt();
}

void exitOS() {
    running_task->status |= (1 << 7); // set the finished bit
    while(1);
}
