#!/bin/bash

# Enhanced C++ run script with comprehensive error handling and mode selection
# ---------------------------------------------------------------------------

# Set script to exit on error
set -e

# Configuration variables
PROJECT_NAME="parser"
BUILD_DIR="../build"
EXECUTABLE="${BUILD_DIR}/${PROJECT_NAME}"
TEST_DATA_DIR="../tests"
INPUT_FILE="${TEST_DATA_DIR}/fixtures/comments.txt"
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

# Check if executable exists
# ... [rest of your existing executable checks]

# Check if input file exists
# ... [rest of your existing input file checks]

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

# ... [rest of your timing and completion code]