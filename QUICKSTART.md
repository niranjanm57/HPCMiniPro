# Quick Start Guide - HPC Task Scheduler Simulator

## 🚀 5-Minute Quick Start

### On macOS

```bash
# 1. Navigate to the project directory
cd /Users/niranjanmali/hpc_scheduler

# 2. Make the compilation script executable
chmod +x compile.sh

# 3. Run the compilation script
./compile.sh

# 4. Execute the scheduler
./scheduler

# 5. (Optional) Run with 8 threads
OMP_NUM_THREADS=8 ./scheduler
```

### Direct Compilation (if compile.sh doesn't work)

```bash
# Using Homebrew GCC (recommended for macOS)
/opt/homebrew/bin/g++-15 -fopenmp -std=c++17 -O2 main.cpp -o scheduler

# Then run:
./scheduler
```

---

## 📋 What You Should See

1. **Header Section**: 
   - Title of the simulator
   - 10 sample jobs listed with their properties

2. **System Information**:
   - Available threads: 8
   - Number of processors: 8

3. **Three Algorithm Sections** (FCFS, Round Robin, Priority):
   - Sequential execution results
   - Job metrics table
   - Average metrics (waiting time, turnaround time, makespan)
   - Gantt chart
   - Execution time

   - Parallel execution results (same structure)
   - Performance comparison (Speedup, Efficiency)

4. **Final Summary**: Confirmation that all components worked

---

## 🧪 Expected Output Example

```
════════════════════════════════════════════════════════════════════
Algorithm: First Come First Serve (FCFS)
════════════════════════════════════════════════════════════════════

--- Sequential Execution ---

Job ID  Arrival Time  Burst Time  Priority ...
P1      0             8           3       ...
P2      1             4           1       ...
...

Average Waiting Time:         13.50 ms
Average Turnaround Time:      17.30 ms
Total Makespan:               38 ms

Gantt Chart:
Time: 0        5        10       15       20       25       30       35
      |        |        |        |        |        |        |        |
Jobs: [P1    ][P2][P3][P4][P5][P6 ][P7][P8  ][P9][P10]

Sequential Execution Time: 0.001000 ms

--- Parallel Execution (4 threads) ---
[Parallel execution data...]

Performance Analysis:
Sequential Execution Time:         0.0010 ms
Parallel Execution Time (4 threads): 0.2050 ms
Speedup (S):                       0.0049x
Efficiency (E):                    0.12%
```

---

## ⚙️ System Requirements

### Compiler Setup

**macOS**:
```bash
# Install GCC with OpenMP
brew install gcc

# Verify installation
/opt/homebrew/bin/g++-15 --version
```

**Linux**:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential libomp-dev

