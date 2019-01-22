#include <msp430.h>
#include <stdint.h>

#define MAX_TASKS 8
#define PRIORITIES 3

typedef void (*process)();

typedef struct {
    uint16_t pid;
    process entry_point;
    uint16_t quantum;
    uint16_t *stack_ptr;
} task;

typedef struct {
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    task *tasks[MAX_TASKS];
}fifo;

extern fifo fifos[PRIORITIES];

extern task * running_task;

extern task tasks[MAX_TASKS * PRIORITIES];

task* fifoGet(fifo *f);

task* addTask(fifo* f);

void register_task(process entry_point, fifo *f);

void fifoPut(fifo *f, task *t);

void scheduler();

task* findAvailableTask();

void initTasks();

