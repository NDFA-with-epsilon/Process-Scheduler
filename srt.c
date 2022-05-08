/* Shortest remaining time next */
#include <stdio.h>
#include <stdlib.h>
#include "sched.h"

void CheckProcessStatus(process** ptr)
{
    for(int i = 0; i < 4; i++)
    {
        printf("\nProcess %d:\tArrival Time: %d \tRemaining time: %d\tFinished: %s\n", i+1, ((*ptr)+i)->t_arrival, ((*ptr)+i)->t_remaining, ((*ptr)+i)->finished == 0 ? "false" : "true");
        printf("\n");
    } 
}

int calculate_total_time(process** ptr, int buf_len)
{
    int sum = 0;
    for(int i = 0; i < buf_len; i++)
    {
        sum = sum + ((*ptr)+i)->t_burst;
    }
    return sum;
}

proc_node_i* CreateInternalListNode(process* proc)
{
    proc_node_i* tmp = malloc(sizeof(proc_node_i));
    tmp->proc_queues_ptr = proc;
    tmp->next_ptr = NULL;

    return tmp;
}

// Deletes the front node of the list
void DeleteNode(proc_node_i** proc)
{   
    proc_node_i* tmp = (*proc)->next_ptr;
    (*proc)->proc_queues_ptr->finished = 1;
    free(*proc);
    (*proc) = tmp;
}

void SortList(proc_node_i** list_ptr)
{
    // just interchange process* pointers between nodes & not nodes themselves  
    //sort in ascending order of remaining time
    proc_node_i *tmp1, *tmp2;
    process* tmp;
    for(tmp1 = *list_ptr; tmp1 != NULL; tmp1 = tmp1->next_ptr)
    {
        for(tmp2 = tmp1->next_ptr; tmp2 != NULL; tmp2 = tmp2->next_ptr)
        {
            if(tmp1->proc_queues_ptr->t_remaining > tmp2->proc_queues_ptr->t_remaining)
            {
                tmp = tmp1->proc_queues_ptr;
                tmp1->proc_queues_ptr = tmp2->proc_queues_ptr;
                tmp2->proc_queues_ptr = tmp;
            }
        }
    }
}

void UpdateStatus(proc_node_i** front_ptr)
{
    (*front_ptr)->proc_queues_ptr->t_remaining -= 1;
    printf("\nProcess %d executing..\n", (*front_ptr)->proc_queues_ptr->PID);

    if((*front_ptr)->proc_queues_ptr->t_remaining == 0)
    {
        DeleteNode(front_ptr);
    }
}

int lookup_at_table(int t, int* A_T, int len)
{
    for(int i = 0; i < len; i++)
    {
        if(t == A_T[i])
        {
            return i+1;
        }
    }

    return 0;
}
void displayqueue(proc_node_i** ptr)
{
    proc_node_i* tmp = *ptr;
    printf("\nProcess IDs:\n");
    while(tmp != NULL)
    {
        printf("%d\t", tmp->proc_queues_ptr->PID);
        tmp = tmp->next_ptr;
    }
}

void scheduler(process** proc_queue, int* A_T, int table_len, int at_total)
{
    printf("\nAT[table_len-1] times is %d\n", A_T[table_len-1]);
    int node_count = 0;

    proc_node_i* front_ptr;
    front_ptr = CreateInternalListNode(*proc_queue);
    node_count += 1;

    for(int t = 0; t <= at_total; t++)
    {
        if(t <= A_T[table_len-1])
        {
            int index = lookup_at_table(t, A_T, table_len);

            if(index != 0)
            {
                printf("\nNew process arrived!!\n");
                proc_node_i* tmp = CreateInternalListNode( ((*proc_queue)+index) );
                node_count += 1;

                if( tmp->proc_queues_ptr->t_burst != front_ptr->proc_queues_ptr->t_remaining )
                {
                    //Add the created node at front of the list
                    tmp->next_ptr = front_ptr;
                    front_ptr = tmp;
                    
                    if(front_ptr->next_ptr != NULL)
                    {
                        SortList(&front_ptr);
                        printf("\nProcess queue updated.\n");
                        
                    }
                }

                displayqueue(&front_ptr);                
                UpdateStatus(&front_ptr);
                continue;
            }

            printf("\nNo new process found\n");
            displayqueue(&front_ptr);
            UpdateStatus(&front_ptr);
            continue;
        }

        printf("\nTime is beyond max AT, no new processes now\n");
        displayqueue(&front_ptr);
        UpdateStatus(&front_ptr);
        if(front_ptr == NULL)
        {
            break;
        }
    }
}


int main(void)
{
    process* proc_queue = malloc(4 * sizeof(process));

    for(int i = 0; i < 4; i++)
    {
        (&proc_queue[i])->t_arrival = i;

        printf("\nEnter burst time:\n");
        scanf("%d",&((&proc_queue[i])->t_burst));

        (&proc_queue[i])->t_remaining = (&proc_queue[i])->t_burst;
        (&proc_queue[i])->PID = i+1;
        (&proc_queue[i])->finished = false;

        printf("\nProcess %d queued\n", i+1);
    }
    CheckProcessStatus(&proc_queue);

    int A_T[3]; //Arrival-time table
    for(int i = 0; i < 3; i++)
    {
        A_T[i] = (&proc_queue[i+1])->t_arrival;
    }
    
    int a_t_total = calculate_total_time(&proc_queue, 4);
    printf("\nTotal running time is:\t%d\n", a_t_total);
    scheduler(&proc_queue, A_T, 3, a_t_total);
    CheckProcessStatus(&proc_queue);

}       