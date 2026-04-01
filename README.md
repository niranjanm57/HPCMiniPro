# Parallel HPC Task Scheduler Simulator using OpenMP

A comprehensive mini-project demonstrating CPU scheduling algorithms with parallel execution using OpenMP. This project simulates HPC (High-Performance Computing) task scheduling systems with performance analysis and comparison between sequential and parallel implementations.

## 📋 Overview

This project implements three popular CPU scheduling algorithms:
1. **First Come First Serve (FCFS)** - Jobs executed in arrival order
2. **Round Robin (RR)** - Each job gets a fixed time quantum
3. **Priority Scheduling** - Jobs executed based on priority levels

Each algorithm is implemented in both **sequential** and **parallel** versions using OpenMP, with comprehensive performance metrics and visualization.

## 🏗️ Project Structure

```
hpc_scheduler/
├── job.h                          # Job structure definition
├── scheduler.h                    # Base scheduler class (interface)
├── main.cpp                       # Main driver program
├── algorithms/
│   ├── fcfs.h                    # First Come First Serve implementation
│   ├── round_robin.h             # Round Robin implementation
│   └── priority.h                # Priority scheduling implementation
├── utils/
│   └── metrics.h                 # Metrics calculation and display utilities
└── README.md                      # This file
```

## 🎯 Core Features

### Scheduling Algorithms

#### 1. First Come First Serve (FCFS)
- **Properties**: Non-preemptive, simple
- **Time Complexity**: O(n)
- **Parallel Strategy**: Sequential timeline calculation + parallel metric computation
- **Use Case**: Fair, simple scheduling without priority consideration

#### 2. Round Robin (RR)
- **Properties**: Preemptive, fair, with configurable time quantum
- **Time Quantum**: 4ms (configurable)
- **Time Complexity**: O(n*k) where k is average scheduling iterations
- **Parallel Strategy**: Sequential timeline + parallel metrics
- **Use Case**: Time-sharing systems, fairness across jobs

#### 3. Priority Scheduling
- **Properties**: Non-preemptive, priority-based
- **Time Complexity**: O(n²) worst case
- **Parallel Strategy**: Sequential priority selection + parallel metrics
- **Use Case**: Real-time systems, importance-based execution

### Job Model

Each job includes:
```cpp
struct Job {
    int id;               // Unique identifier
    int arrival_time;     // When job arrives at queue
    int burst_time;       // CPU time required
    int priority;         // Priority level (lower = higher priority)
    int start_time;       // Execution start time
    int completion_time;  // Execution completion time
    int waiting_time;     // Time spent waiting
    int turnaround_time;  // Total time from arrival to completion
};
```

### Performance Metrics Calculated

1. **Waiting Time**: Time job waits before execution
   ```
   Waiting Time = Start Time - Arrival Time
   ```

2. **Turnaround Time**: Total time from arrival to completion
   ```
   Turnaround Time = Completion Time - Arrival Time
   ```

3. **Makespan**: Total time to complete all jobs
   ```
   Makespan = Maximum Completion Time
   ```

4. **Speedup**: Ratio of sequential to parallel execution time
   ```
   Speedup = Sequential Time / Parallel Time
   ```

5. **Efficiency**: Speedup normalized by number of threads
   ```
   Efficiency = (Speedup / Number of Threads) × 100%
   ```

### Parallel Execution with OpenMP

The project uses OpenMP pragmas for parallelization:

```cpp
// Parallel metric calculation (no dependencies)
#pragma omp parallel for num_threads(num_threads) schedule(static)
for (size_t i = 0; i < scheduled_jobs.size(); ++i) {
    scheduled_jobs[i].calculate_waiting_time();
    scheduled_jobs[i].calculate_turnaround_time();
}
```

**Parallelization Strategy**:
- **Scheduling timeline**: Sequential (inherent data dependencies)
- **Metric calculations**: Parallel (independent computations)
- **Thread scheduling**: Static schedule for load balancing

## 📥 Compilation

### Prerequisites
- **macOS**:
  ```bash
  # Install Xcode Command Line Tools (includes clang with OpenMP support)
  xcode-select --install
  
  # Or use brew to install gcc with OpenMP
  brew install gcc
  ```

- **Linux**:
  ```bash
  # Ubuntu/Debian
  sudo apt-get install build-essential libomp-dev
  
  # Fedora/RHEL
  sudo yum install gcc-c++ libomp-devel
  ```

- **Windows**:
  ```bash
  # Using MinGW-w64 (recommended)
  # Ensure g++ and libgomp are installed
  g++ --version  # Should show version with OpenMP support
  ```

### Compilation Commands

**Basic Compilation** (using clang++ on macOS with OpenMP):
```bash
# Navigate to project directory
cd /Users/niranjanmali/hpc_scheduler

# Compile with OpenMP support
clang++ -fopenmp -std=c++17 -O2 main.cpp -o scheduler

# Or using g++ (if installed via brew)
g++ -fopenmp -std=c++17 -O2 main.cpp -o scheduler
```

