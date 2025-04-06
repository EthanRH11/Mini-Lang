#!/bin/bash

# Enhanced C++ run script with comprehensive error handling
# --------------------------------------------------------

# Set script to exit on error
set -e

# Configuration variables
PROJECT_NAME="parser"
BUILD_DIR="../build"
EXECUTABLE="${BUILD_DIR}/${PROJECT_NAME}"
TEST_DATA_DIR="../test/data"
INPUT_FILE="${TEST_DATA_DIR}/simple_input.txt"

# Create color codes for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print banner
echo -e "${BLUE}======================================${NC}"
echo -e "${BLUE}   Running ${PROJECT_NAME}   ${NC}"
echo -e "${BLUE}======================================${NC}"

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}Error: Executable not found at: ${EXECUTABLE}${NC}"
    echo -e "${YELLOW}Did you run the build script first?${NC}"
    echo -e "${YELLOW}Trying to build now...${NC}"
    
    # Check if build script exists and attempt to run it
    BUILD_SCRIPT="./build.sh"
    if [ -f "$BUILD_SCRIPT" ]; then
        echo -e "${YELLOW}Running build script: ${BUILD_SCRIPT}${NC}"
        bash "$BUILD_SCRIPT"
        
        # Check again if executable was created
        if [ ! -f "$EXECUTABLE" ]; then
            echo -e "${RED}Build script ran but executable still not found.${NC}"
            exit 1
        fi
    else
        echo -e "${RED}Build script not found at: ${BUILD_SCRIPT}${NC}"
        exit 1
    fi
fi

# Check if executable has execute permissions
if [ ! -x "$EXECUTABLE" ]; then
    echo -e "${YELLOW}Adding execute permission to executable...${NC}"
    chmod +x "$EXECUTABLE"
fi

# Check if input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo -e "${RED}Error: Input file not found at: ${INPUT_FILE}${NC}"
    
    # List available test files
    echo -e "${YELLOW}Available test files:${NC}"
    if [ -d "$TEST_DATA_DIR" ]; then
        ls -la "$TEST_DATA_DIR"
    else
        echo -e "${RED}Test data directory not found at: ${TEST_DATA_DIR}${NC}"
    fi
    
    # Prompt for alternative input file
    echo -e "${YELLOW}Would you like to specify a different input file? (y/n)${NC}"
    read -r response
    if [[ "$response" =~ ^[Yy] ]]; then
        echo -e "${YELLOW}Enter path to input file:${NC}"
        read -r INPUT_FILE
        if [ ! -f "$INPUT_FILE" ]; then
            echo -e "${RED}Error: Specified input file not found at: ${INPUT_FILE}${NC}"
            exit 1
        fi
    else
        exit 1
    fi
fi

# Run the executable
echo -e "${BLUE}Executing: ${EXECUTABLE} ${INPUT_FILE}${NC}"
echo -e "${YELLOW}----------------------------------------${NC}"

# Create a timestamp for the start time
START_TIME=$(date +%s.%N)

# Run the program and capture exit code
"$EXECUTABLE" "$INPUT_FILE"
EXIT_CODE=$?

# Calculate execution time
END_TIME=$(date +%s.%N)
EXECUTION_TIME=$(echo "$END_TIME - $START_TIME" | bc)

echo -e "${YELLOW}----------------------------------------${NC}"

# Check exit code
if [ $EXIT_CODE -eq 0 ]; then
    echo -e "${GREEN}Program executed successfully with exit code: ${EXIT_CODE}${NC}"
else
    echo -e "${RED}Program failed with exit code: ${EXIT_CODE}${NC}"
fi

# Print execution time
printf "${BLUE}Execution time: %.4f seconds${NC}\n" $EXECUTION_TIME

echo -e "${BLUE}======================================${NC}"
echo -e "${BLUE}   Execution complete   ${NC}"
echo -e "${BLUE}======================================${NC}"