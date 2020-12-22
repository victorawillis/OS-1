#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"
#define SRTF "SRTF"

// Neat displayer function
void print_process_stats (char* process_type, ScheduleResult_t* result, FILE* fstream)
{
    // Print results
    printf("%s%s", "\n* * * Process Stats * * *\n", process_type);
    printf("%s %.2f", "\nAWT", result->average_waiting_time);
    printf("%s %.2f", "\nATT", result->average_turnaround_time);
    printf("%s %lu\n", "\nTRT", result->total_run_time);
    // Write results to readme.md
    fprintf(fstream, "%s%s", "\n* * * Process Stats * * *\n", process_type);
    fprintf(fstream, "%s %.2f", "\n\nAWT", result->average_waiting_time);
    fprintf(fstream, "%s %.2f", "\n\nATT", result->average_turnaround_time);
    fprintf(fstream, "%s %lu", "\n\nTRT", result->total_run_time);
}

int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%p <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Collect PCB file, process type from CL
    char *filename = argv[1];
    char *process_type = argv[2];
    
    // In case of getting quantum from CL
    char *q;
    size_t quantum;
    if (argc == 4) 
    {
        q = argv[3];
        quantum = (size_t) atoi(q);
    }

    int res = strcmp(process_type, RR);
    
    // Quantum pertains to RR
    if (res != 0 && argc == 4)
    {
        printf("\nArgument Quantum pertains to RR as the Process Type\n");
        return EXIT_FAILURE;
    }

    if (argc < 4 && strcmp(process_type, RR) == 0)
    {
        printf("\nArgument Quantum necessary to run RR\n");
        return EXIT_FAILURE;
    }

    // Setup readme.md
    FILE *fstream;
    if (!(fstream = fopen("../readme.md", "a")))
    {
        printf("\nreadme.md failed to open\n");
        return EXIT_FAILURE;
    }

    // Create dyn_array from file
    dyn_array_t *dyn_array = load_process_control_blocks(filename);

    if (!dyn_array)
    {
        printf("\nFile is invalid\n");
        fclose(fstream);
        return EXIT_FAILURE;
    }
    
    // Create instance of result ready to record process stats
    ScheduleResult_t *result = malloc(sizeof(ScheduleResult_t));
    result->average_waiting_time = 0;
    result->average_turnaround_time = 0;
    result->total_run_time = 0;

    // Serve the process stats
    if (strcmp(process_type, FCFS) == 0)
    {
        first_come_first_serve(dyn_array, result);
        print_process_stats(FCFS, result, fstream);
    }
    if (strcmp(process_type, SRTF) == 0)
    {
        priority(dyn_array, result);
        print_process_stats(SRTF, result, fstream);
    }
    if (strcmp(process_type, RR) == 0)
    {
        round_robin(dyn_array, result, quantum);
        print_process_stats(RR, result, fstream);
    }
    if (strcmp(process_type, SJF) == 0)
    {
        shortest_job_first(dyn_array, result);
        print_process_stats(SJF, result, fstream);
    }
    // Invalid input
    else
    {
        //printf("\nProcess type entered is invalid. Choose from FCFS, SRTF, RR, or SJF\n");
        free(result);
        dyn_array_destroy(dyn_array);
        fclose(fstream);
        return EXIT_FAILURE;
    }

    // Pack up and go home
    fclose(fstream);
    free(result);
    dyn_array_destroy(dyn_array);
    
    return EXIT_SUCCESS;
}
