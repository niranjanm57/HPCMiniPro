#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "../scheduler.h"
#include <omp.h>
#include <climits>

/**
 * @class PriorityScheduler
 * @brief Priority-based CPU Scheduling
 * 
 * Jobs are scheduled based on their priority level.
 * Lower priority value = higher priority (executed first).
 * Non-preemptive scheduling algorithm.
 */
class PriorityScheduler : public Scheduler {
private:
    /**
     * Sort jobs by priority (and arrival time as tiebreaker)
     */
    void sort_by_priority() {
        std::sort(jobs.begin(), jobs.end(), 
                 [](const Job& a, const Job& b) {
                     // First, consider arrival time
                     if (a.arrival_time != b.arrival_time) {
                         return a.arrival_time < b.arrival_time;
                     }
                     // Then, by priority (lower value = higher priority)
                     return a.priority < b.priority;
                 });
    }
    
    /**
     * Get jobs ready to execute at a given time
     * and select the one with highest priority
     */
    int get_next_job_index(int current_time, const std::vector<bool>& executed) {
        int best_idx = -1;
        int best_priority = INT_MAX;
        
        for (size_t i = 0; i < jobs.size(); ++i) {
            if (!executed[i] && jobs[i].arrival_time <= current_time) {
                if (jobs[i].priority < best_priority) {
                    best_priority = jobs[i].priority;
                    best_idx = i;
                }
            }
        }
        return best_idx;
    }
    
public:
    const char* get_algorithm_name() const override {
        return "Priority Scheduling";
    }
    
    /**
     * Sequential Priority scheduling
     * Time Complexity: O(n^2) in worst case
     */
    void schedule_sequential() override {
        reset_jobs();
        sort_by_priority();
        
        std::vector<bool> executed(jobs.size(), false);
        int current_time = 0;
        
        // Collect all jobs that will be scheduled
        std::vector<Job> temp_jobs = jobs;
        
        for (size_t i = 0; i < temp_jobs.size(); ++i) {
            // Find next job to execute
            int next_idx = -1;
            int best_priority = INT_MAX;
            
            for (size_t j = 0; j < temp_jobs.size(); ++j) {
                if (!executed[j] && temp_jobs[j].arrival_time <= current_time) {
                    if (temp_jobs[j].priority < best_priority) {
                        best_priority = temp_jobs[j].priority;
                        next_idx = j;
                    }
                }
            }
            
            // If no job is ready, advance time to next arrival
            if (next_idx == -1) {
                int min_arrival = INT_MAX;
                for (size_t j = 0; j < temp_jobs.size(); ++j) {
                    if (!executed[j] && temp_jobs[j].arrival_time > current_time) {
                        min_arrival = std::min(min_arrival, temp_jobs[j].arrival_time);
                    }
                }
                if (min_arrival != INT_MAX) {
                    current_time = min_arrival;
                    i--; // Retry this iteration
                }
                continue;
            }
            
            Job& job = temp_jobs[next_idx];
            executed[next_idx] = true;
            
            job.start_time = std::max(current_time, job.arrival_time);
            job.completion_time = job.start_time + job.burst_time;
            job.calculate_waiting_time();
            job.calculate_turnaround_time();
            
            scheduled_jobs.push_back(job);
            current_time = job.completion_time;
        }
    }
    
    /**
     * Parallel Priority scheduling using OpenMP
     * 
     * Strategy:
     * - Scheduling logic must be sequential (priority selection dependency)
     * - Metric calculations parallelized where possible
     */
    void schedule_parallel() override {
        reset_jobs();
        sort_by_priority();
        
        std::vector<bool> executed(jobs.size(), false);
        int current_time = 0;
        std::vector<Job> temp_jobs = jobs;
        
        // Sequential scheduling (priority selection has dependencies)
        for (size_t i = 0; i < temp_jobs.size(); ++i) {
            int next_idx = -1;
            int best_priority = INT_MAX;
            
            for (size_t j = 0; j < temp_jobs.size(); ++j) {
                if (!executed[j] && temp_jobs[j].arrival_time <= current_time) {
                    if (temp_jobs[j].priority < best_priority) {
                        best_priority = temp_jobs[j].priority;
                        next_idx = j;
                    }
                }
            }
            
            if (next_idx == -1) {
                int min_arrival = INT_MAX;
                for (size_t j = 0; j < temp_jobs.size(); ++j) {
                    if (!executed[j] && temp_jobs[j].arrival_time > current_time) {
                        min_arrival = std::min(min_arrival, temp_jobs[j].arrival_time);
                    }
                }
                if (min_arrival != INT_MAX) {
                    current_time = min_arrival;
                    i--;
                }
                continue;
            }
            
            Job& job = temp_jobs[next_idx];
            executed[next_idx] = true;
            
            job.start_time = std::max(current_time, job.arrival_time);
            job.completion_time = job.start_time + job.burst_time;
            scheduled_jobs.push_back(job);
            current_time = job.completion_time;
        }
        
        // Parallel metric calculation
        #pragma omp parallel for num_threads(num_threads) schedule(static)
        for (size_t i = 0; i < scheduled_jobs.size(); ++i) {
            scheduled_jobs[i].calculate_waiting_time();
            scheduled_jobs[i].calculate_turnaround_time();
        }
    }
};

#endif // PRIORITY_SCHEDULER_H