**Optimized Compilation**:
```bash
g++ -fopenmp -std=c++17 -O3 -march=native -ffast-math main.cpp -o scheduler
```

**With Debugging Symbols**:
```bash
g++ -fopenmp -std=c++17 -g -O0 main.cpp -o scheduler
```

**Full Example**:
```bash
# Clean previous builds
rm -f scheduler

# Compile with all optimizations
g++ -fopenmp -std=c++17 -O2 main.cpp -o scheduler

# Verify compilation
ls -la scheduler
```

## 🚀 Execution

### Run the Scheduler

```bash
# Launch the simulator
./scheduler

# With environment variables to control threads
OMP_NUM_THREADS=4 ./scheduler
```

### Sample Output Structure

```
╔════════════════════════════════════════════════════════════════════╗
║   Parallel HPC Task Scheduler Simulator using OpenMP              ║
║   Demonstrating FCFS, Round Robin, and Priority Scheduling        ║
╚════════════════════════════════════════════════════════════════════╝

Generated 10 sample jobs for scheduling:

System Information:
Maximum available threads (OpenMP): 8
Number of processors: 8

====================================== ALGORITHM: First Come First Serve (FCFS)

--- Sequential Execution ---

[Job Metrics Table]
Average Metrics:
  Average Waiting Time: 15.40 ms
  Average Turnaround Time: 25.40 ms
  Total Makespan: 39 ms

Gantt Chart:
  Time: 0        5        10       15       20       25       30       35
       |        |        |        |        |        |        |        |
Jobs:  [P1-------][P2--][P3][P4][P5---][P6-----][P7--][P8------][P9---][P10----]

Sequential Execution Time: 0.003456 ms

--- Parallel Execution (4 threads) ---

[Job Metrics Table]
Average Metrics:
  Average Waiting Time: 15.40 ms
  Average Turnaround Time: 25.40 ms
  Total Makespan: 39 ms

Gantt Chart:
[Same as above]

Parallel Execution Time: 0.002150 ms

Performance Analysis:
  Sequential Execution Time: 0.003456 ms
  Parallel Execution Time (4 threads): 0.002150 ms
  Speedup (S): 1.6064x
  Efficiency (E): 40.16%
```

## 📊 Understanding the Output

### Job Metrics Table
Shows per-job scheduling details including:
- **Arrival Time**: When job enters the system
- **Burst Time**: CPU time needed
- **Start Time**: When execution begins
- **Completion Time**: When execution finishes
- **Waiting Time**: Idle time before execution
- **Turnaround Time**: Total time in system

### Gantt Chart
Visual representation of job execution timeline:
```
[P1-------][P2--][P3][P4]...
```
- Each block represents a job
- Duration shows how long job executes
- Order shows execution sequence

### Performance Metrics
- **Speedup < 1**: Parallel slower (overhead dominates)
- **Speedup ≈ 1**: No improvement
- **Speedup > 1**: Parallel faster
- **Efficiency 100%**: Perfect linear scaling (rarely achieved)
- **Efficiency 25%**: On 4 threads, 1x speedup

## 🧪 Testing & Validation

### Verify Correct Output

Run the program and check:

1. **FCFS Scheduler**:
   - Jobs executed in arrival order
   - No job starts before its arrival time
   - Each job runs to completion

2. **Round Robin Scheduler**:
   - Each job gets max 4ms per turn
   - Longer jobs requeued multiple times
   - Fair scheduling visible in Gantt chart

3. **Priority Scheduler**:
   - Higher priority jobs (lower values) run first
   - Waiting jobs execute in priority order

4. **Metrics Validation**:
   - Completion time ≥ start time + burst time
   - Turnaround time ≥ waiting time
   - Average metrics ≥ individual minimums

### Edge Case Testing

The provided test jobs cover:
- ✓ Jobs arriving at different times
- ✓ Jobs with same arrival time
- ✓ Different priority levels
- ✓ Variable burst times (1ms to 8ms)

## 🔧 Customization

### Modify Job Set

Edit `generate_sample_jobs()` in main.cpp:

```cpp
std::vector<Job> generate_sample_jobs() {
    std::vector<Job> jobs;
    
    // Add your custom jobs
    jobs.push_back(Job(id, arrival_time, burst_time, priority));
    
    return jobs;
}
```

### Change Thread Count

Modify in `benchmark_scheduler()`:

```cpp
scheduler.set_num_threads(8);  // Change from 4 to 8 threads
```

### Adjust Round Robin Quantum

Change in main.cpp:

```cpp
RoundRobinScheduler rr_scheduler(8);  // Change from 4ms to 8ms
```

### Control OpenMP Behavior

