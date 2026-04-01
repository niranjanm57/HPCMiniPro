#ifndef METRICS_H
#define METRICS_H

#include "../job.h"
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>

/**
 * @class SchedulingMetrics
 * @brief Utility class for calculating and displaying scheduling metrics
 */
class SchedulingMetrics {
public:
    /**
     * Display a formatted table of job scheduling results
     */
    static void display_job_metrics(const std::vector<Job>& jobs) {
        if (jobs.empty()) {
            std::cout << "No jobs to display.\n";
            return;
        }
        
        std::cout << "\n" << std::string(120, '=') << "\n";
        std::cout << std::left 
                  << std::setw(6) << "Job ID"
                  << std::setw(14) << "Arrival Time"
                  << std::setw(12) << "Burst Time"
                  << std::setw(10) << "Priority"
                  << std::setw(12) << "Start Time"
                  << std::setw(16) << "Completion Time"
                  << std::setw(13) << "Waiting Time"
                  << std::setw(16) << "Turnaround Time"
                  << "\n";
        std::cout << std::string(120, '-') << "\n";
        
        for (const auto& job : jobs) {
            std::cout << std::left 
                      << std::setw(6) << "P" + std::to_string(job.id)
                      << std::setw(14) << job.arrival_time
                      << std::setw(12) << job.burst_time
                      << std::setw(10) << job.priority
                      << std::setw(12) << job.start_time
                      << std::setw(16) << job.completion_time
                      << std::setw(13) << job.waiting_time
                      << std::setw(16) << job.turnaround_time
                      << "\n";
        }
        std::cout << std::string(120, '=') << "\n";
    }
    
    /**
     * Display average metrics
     */
    static void display_average_metrics(const std::vector<Job>& jobs) {
        if (jobs.empty()) return;
        
        double avg_waiting = 0, avg_turnaround = 0;
        int total_wait = 0, total_turnaround = 0;
        
        for (const auto& job : jobs) {
            total_wait += job.waiting_time;
            total_turnaround += job.turnaround_time;
        }
        
        avg_waiting = (double)total_wait / jobs.size();
        avg_turnaround = (double)total_turnaround / jobs.size();
        
        std::cout << "\n" << std::string(60, '-') << "\n";
        std::cout << "Average Metrics:\n";
        std::cout << std::string(60, '-') << "\n";
        std::cout << std::left 
                  << std::setw(30) << "Average Waiting Time: "
                  << std::fixed << std::setprecision(2) << avg_waiting << " ms\n"
                  << std::setw(30) << "Average Turnaround Time: "
                  << std::fixed << std::setprecision(2) << avg_turnaround << " ms\n";
        
        // Calculate makespan
        int makespan = 0;
        for (const auto& job : jobs) {
            makespan = std::max(makespan, job.completion_time);
        }
        std::cout << std::setw(30) << "Total Makespan: "
                  << makespan << " ms\n";
        std::cout << std::string(60, '-') << "\n";
    }
    
    /**
     * Display a simple Gantt chart
     */
    static void display_gantt_chart(const std::vector<Job>& jobs) {
        if (jobs.empty()) return;
        
        // Sort jobs by start time for Gantt chart visualization
        std::vector<Job> sorted_jobs = jobs;
        std::sort(sorted_jobs.begin(), sorted_jobs.end(), 
                 [](const Job& a, const Job& b) {
                     return a.start_time < b.start_time;
                 });
        
        std::cout << "\nGantt Chart:\n";
        std::cout << std::string(80, '-') << "\n";
        
        // Time axis with job blocks
        std::cout << "Time: ";
        for (int i = 0; i <= sorted_jobs.back().completion_time; i += 5) {
            std::cout << std::setw(9) << i;
        }
        std::cout << "\n";
        
        std::cout << "       ";
        for (size_t i = 0; i < sorted_jobs.back().completion_time; i += 5) {
            std::cout << "|        ";
        }
        std::cout << "\n";
        
        // Job blocks
        std::cout << "Jobs:  ";
        int last_time = 0;
        for (const auto& job : sorted_jobs) {
            // Add spacing
            for (int i = 0; i < job.start_time - last_time; ++i) {
                std::cout << " ";
            }
            
            // Add job block
            int block_width = job.completion_time - job.start_time;
            std::string job_label = "P" + std::to_string(job.id);
            
            // Ensure minimum width
            if (block_width < (int)job_label.length()) {
                block_width = job_label.length();
            }
            
            std::cout << "[" << std::setw(block_width - 2) 
                      << std::left << job_label << "]";
            last_time = job.completion_time;
        }
        std::cout << "\n";
        std::cout << std::string(80, '-') << "\n";
    }
    
    /**
     * Display performance comparison between sequential and parallel
     */
    static void display_performance_comparison(double seq_time_ms, double par_time_ms, int num_threads) {
        std::cout << "\n" << std::string(70, '=') << "\n";
        std::cout << "Performance Analysis:\n";
        std::cout << std::string(70, '=') << "\n";
        
        std::cout << std::left 
                  << std::setw(35) << "Sequential Execution Time: "
                  << std::fixed << std::setprecision(4) << seq_time_ms << " ms\n"
                  << std::setw(35) << "Parallel Execution Time (" 
                  << num_threads << " threads): "
                  << std::fixed << std::setprecision(4) << par_time_ms << " ms\n";
        
        double speedup = seq_time_ms / (par_time_ms > 0 ? par_time_ms : 1);
        double efficiency = (speedup / num_threads) * 100;
        
        std::cout << std::setw(35) << "Speedup (S): "
                  << std::fixed << std::setprecision(4) << speedup << "x\n"
                  << std::setw(35) << "Efficiency (E): "
                  << std::fixed << std::setprecision(2) << efficiency << "%\n";
        
        std::cout << std::string(70, '=') << "\n\n";
    }
};

#endif // METRICS_H
