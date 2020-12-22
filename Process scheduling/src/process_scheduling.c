#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
   // Need valid file name
    if (!ready_queue ||!result)
    {
        return false;
    }

    // Process values
    int burst = 0;
    int delay = 0;
    int size = (int) dyn_array_size(ready_queue);
    int wait = 0;

    // Need instance of PCB to track temporary values
    ProcessControlBlock_t block;

    // Step through list to assign values
    int i;
    for (i = 0; i < size; i++)
    {
        // delay is all cumulative burst times
        delay += burst;

        // Compute the burst time of each PCB
        dyn_array_extract_back(ready_queue, (void *) &block);
        burst += block.remaining_burst_time;

        // Wait is the bursts that have happened so far
        wait += burst;

        // Remaining burst time is counted toward to decrementing the current burst time
        while (!block.remaining_burst_time )

            virtual_cpu(&block);
    }
    
    // Compute AWT / ATA / TRT
    // AWT is cumulative bursts (delay)
    result->average_waiting_time = (float) delay / size;
    // ATA is time the process finished (cumulative wait until last burst completed) - its arrival time
    result->average_turnaround_time = (float) wait / size;
    // TRT is the cumulative count of all bursts for the PCBs
    result->total_run_time = (float) burst;

    return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    // Need valid file name
    if (!ready_queue ||!result)
    {
        return false;
    }

    return true;

}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    // See that params are valid
    if(!ready_queue || !result || (int) quantum < 0)
	{
		return false;
	}

    // Process values
    // size is size of dyn_array, quantum_burst is ongoing quantum, wait is turn around time
	int size = (int) dyn_array_size(ready_queue);
	int quantum_burst = 0;
    int wait = 0;

    // PCB to run in process and utility PCB to load values
	ProcessControlBlock_t pcb, util;

	if (size <= 0)
		{
			result->average_turnaround_time = 0;
    		result->average_waiting_time = 0;
    		result->total_run_time = 0;
			return true;
		}

   while (dyn_array_empty(ready_queue) == false)
   {
        dyn_array_extract_back(ready_queue,(void*)&pcb);

        // Run the quantum bursts on the process
		for(int i = 0; i < (int) quantum; i++)
		{
			if (pcb.remaining_burst_time == 0)
				{
					break;
				}
			virtual_cpu(&pcb);
			quantum_burst++;	
		}
	
        // If the PCB still has some bursts remaining it goes to back of queue to run again
		if(pcb.remaining_burst_time != 0)
			{
			
				dyn_array_push_front(ready_queue, (void*)&pcb);
			}

        // If the PCB has no bursts remaining it's done, save its burst to compute averages
		else
			{
				wait += quantum_burst;	
			}

        // Util stores the values of the current pcb
		pcb = util;
   }

    // Compute AWT / ATA / TRT
    // AWT is cumulative bursts (wait - quantum_burst)
	result->average_waiting_time = (float)(wait - quantum_burst)/size;
    // ATA is the time the process finished (cumulative wait until last burst completed - its arrival time)
	result->average_turnaround_time = (float) wait / size;
    // TRT is the cumulative count of all bursts for the PCBs
	result->total_run_time = (float) quantum_burst;

	 return true;
}

dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    // Need valid file name
    if (!input_file || input_file == '\0')
    {
        return NULL;
    }

    // Open the file to read in binary
    FILE *fstream = fopen(input_file, "rb");

    if (fstream == NULL)
    {
        return NULL;
    }

    // Get the first number from the file, n will be the # of PCBs to make
    uint32_t n;
    if ( (fread(&n, sizeof(int), 1, fstream)) != 1)
    {
        return NULL;
    }

    // Create n PCBs and fill their burst time, priority, and arrival time
	// They will go into the dyn_array: array
    dyn_array_t *array;
    array = dyn_array_create(n, sizeof(ProcessControlBlock_t), NULL);

    for (uint32_t i = 0; i < n; i++)
    {
        // Create the PCB
        ProcessControlBlock_t *pcb = malloc(sizeof(ProcessControlBlock_t));
        if (pcb == NULL)
        {
            free(pcb);
            return NULL;
        }
    
        // Collect values from the binary file

        // 1) Burst
        if ( (fread(&(pcb->remaining_burst_time), sizeof(int), 1, fstream)) != 1)
        {
            return NULL;
        }

        // 2) Priority
        if ( (fread(&(pcb->priority), sizeof(int), 1, fstream)) != 1)
        {
            return NULL;
        }

        // 3) Arrival
        if ( (fread(&(pcb->arrival), sizeof(int), 1, fstream)) != 1)
        {
            return NULL;
        }

        pcb->started = false;

        // Push the new pcb to the back of the array
        dyn_array_push_back(array, pcb);
    }

    return array;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    // See that params are valid
    if (!ready_queue || !result)
    {
        return false;
    }

    // Process values 
    int size = (int) dyn_array_size(ready_queue);
    int burst = 0;
    int wait = 0;
    int turn = 0; 
    int total_time = 0;

    // Utility place holder blocks
    ProcessControlBlock_t pcb, util;

    // PCB count should be valid integer
    if (size <= 0)
    {
        result->average_turnaround_time = 0;
        result->average_turnaround_time = 0;
        result->total_run_time = 0;
        return true;
    }
    // Step through PCB array to calculate its process stats
    else
    {
        int i;
        for (i = 0; i < size; i++)
        {
            // Calc wait
            wait += burst;

            // Iterate over the queue to extract the values
            dyn_array_extract_back(ready_queue, (void *) &pcb);

            burst = pcb.remaining_burst_time;
            total_time += burst;
            turn += (wait + pcb.remaining_burst_time);
        }

        // As long as it hasn't burst quantum times, accumulate the wait
        if (i != 0)
        {
            wait += wait;
        }

        // Expend the burst value
        while(pcb.remaining_burst_time)
        {
            virtual_cpu(&pcb);
        }

        pcb = util;
    }

    // Compute AWT / ATA / TRT
    // AWT is cumulative bursts (wait - quantum_burst)
    result->average_waiting_time = (float) wait / size;
    // ATA is the time the process finished (cumulative wait until last burst completed - its arrival time)
    result->average_turnaround_time = (float) turn / size;
    // TRT is the cumulative count of all bursts for the PCBs
    result->total_run_time = (float) total_time;

    return true;
}