# Fedora/RHEL
sudo yum install gcc-c++ libomp-devel
```

**Windows (MinGW)**:
```bash
# Install MinGW-w64 with GCC and OpenMP support
# Then use: g++ -fopenmp ...
```

---

## 📊 Understanding the Output

### Job Metrics Table
- **Job ID**: Process identifier
- **Arrival Time**: When job enters the system
- **Burst Time**: CPU cycles needed
- **Priority**: Lower number = higher priority
- **Start Time**: When CPU actually starts this job
- **Completion Time**: When job finishes
- **Waiting Time**: Time job waits (before execution starts)
- **Turnaround Time**: Total time in system

### Gantt Chart
Visual representation showing:
- **Timeline**: X-axis shows milliseconds
- **Jobs**: Each job represented as a block
- **Duration**: Block width = execution time
- **Order**: Visual sequence of execution

### Performance Metrics
- **Speedup**: How much faster parallel vs sequential
  - > 1 = parallel is faster
  - < 1 = sequential is faster (overhead dominates)
  
- **Efficiency**: Parallel speedup normalized by thread count
  - 100% = perfect scaling (rarely achieved)
  - 25% = reasonable scaling on modern systems

---

## 🎮 Customization

### Change Number of Threads

Edit in `main.cpp`, function `benchmark_scheduler()`:

```cpp
scheduler.set_num_threads(8);  // Change from 4 to 8
```

Or use environment variable:
```bash
OMP_NUM_THREADS=8 ./scheduler
```

### Modify Time Quantum (Round Robin)

Edit in `main.cpp`:

```cpp
RoundRobinScheduler rr_scheduler(8);  // Change from 4ms to 8ms
```

### Add Custom Jobs

Edit `generate_sample_jobs()` function in `main.cpp`:

```cpp
std::vector<Job> generate_sample_jobs() {
    std::vector<Job> jobs;
    
    // Job(ID, ArrivalTime, BurstTime, Priority)
    jobs.push_back(Job(1, 0, 10, 2));
    jobs.push_back(Job(2, 2, 5, 1));
    // Add more jobs...
    
    return jobs;
}
```

---

## 🐛 Troubleshooting

### "command not found: g++-15"
→ Install GCC: `brew install gcc`

### "fopenmp not recognized"
→ Use: `clang++ -Xclang -fopenmp -lomp -std=c++17 -O2 main.cpp -o scheduler`

### "fatal error: 'iostream' file not found"
→ Use full path to compiler or reinstall Xcode Command Line Tools:
```bash
xcode-select --install
```

### Program runs very slow
→ Parallelization overhead for small datasets. Use larger job sets or higher thread counts.

### Speedup < 1
→ Normal for small computations. Parallelization overhead > work done.

---

## 💾 Files Description

| File | Purpose |
|------|---------|
| `main.cpp` | Main driver program with test cases |
| `job.h` | Job structure and definitions |
| `scheduler.h` | Base scheduler class interface |
| `algorithms/fcfs.h` | FCFS implementation |
| `algorithms/round_robin.h` | Round Robin implementation |
| `algorithms/priority.h` | Priority scheduling implementation |
| `utils/metrics.h` | Metrics calculation and display |
| `README.md` | Comprehensive documentation |
| `compile.sh` | Compilation helper script |
| `scheduler` | Compiled executable (after compilation) |

---

## ✅ Verification Checklist

- [ ] Code compiles without errors
- [ ] Program runs and shows output
- [ ] All 3 algorithms execute (FCFS, RR, Priority)
- [ ] Metrics display (waiting time, turnaround time)
- [ ] Gantt charts appear
- [ ] Speedup values shown
- [ ] Efficiency percentages calculated
- [ ] No crashes or segmentation faults

---

## 📚 Learning Points

This project teaches:

1. **CPU Scheduling Algorithms**: How operating systems schedule processes
2. **OpenMP Parallelization**: Practical parallel programming on shared-memory systems
3. **Performance Analysis**: Measuring and comparing algorithm efficiency
4. **Data Structures**: Queues, arrays, custom structures
5. **Algorithm Complexity**: Time and space analysis
6. **C++ Features**: Classes, templates, modern C++ practices

---

## 🎓 Use Cases

- **Academic**: Exam preparation, coursework, learning HPC
- **Interview Preparation**: Scheduling algorithm questions
- **Research**: Baseline for scheduler modifications
- **Teaching**: Demonstrating parallel concepts

---

## 📞 Quick Reference

| Task | Command |
|------|---------|
| Compile | `./compile.sh` or `/opt/homebrew/bin/g++-15 -fopenmp -std=c++17 -O2 main.cpp -o scheduler` |
| Run | `./scheduler` |
| Run with 8 threads | `OMP_NUM_THREADS=8 ./scheduler` |
| Clean build | `rm -f scheduler && ./compile.sh` |
| Check compiler | `/opt/homebrew/bin/g++-15 --version` |
| View code | `cat main.cpp`, `cat algorithms/fcfs.h`, etc. |

---

**Last Updated**: April 1, 2026
**Status**: Ready for exam ✓
