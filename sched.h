#ifndef SCHED_SCHED_H
#define SCHED_SCHED_H

#include <stdbool.h>

typedef struct _process
{
    int t_arrival;
    int t_burst; 
    int t_remaining;
    int PID;
    bool finished;
}process;

typedef struct _proc_node_i ///* 'i' stands for 'internal' i.e internal list maintained by the scheduler *///
{
    process* proc_queues_ptr;
    struct _proc_node_i* next_ptr;
}proc_node_i;

int calculate_total_time(process** ptr, int buf_len);
void CheckProcessStatus(process** ptr);

proc_node_i* CreateInternalListNode(process* proc);
void DeleteNode(proc_node_i** proc);
void SortList(proc_node_i** list_ptr);
void UpdateStatus(proc_node_i** front_ptr);
void displayqueue(proc_node_i** ptr);

void scheduler(process** proc_queue, int* A_T, int table_len, int at_total);

#endif