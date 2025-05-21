#!/bin/bash

# Enhanced C++ run script with comprehensive error handling and mode selection
# ---------------------------------------------------------------------------

# Set script to exit on error
set -e

# Configuration variables
PROJECT_NAME="parser"
SRC_DIR="../src"
LIBRARY_DIR="../src/library"
BUILD_DIR="../build"
EXECUTABLE="${BUILD_DIR}/${PROJECT_NAME}"
TEST_DATA_DIR="../tests"
INPUT_FILE="${TEST_DATA_DIR}/libTest.txt"
MODE="all"  # Default mode

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

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --mode=*)
            MODE="${1#*=}"
            shift
            ;;
        --input=*)
            INPUT_FILE="${1#*=}"
            shift
            ;;
        --help)
            echo -e "Usage: ./run.sh [options]"
            echo -e "Options:"
            echo -e "  --mode=MODE      Set execution mode (lex, parse, interpret, all)"
            echo -e "  --input=FILE     Specify input file path"
            echo -e "  --help           Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            echo -e "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Validate selected mode
if [[ "$MODE" != "lex" && "$MODE" != "parse" && "$MODE" != "interpret" && "$MODE" != "all" ]]; then
    echo -e "${RED}Invalid mode: ${MODE}${NC}"
    echo -e "${YELLOW}Valid modes: lex, parse, interpret, all${NC}"
    exit 1
fi

# Check if input file exists
if [[ ! -f "$INPUT_FILE" ]]; then
    echo -e "${RED}Error: Input file not found at ${INPUT_FILE}${NC}"
    exit 1
fi

# Check if executable exists or needs to be rebuilt
if [[ ! -f "$EXECUTABLE" || $(find "$SRC_DIR" -name "*.cpp" -newer "$EXECUTABLE" 2>/dev/null | wc -l) -gt 0 || $(find "$LIBRARY_DIR" -name "*.cpp" -newer "$EXECUTABLE" 2>/dev/null | wc -l) -gt 0 ]]; then
    echo -e "${YELLOW}Building/rebuilding executable...${NC}"
    
    # Create build directory if it doesn't exist
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    # Clean up any old object files to prevent linking issues
    rm -f *.o
    
    # Collect source files with proper deduplication
    SRC_FILES=$(find "$SRC_DIR" -name "*.cpp" -not -path "$LIBRARY_DIR/*" | sort -u)
    LIB_FILES=$(find "$LIBRARY_DIR" -name "*.cpp" 2>/dev/null | sort -u)
    
    # Compile each source file to an object file
    echo -e "${BLUE}Compiling source files...${NC}"
    OBJECTS=""
    
    for src in $SRC_FILES $LIB_FILES; do
        obj=$(basename "$src" .cpp).o
        echo -e "${YELLOW}Compiling: $src${NC}"
        g++ -std=c++17 -Wall -Wextra -I"$SRC_DIR" -c "$src" -o "$obj"
        OBJECTS="$OBJECTS $obj"
    done
    
    # Link all object files
    echo -e "${BLUE}Linking: g++ $OBJECTS -o ${PROJECT_NAME}${NC}"
    g++ $OBJECTS -o "${PROJECT_NAME}"
    
    if [[ $? -eq 0 ]]; then
        echo -e "${GREEN}Compilation successful!${NC}"
    else
        echo -e "${RED}Compilation failed!${NC}"
        exit 1
    fi
    
    # Return to the original directory
    cd - > /dev/null
fi

# Print execution information
echo -e "${YELLOW}Execution mode: ${MODE}${NC}"
echo -e "${YELLOW}Input file: ${INPUT_FILE}${NC}"

# Run the executable with selected mode
echo -e "${BLUE}Executing: ${EXECUTABLE} ${INPUT_FILE} ${MODE}${NC}"
echo -e "${YELLOW}----------------------------------------${NC}"

# Create a timestamp for the start time
START_TIME=$(date +%s.%N)

# Run the program with the selected mode and capture exit code
"$EXECUTABLE" "$INPUT_FILE" "$MODE"
EXIT_CODE=$?

# Calculate execution time
END_TIME=$(date +%s.%N)
EXECUTION_TIME=$(echo "$END_TIME - $START_TIME" | bc)

# Check the exit code and display appropriate message
if [[ $EXIT_CODE -eq 0 ]]; then
    echo -e "${GREEN}----------------------------------------${NC}"
    echo -e "${GREEN}Program completed successfully in ${EXECUTION_TIME} seconds${NC}"
else
    echo -e "${RED}----------------------------------------${NC}"
    echo -e "${RED}Program failed with exit code: ${EXIT_CODE}${NC}"
fi

exit $EXIT_CODE