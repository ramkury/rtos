#include "os.h"

fifo fifos[PRIORITIES];

task * running_task;

task tasks[MAX_TASKS * PRIORITIES] = { 0 };

uint16_t *next_sp = (uint16_t*)(0x2800 + 0x80);

task* fifoGet(fifo *f){

    task* t = f->tasks[f->head];
    f->head = (f->head+1)%MAX_TASKS;
    return t;
}

task* addTask(fifo* f) {
    task* t = f->tasks[f->tail];
    f->tail = (f->tail + 1)%MAX_TASKS;
    f->count++;
    return t;
}

void register_task(process entry_point, fifo *f) {

    task* current = findAvailableTask();
    current->entry_point = entry_point;
    uint32_t pc = (uint32_t)entry_point;
    *(--current->stack_ptr) = (uint16_t)(pc & 0xffff);
    *(--current->stack_ptr) = (uint16_t)(((pc & 0xf0000) >> 4) | GIE);
    current->stack_ptr -= 24;
    fifoPut(f, current);
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

void initTasks() {
    uint16_t * stack_ptr = (uint16_t*)0x2880;
    uint8_t i;
    for(i = 0; i < MAX_TASKS * PRIORITIES; ++i) {
        tasks[i].pid = 0;
        tasks[i].stack_ptr = stack_ptr;
        stack_ptr += (0x80 / sizeof(uint16_t));
    }
}

void fifoPut(fifo *f, task *t){

    f->tasks[f->tail] = t;
    f->tail = (f->tail + 1)%MAX_TASKS;
    f->count++;
}

void scheduler() {
    uint8_t i;
    for(i = 2; i >= 0; --i) {
        if(fifos[i].count != 0) {
            break;
        }
    }

    running_task = fifoGet(&fifos[i]);
}
