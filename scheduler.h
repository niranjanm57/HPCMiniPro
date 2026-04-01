#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"
#include <vector>
#include <queue>
#include <chrono>

/**
 * @class Scheduler
 * @brief Base class for CPU scheduling algorithms
 * 
 * Provides interface for implementing different scheduling strategies
 * with both sequential and parallel versions
 */
class Scheduler {
protected:
    std::vector<Job> jobs;
    std::vector<Job> scheduled_jobs;
    int num_threads = 1;
    
public:
    Scheduler() = default;
    virtual ~Scheduler() = default;
    
    /**
     * Set the number of threads for parallel execution
     */
    void set_num_threads(int threads) {
        num_threads = threads;
    }
    
    /**
     * Add a job to the scheduler
     */
    void add_job(const Job& job) {
        jobs.push_back(job);
    }
    
    /**
     * Add multiple jobs to the scheduler
     */
    void add_jobs(const std::vector<Job>& job_list) {
        jobs.insert(jobs.end(), job_list.begin(), job_list.end());
    }
    
    /**
     * Get the scheduled jobs
     */
    const std::vector<Job>& get_scheduled_jobs() const {
        return scheduled_jobs;
    }
    
    /**
     * Virtual methods to implement scheduling algorithms
     */
    virtual void schedule_sequential() = 0;
    virtual void schedule_parallel() = 0;
    
    /**
     * Calculate average waiting time
     */
    double get_avg_waiting_time() const {
        if (scheduled_jobs.empty()) return 0.0;
        double sum = 0;
        for (const auto& job : scheduled_jobs) {
            sum += job.waiting_time;
        }
        return sum / scheduled_jobs.size();
    }
    
    /**
     * Calculate average turnaround time
     */
    double get_avg_turnaround_time() const {
        if (scheduled_jobs.empty()) return 0.0;
        double sum = 0;
        for (const auto& job : scheduled_jobs) {
            sum += job.turnaround_time;
        }
        return sum / scheduled_jobs.size();
    }
    
    /**
     * Get total execution time (makespan)
     */
    int get_makespan() const {
        if (scheduled_jobs.empty()) return 0;
        int max_completion = 0;
        for (const auto& job : scheduled_jobs) {
            if (job.completion_time > max_completion) {
                max_completion = job.completion_time;
            }
        }
        return max_completion;
    }
    
    /**
     * Reset jobs for re-scheduling
     */
    void reset_jobs() {
        for (auto& job : jobs) {
            job.reset();
        }
        scheduled_jobs.clear();
    }
    
    /**
     * Get the algorithm name
     */
    virtual const char* get_algorithm_name() const = 0;
};

#endif // SCHEDULER_H
