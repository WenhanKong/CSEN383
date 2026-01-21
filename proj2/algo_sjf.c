#include "scheduler.h"

void run_SJF(Process *processes, int process_count) {
    int time = 0;
    int completed = 0;

    // Initialize runtime fields
    for (int i = 0; i < process_count; i++) {
        processes[i].remaining_time = processes[i].run_time;
        processes[i].start_time = -1;
        processes[i].finish_time = 0;
        for (int t = 0; t < TOTAL_QUANTA; t++) {
            processes[i].history[t] = false;
        }
    }

    while (time < TOTAL_QUANTA && completed < process_count) {

        // Select shortest job that has arrived and is not finished
        int idx = -1;
        int shortest = 999999;

        for (int i = 0; i < process_count; i++) {
            if (processes[i].remaining_time > 0 &&
                processes[i].arrival_time <= time) {

                if (processes[i].remaining_time < shortest) {
                    shortest = processes[i].remaining_time;
                    idx = i;
                }
            }
        }

        // No job ready → CPU idle for this quantum
        if (idx == -1) {
            time++;
            continue;
        }

        // Start job at quantum boundary
        if (processes[idx].start_time == -1) {
            processes[idx].start_time = time;
        }

        // Run job to completion (non-preemptive)
        while (processes[idx].remaining_time > 0) {

            // Mark execution history only within 100 quanta window
            if (time < TOTAL_QUANTA) {
                processes[idx].history[time] = true;
            }

            processes[idx].remaining_time--;
            time++;

            // Job may finish after 100, but we stop tracking history at 100
        }

        processes[idx].finish_time = time;
        completed++;
    }
}
