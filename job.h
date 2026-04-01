#ifndef JOB_H
#define JOB_H

#include <iostream>

/**
 * @struct Job
 * @brief Represents a job in the HPC scheduler system
 * 
 * Each job has:
 * - ID: Unique identifier
 * - Arrival Time: When the job arrives at the queue
 * - Burst Time: CPU time required to complete the job
 * - Priority: Priority level (lower value = higher priority)
 */
struct Job {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int start_time = -1;      // When execution starts
    int completion_time = -1; // When execution completes
    
    // Computed metrics
    int waiting_time = 0;
    int turnaround_time = 0;
    
    Job() : id(0), arrival_time(0), burst_time(0), priority(0) {}
    
    Job(int id, int at, int bt, int p) 
        : id(id), arrival_time(at), burst_time(bt), priority(p) {}
    
    /**
     * Calculate waiting time = start_time - arrival_time
     */
    void calculate_waiting_time() {
        if (start_time != -1) {
            waiting_time = start_time - arrival_time;
            if (waiting_time < 0) waiting_time = 0;
        }
    }
    
    /**
     * Calculate turnaround time = completion_time - arrival_time
     */
    void calculate_turnaround_time() {
        if (completion_time != -1) {
            turnaround_time = completion_time - arrival_time;
            if (turnaround_time < 0) turnaround_time = 0;
        }
    }
    
    /**
     * Reset job execution metrics for reuse
     */
    void reset() {
        start_time = -1;
        completion_time = -1;
        waiting_time = 0;
        turnaround_time = 0;
    }
};

#endif // JOB_H