Use environment variables:

```bash
# Set number of threads
export OMP_NUM_THREADS=8

# Disable dynamic thread adjustment
export OMP_DYNAMIC=false

# Run with nested parallelism
export OMP_NESTED=true

./scheduler
```

## 💡 Key Concepts Demonstrated

### 1. **OpenMP Parallelization**
- `#pragma omp parallel for` for loop parallelization
- Static scheduling for load balancing
- Critical sections for thread synchronization

### 2. **Scheduling Algorithms**
- Non-preemptive vs. preemptive schedulers
- Priority-based selection
- Time quantum concepts

### 3. **Performance Analysis**
- Sequential vs. parallel comparison
- Speedup calculation
- Efficiency metrics

### 4. **Thread-Safe Operations**
- Lock-free independent computation
- Dependencies awareness
- Scalability considerations

## 📈 Performance Insights

### When Parallelization Helps

Parallelization shows benefits when:
- ✓ Metric calculations on large job sets
- ✓ Multiple independent computations
- ✓ Overhead < work done per job

### When Parallelization Doesn't Help

- ✗ Small job sets (overhead dominates)
- ✗ Sequential dependencies in scheduling
- ✗ Complex synchronization needed

### Optimization Tips

1. **Increase job count** for better parallel efficiency
2. **Use `-O3` flag** for compiler optimizations
3. **Set proper thread count** based on system cores
4. **Monitor efficiency** - ideally > 50% on modern systems

## 🐛 Troubleshooting

### Compilation Error: "fopenmp not recognized"

**Solution**: 
```bash
# On macOS with clang
clang++ -Xclang -fopenmp -lomp main.cpp -o scheduler

# Or install gcc via brew
brew install gcc
/usr/local/bin/g++-11 -fopenmp main.cpp -o scheduler
```

### OpenMP not working (only 1 thread)

**Solution**:
```bash
# Check OpenMP detection
grep "Available threads" output.txt

# Set threads explicitly
export OMP_NUM_THREADS=4
./scheduler

# Check environment
echo $OMP_NUM_THREADS
```

### Inconsistent results across runs

**Normal behavior** - parallelization timing varies with system load.

Use average of multiple runs:
```bash
for i in {1..5}; do ./scheduler | grep "Speedup"; done
```

## 📚 Learning Resources

### Concepts Covered
1. **CPU Scheduling**: FCFS, RR, Priority Scheduling
2. **Parallelization**: OpenMP, thread distribution
3. **Performance Analysis**: Speedup, efficiency calculations
4. **Data Structures**: Queues, sorted arrays, Gantt charts

### Further Improvements
- Implement SRTF (Shortest Remaining Time First)
- Add CUDA kernel for heavy job simulation
- Implement advanced OpenMP features (task-based parallelism)
- Add interactive user input for job configuration
- Create GUI visualization for Gantt charts
- Implement load balancing algorithms

## 📝 Code Standards

The project follows best practices:
- ✓ Clear comments explaining logic
- ✓ Meaningful variable names
- ✓ Modular design with separations of concerns
- ✓ Header guards to prevent multiple includes
- ✓ Const-correct code
- ✓ Efficient algorithms (no unnecessary loops)

## 🎓 Exam Preparation

This project is ideal for:
- ✓ SPPU BE HPC Practical
- ✓ Operating Systems courses
- ✓ Parallel Computing assignments
- ✓ Job scheduling interviews
- ✓ Performance analysis demonstrations

## 📋 Quick Reference Card

| Component | Details |
|-----------|---------|
| Language | C++17 |
| Parallelization | OpenMP |
| Algorithms | 3 (FCFS, RR, Priority) |
| Versions | 2 (Sequential, Parallel) |
| Jobs | 10 sample jobs |
| Metrics | 5+ performance metrics |
| Compilation | Single command |
| Threads | Configurable (1-N) |
| Time Quantum | 4ms (RR) |
| Max Jobs | Unlimited |

## 📞 Support & Questions

For issues or questions:

1. **Check compilation flags**: Ensure OpenMP is enabled
2. **Verify environment**: Check `echo $OMP_NUM_THREADS`
3. **Test with small set**: Start with provided 10 jobs
4. **Check system resources**: Monitor CPU and memory usage

## ✅ Verification Checklist

- [ ] Code compiles without errors
- [ ] Program runs without crashes
- [ ] All three algorithms execute
- [ ] Sequential and parallel versions work
- [ ] Performance metrics display correctly
- [ ] Gantt charts show correct job order
- [ ] Speedup > 1 for large job sets
- [ ] Efficiency ≥ 25% on good systems

## 📄 License

This project is developed for educational purposes. Feel free to use, modify, and distribute for learning.

---

**Created**: April 2026
**Purpose**: SPPU BE HPC Practical Mini-Project
**Status**: Complete and tested ✓
