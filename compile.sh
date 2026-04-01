#!/bin/bash

# ============================================================================
# HPC Task Scheduler Simulator - Compilation Script
# ============================================================================
# This script provides easy compilation of the project on macOS
# 
# Supported compilers:
#   1. GCC 15 (via Homebrew) - RECOMMENDED
#   2. Clang with OpenMP (requires installation)
#
# ============================================================================

echo "╔════════════════════════════════════════════════════════════════════╗"
echo "║  HPC Task Scheduler Simulator - Compilation Script               ║"
echo "╚════════════════════════════════════════════════════════════════════╝"
echo ""

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Project directory
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Check for GCC
check_gcc() {
    if [ -x "/opt/homebrew/bin/g++-15" ]; then
        echo -e "${GREEN}✓${NC} Found GCC 15 at /opt/homebrew/bin/g++-15"
        return 0
    elif command -v g++ &> /dev/null; then
        echo -e "${GREEN}✓${NC} Found g++"
        return 0
    else
        echo -e "${RED}✗${NC} GCC not found"
        return 1
    fi
}

# Compile function
compile_project() {
    local compiler=$1
    local output_file="scheduler"
    
    echo ""
    echo "Compiling with: $compiler"
    echo "Project directory: $PROJECT_DIR"
    echo "Output file: $output_file"
    echo ""
    
    cd "$PROJECT_DIR"
    
    # Compile with OpenMP support
    echo "Running: $compiler -fopenmp -std=c++17 -O2 main.cpp -o $output_file"
    
    if $compiler -fopenmp -std=c++17 -O2 main.cpp -o $output_file 2>&1; then
        echo ""
        echo -e "${GREEN}✓ Compilation successful!${NC}"
        echo ""
        
        # Show file info
        if [ -f "$output_file" ]; then
            SIZE=$(ls -lh "$output_file" | awk '{print $5}')
            echo "Generated executable: $output_file ($SIZE)"
            echo ""
            echo "To run the scheduler:"
            echo "  cd $PROJECT_DIR"
            echo "  ./$output_file"
            echo ""
            echo "Or with custom thread count:"
            echo "  OMP_NUM_THREADS=8 ./$output_file"
            echo ""
        fi
        return 0
    else
        echo ""
        echo -e "${RED}✗ Compilation failed!${NC}"
        return 1
    fi
}

# Main logic
main() {
    echo "Checking for compilers..."
    echo ""
    
    # Check for GCC
    if check_gcc; then
        if [ -x "/opt/homebrew/bin/g++-15" ]; then
            compile_project "/opt/homebrew/bin/g++-15"
        else
            compile_project "g++"
        fi
    else
        echo ""
        echo -e "${YELLOW}To install GCC with OpenMP support:${NC}"
        echo "  brew install gcc"
        echo ""
        return 1
    fi
}

# Run main function
main
