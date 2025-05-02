#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Get the absolute path to the project directory
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${PROJECT_DIR}/build"
TEST_DIR="${PROJECT_DIR}/tests"
PARSER="${BUILD_DIR}/parser"
FIXTURES_DIR="${TEST_DIR}/fixtures"
INTEGRATION_DIR="${TEST_DIR}/integration"
EXPECTED_DIR="${TEST_DIR}/expected"
RESULTS_DIR="${TEST_DIR}/results"
OUTPUT_DIR="${PROJECT_DIR}/output"

# Create directories if they don't exist
mkdir -p "${RESULTS_DIR}"
mkdir -p "${EXPECTED_DIR}"

# Check if parser exists
if [ ! -f "${PARSER}" ]; then
    echo -e "${RED}Error: Parser executable not found at: ${PARSER}${NC}"
    echo -e "${YELLOW}Make sure you've built the project first.${NC}"
    exit 1
fi

echo -e "${BLUE}Using parser at: ${PARSER}${NC}"

# Track test results
PASSED=0
FAILED=0
TOTAL=0

# Function to run a single test
run_test() {
    local test_file="$1"
    local filename=$(basename "$test_file")
    
    echo -n "Testing ${filename}... "
    TOTAL=$((TOTAL + 1))
    
    # Clear any previous output files or set a specific output file for this test
    rm -f ${OUTPUT_DIR}/output_* 2>/dev/null
    
    # Run the test - debug output goes to terminal
    "${PARSER}" "${test_file}" interpret
    local run_status=$?
    
    if [ $run_status -ne 0 ]; then
        echo -e "${RED}EXECUTION FAILED (exit code: ${run_status})${NC}"
        FAILED=$((FAILED + 1))
        return 1
    fi
    
    # Find the most recent output file
    local output_file=$(ls -t ${OUTPUT_DIR}/output_* | head -1)
    
    if [ -z "$output_file" ]; then
        echo -e "${RED}No output file found in ${OUTPUT_DIR}${NC}"
        FAILED=$((FAILED + 1))
        return 1
    fi
    
    # Copy the most recent output to results directory
    cp "$output_file" "${RESULTS_DIR}/${filename}.out"
    
    # Compare with expected output
    local expected_file="${EXPECTED_DIR}/${filename}.expected"
    if [ -f "${expected_file}" ]; then
        if diff -w "${expected_file}" "${RESULTS_DIR}/${filename}.out" > /dev/null; then
            echo -e "${GREEN}PASSED${NC}"
            PASSED=$((PASSED + 1))
            return 0
        else
            echo -e "${RED}FAILED (output mismatch)${NC}"
            echo "Expected:"
            cat "${expected_file}"
            echo "Got:"
            cat "${RESULTS_DIR}/${filename}.out"
            FAILED=$((FAILED + 1))
            return 1
        fi
    else
        echo -e "${YELLOW}WARNING: No expected output file found at: ${expected_file}${NC}"
        echo "Output was:"
        cat "${RESULTS_DIR}/${filename}.out"
        return 0
    fi
}

# Main function to run tests
run_tests() {
    echo -e "${BLUE}======================================${NC}"
    echo -e "${BLUE}   Running Language Tests   ${NC}"
    echo -e "${BLUE}======================================${NC}"
    
    # Check if a specific test was requested
    if [ "$1" != "" ]; then
        if [ -f "$1" ]; then
            run_test "$1"
        else
            echo -e "${RED}Test file not found: $1${NC}"
            exit 1
        fi
    else
        # Run fixture tests
        if [ -d "${FIXTURES_DIR}" ] && [ "$(ls -A ${FIXTURES_DIR})" ]; then
            echo -e "${YELLOW}Running fixture tests...${NC}"
            for test_file in "${FIXTURES_DIR}"/*.txt; do
                run_test "${test_file}"
            done
        else
            echo -e "${YELLOW}No fixture tests found in ${FIXTURES_DIR}${NC}"
        fi
        
        # Run integration tests
        if [ -d "${INTEGRATION_DIR}" ] && [ "$(ls -A ${INTEGRATION_DIR})" ]; then
            echo -e "${YELLOW}Running integration tests...${NC}"
            for test_file in "${INTEGRATION_DIR}"/*.txt; do
                run_test "${test_file}"
            done
        else
            echo -e "${YELLOW}No integration tests found in ${INTEGRATION_DIR}${NC}"
        fi
    fi
    
    # Print summary
    echo -e "${BLUE}======================================${NC}"
    echo -e "${YELLOW}Test Summary:${NC}"
    echo "Total: ${TOTAL}"
    echo -e "${GREEN}Passed: ${PASSED}${NC}"
    
    if [ ${FAILED} -gt 0 ]; then
        echo -e "${RED}Failed: ${FAILED}${NC}"
        return 1
    else
        echo "Failed: ${FAILED}"
        return 0
    fi
}

# Run the tests
run_tests "$1"
exit $?