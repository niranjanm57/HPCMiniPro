#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include "../scheduler.h"
#include <queue>
#include <omp.h>

/**
 * @class RoundRobinScheduler
 * @brief Round Robin (RR) Scheduling
 * 
 * Each job is assigned a fixed time quantum (time slice).
 * Jobs are executed in a circular queue manner.
 * Preemptive scheduling algorithm.
 */
class RoundRobinScheduler : public Scheduler {
private:
    int time_quantum = 4;  // Default time quantum
    
    /**
     * Sort jobs by arrival time for initial queue
     */
    void sort_by_arrival_time() {
        std::sort(jobs.begin(), jobs.end(), 
                 [](const Job& a, const Job& b) {
                     return a.arrival_time < b.arrival_time;
                 });
    }
    
public:
    RoundRobinScheduler(int quantum = 4) : time_quantum(quantum) {}
    
    void set_time_quantum(int quantum) {
        time_quantum = quantum;
    }
    
    const char* get_algorithm_name() const override {
        return "Round Robin (RR)";
    }
    
    /**
     * Sequential Round Robin scheduling
     * Time Complexity: O(n * k) where k is average number of scheduling passes
     */
    void schedule_sequential() override {
        reset_jobs();
        sort_by_arrival_time();
        
        // Create Gantt chart representation
        std::vector<int> gantt_time;
        std::vector<int> gantt_job;
        
        std::queue<Job> ready_queue;
        int current_time = 0;
        size_t job_idx = 0;
        
        // Initialize: add jobs that arrive at time 0
        while (job_idx < jobs.size() && jobs[job_idx].arrival_time <= current_time) {
            ready_queue.push(jobs[job_idx]);
            job_idx++;
        }
        
        // Process jobs
        while (!ready_queue.empty()) {
            Job job = ready_queue.front();
            ready_queue.pop();
            
            // If job starts execution now, record start time
            if (job.start_time == -1) {
                job.start_time = std::max(current_time, job.arrival_time);
            }
            
            // Execute for time quantum or until completion
            int execution_time = std::min(time_quantum, job.burst_time);
            
            gantt_time.push_back(current_time);
            gantt_job.push_back(job.id);
            
            job.burst_time -= execution_time;
            current_time += execution_time;
            
            // Add newly arrived jobs to queue
            while (job_idx < jobs.size() && jobs[job_idx].arrival_time <= current_time) {
                ready_queue.push(jobs[job_idx]);
                job_idx++;
            }
            
            // If job not completed, add back to queue
            if (job.burst_time > 0) {
                ready_queue.push(job);
            } else {
                // Job completed
                job.completion_time = current_time;
                job.calculate_waiting_time();
                job.calculate_turnaround_time();
                scheduled_jobs.push_back(job);
            }
        }
    }
    
    /**
     * Parallel Round Robin scheduling using OpenMP
     * 
     * Strategy:
     * - Execute the scheduling timeline sequentially (inherent dependency)
     * - Parallelize metric calculations
     */
    void schedule_parallel() override {
        reset_jobs();
        sort_by_arrival_time();
        
        std::queue<Job> ready_queue;
        int current_time = 0;
        size_t job_idx = 0;
        
        // Initialize queue with arrival jobs
        while (job_idx < jobs.size() && jobs[job_idx].arrival_time <= current_time) {
            ready_queue.push(jobs[job_idx]);
            job_idx++;
        }
        
        // Process jobs (sequential due to time dependencies)
        while (!ready_queue.empty()) {
            Job job = ready_queue.front();
            ready_queue.pop();
            
            if (job.start_time == -1) {
                job.start_time = std::max(current_time, job.arrival_time);
            }
            
            int execution_time = std::min(time_quantum, job.burst_time);
            job.burst_time -= execution_time;
            current_time += execution_time;
            
            // Add newly arrived jobs (can be parallelized but overhead not worth it)
            while (job_idx < jobs.size() && jobs[job_idx].arrival_time <= current_time) {
                ready_queue.push(jobs[job_idx]);
                job_idx++;
            }
            
            if (job.burst_time > 0) {
                ready_queue.push(job);
            } else {
                job.completion_time = current_time;
                scheduled_jobs.push_back(job);
            }
        }
        
        // Parallel metric calculation
        #pragma omp parallel for num_threads(num_threads) schedule(static)
        for (size_t i = 0; i < scheduled_jobs.size(); ++i) {
            scheduled_jobs[i].calculate_waiting_time();
            scheduled_jobs[i].calculate_turnaround_time();
        }
    }
};

#endif // ROUND_ROBIN_SCHEDULER_H
