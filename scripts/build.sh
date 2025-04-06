#!/bin/bash

# Enhanced C++ build script with robust error handling and clear feedback
# -----------------------------------------------------------------------

# Set script to exit on error
set -e

# Configuration variables
PROJECT_NAME="parser"
SOURCE_DIR="../src"
OUTPUT_DIR="../build"
LOG_FILE="build.log"
CXX_COMPILER="g++"
CXX_FLAGS="-g -Wall -Wextra -std=c++11"

# Create color codes for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print banner
echo -e "${BLUE}======================================${NC}"
echo -e "${BLUE}   Building ${PROJECT_NAME}   ${NC}"
echo -e "${BLUE}======================================${NC}"

# Create output directory if it doesn't exist
if [ ! -d "$OUTPUT_DIR" ]; then
    echo -e "${YELLOW}Creating output directory: ${OUTPUT_DIR}${NC}"
    mkdir -p "$OUTPUT_DIR"
fi

# Check if source directory exists
if [ ! -d "$SOURCE_DIR" ]; then
    echo -e "${RED}Error: Source directory not found: ${SOURCE_DIR}${NC}"
    exit 1
fi

# No include directory check needed

# Identify source files
SOURCE_FILES=("$SOURCE_DIR"/*.cpp)
if [ ${#SOURCE_FILES[@]} -eq 0 ]; then
    echo -e "${RED}Error: No source files found in ${SOURCE_DIR}${NC}"
    exit 1
fi

# Count source files for feedback
NUM_SOURCE_FILES=$(ls -1 "$SOURCE_DIR"/*.cpp 2>/dev/null | wc -l)
echo -e "${BLUE}Found ${NUM_SOURCE_FILES} source files${NC}"

# Clear previous log file
> "$LOG_FILE"

# Start build process
echo -e "${YELLOW}Starting compilation...${NC}"
echo "Compilation started at $(date)" >> "$LOG_FILE"

# Build command
OUTPUT_PATH="$OUTPUT_DIR/$PROJECT_NAME"
BUILD_CMD="$CXX_COMPILER $CXX_FLAGS $SOURCE_DIR/*.cpp -o $OUTPUT_PATH"

# Echo the command for debugging purposes
echo -e "${BLUE}Executing: ${BUILD_CMD}${NC}"

# Execute the build and capture output
if $BUILD_CMD 2>> "$LOG_FILE"; then
    echo -e "${GREEN}Compilation successful!${NC}"
else
    RETURN_CODE=$?
    echo -e "${RED}Compilation failed with error code: ${RETURN_CODE}${NC}"
    echo -e "${RED}Last 10 lines of error log:${NC}"
    tail -n 10 "$LOG_FILE"
    echo -e "${YELLOW}See ${LOG_FILE} for complete error information${NC}"
    exit $RETURN_CODE
fi

# Check if the executable was actually created
if [ -f "$OUTPUT_PATH" ]; then
    # Make sure it's executable
    chmod +x "$OUTPUT_PATH"
    
    # Display file information
    FILE_SIZE=$(du -h "$OUTPUT_PATH" | cut -f1)
    echo -e "${GREEN}Build complete!${NC}"
    echo -e "${GREEN}Executable: ${OUTPUT_PATH}${NC}"
    echo -e "${GREEN}Size: ${FILE_SIZE}${NC}"
    
    # Display run command example
    echo -e "${YELLOW}To run the program:${NC}"
    echo -e "${BLUE}$OUTPUT_PATH <input_file>${NC}"
else
    echo -e "${RED}Error: Executable was not created at: ${OUTPUT_PATH}${NC}"
    exit 1
fi

echo "Test Log Entry $(date)" >> build.log


echo -e "${BLUE}======================================${NC}"
echo -e "${GREEN}Build process completed successfully${NC}"
echo -e "${BLUE}======================================${NC}"