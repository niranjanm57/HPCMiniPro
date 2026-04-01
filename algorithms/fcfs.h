#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "../scheduler.h"
#include <algorithm>
#include <omp.h>

/**
 * @class FCFSScheduler
 * @brief First Come First Serve (FCFS) Scheduling
 * 
 * Jobs are executed in the order of their arrival.
 * Non-preemptive, simple scheduling algorithm.
 */
class FCFSScheduler : public Scheduler {
private:
    /**
     * Sort jobs by arrival time for FCFS
     */
    void sort_by_arrival_time() {
        std::sort(jobs.begin(), jobs.end(), 
                 [](const Job& a, const Job& b) {
                     return a.arrival_time < b.arrival_time;
                 });
    }
    
public:
    const char* get_algorithm_name() const override {
        return "First Come First Serve (FCFS)";
    }
    
    /**
     * Sequential FCFS scheduling
     * Time Complexity: O(n)
     */
    void schedule_sequential() override {
        reset_jobs();
        sort_by_arrival_time();
        scheduled_jobs = jobs;
        
        int current_time = 0;
        
        for (auto& job : scheduled_jobs) {
            // Job starts when it arrives or when CPU is free
            job.start_time = std::max(current_time, job.arrival_time);
            job.completion_time = job.start_time + job.burst_time;
            job.calculate_waiting_time();
            job.calculate_turnaround_time();
            
            // Update current time for next job
            current_time = job.completion_time;
        }
    }
    
    /**
     * Parallel FCFS scheduling using OpenMP
     * 
     * Strategy: 
     * - Sort jobs by arrival time (sequential)
     * - Calculate start/completion times sequentially (dependency in timeline)
     * - Calculate metrics in parallel (no dependencies)
     */
    void schedule_parallel() override {
        reset_jobs();
        sort_by_arrival_time();
        scheduled_jobs = jobs;
        
        // Calculate timeline sequentially (data dependency)
        int current_time = 0;
        for (auto& job : scheduled_jobs) {
            job.start_time = std::max(current_time, job.arrival_time);
            job.completion_time = job.start_time + job.burst_time;
            current_time = job.completion_time;
        }
        
        // Calculate metrics in parallel (no dependencies between jobs)
        #pragma omp parallel for num_threads(num_threads) schedule(static)
        for (size_t i = 0; i < scheduled_jobs.size(); ++i) {
            scheduled_jobs[i].calculate_waiting_time();
            scheduled_jobs[i].calculate_turnaround_time();
        }
    }
};

#endif // FCFS_SCHEDULER_H
