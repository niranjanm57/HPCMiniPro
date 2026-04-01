#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

#include "job.h"
#include "scheduler.h"
#include "algorithms/fcfs.h"
#include "algorithms/round_robin.h"
#include "algorithms/priority.h"
#include "utils/metrics.h"

/**
 * Generate sample jobs for testing
 * 
 * Creating 10 diverse jobs to test scheduler performance
 */
std::vector<Job> generate_sample_jobs() {
    std::vector<Job> jobs;
    
    // Job format: Job(ID, Arrival_Time, Burst_Time, Priority)
    // Lower priority value = higher priority
    jobs.push_back(Job(1, 0, 8, 3));      // P1: arrives at 0, needs 8ms, low priority
    jobs.push_back(Job(2, 1, 4, 1));      // P2: arrives at 1, needs 4ms, high priority
    jobs.push_back(Job(3, 2, 2, 4));      // P3: arrives at 2, needs 2ms, low priority
    jobs.push_back(Job(4, 3, 1, 2));      // P4: arrives at 3, needs 1ms, medium priority
    jobs.push_back(Job(5, 4, 3, 3));      // P5: arrives at 4, needs 3ms, low priority
    jobs.push_back(Job(6, 5, 5, 1));      // P6: arrives at 5, needs 5ms, high priority
    jobs.push_back(Job(7, 6, 2, 2));      // P7: arrives at 6, needs 2ms, medium priority
    jobs.push_back(Job(8, 7, 6, 4));      // P8: arrives at 7, needs 6ms, low priority
    jobs.push_back(Job(9, 8, 3, 1));      // P9: arrives at 8, needs 3ms, high priority
    jobs.push_back(Job(10, 9, 4, 2));     // P10: arrives at 9, needs 4ms, medium priority
    
    return jobs;
}

/**
 * Benchmark a scheduler algorithm
 */
void benchmark_scheduler(Scheduler& scheduler, const std::string& algo_name) {
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << "Algorithm: " << algo_name << "\n";
    std::cout << std::string(100, '=') << "\n";
    
    // Sequential execution
    auto start_seq = std::chrono::high_resolution_clock::now();
    scheduler.schedule_sequential();
    auto end_seq = std::chrono::high_resolution_clock::now();
    
    double seq_time = std::chrono::duration<double, std::milli>(end_seq - start_seq).count();
    
    auto seq_jobs = scheduler.get_scheduled_jobs();
    
    std::cout << "\n--- Sequential Execution ---\n";
    SchedulingMetrics::display_job_metrics(seq_jobs);
    SchedulingMetrics::display_average_metrics(seq_jobs);
    SchedulingMetrics::display_gantt_chart(seq_jobs);
    std::cout << "Sequential Execution Time: " << std::fixed << std::setprecision(6) 
              << seq_time << " ms\n";
    
    // Parallel execution
    scheduler.set_num_threads(4);  // Use 4 threads
    auto start_par = std::chrono::high_resolution_clock::now();
    scheduler.schedule_parallel();
    auto end_par = std::chrono::high_resolution_clock::now();
    
    double par_time = std::chrono::duration<double, std::milli>(end_par - start_par).count();
    
    auto par_jobs = scheduler.get_scheduled_jobs();
    
    std::cout << "\n--- Parallel Execution (4 threads) ---\n";
    SchedulingMetrics::display_job_metrics(par_jobs);
    SchedulingMetrics::display_average_metrics(par_jobs);
    SchedulingMetrics::display_gantt_chart(par_jobs);
    std::cout << "Parallel Execution Time: " << std::fixed << std::setprecision(6) 
              << par_time << " ms\n";
    
    // Performance comparison
    SchedulingMetrics::display_performance_comparison(seq_time, par_time, 4);
}

/**
 * Main program demonstrating HPC Task Scheduler Simulator
 */
int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║   Parallel HPC Task Scheduler Simulator using OpenMP              ║\n";
    std::cout << "║   Demonstrating FCFS, Round Robin, and Priority Scheduling        ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════════╝\n";
    
    // Generate sample jobs
    auto jobs = generate_sample_jobs();
    
    std::cout << "\nGenerated " << jobs.size() << " sample jobs for scheduling:\n";
    std::cout << std::string(100, '-') << "\n";
    SchedulingMetrics::display_job_metrics(jobs);
    
    // Information about the system
    std::cout << "\nSystem Information:\n";
    std::cout << "Maximum available threads (OpenMP): " << omp_get_max_threads() << "\n";
    std::cout << "Number of processors: " << omp_get_num_procs() << "\n";
    std::cout << std::string(100, '-') << "\n";
    
    // ============= FCFS Scheduling =============
    {
        FCFSScheduler fcfs_scheduler;
        fcfs_scheduler.add_jobs(jobs);
        benchmark_scheduler(fcfs_scheduler, "First Come First Serve (FCFS)");
    }
    
    // ============= Round Robin Scheduling =============
    {
        RoundRobinScheduler rr_scheduler(4);  // Time quantum = 4ms
        rr_scheduler.add_jobs(jobs);
        benchmark_scheduler(rr_scheduler, "Round Robin (Time Quantum = 4ms)");
    }
    
    // ============= Priority Scheduling =============
    {
        PriorityScheduler priority_scheduler;
        priority_scheduler.add_jobs(jobs);
        benchmark_scheduler(priority_scheduler, "Priority Scheduling");
    }
    
    // Summary comparison
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << "SUMMARY:\n";
    std::cout << std::string(100, '=') << "\n";
    std::cout << "✓ All three scheduling algorithms implemented and tested\n";
    std::cout << "✓ Both sequential and parallel versions executed\n";
    std::cout << "✓ Performance metrics calculated (waiting time, turnaround time, speedup, efficiency)\n";
    std::cout << "✓ Gantt charts displayed for visualization\n";
    std::cout << "✓ OpenMP parallelization demonstrated\n";
    std::cout << std::string(100, '=') << "\n\n";
    
    return 0;
}
