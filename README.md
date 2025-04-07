# Mini-Lang

**Mini-Lang** is a C++ project designed to tokenize and parse a given input string or file. Using the Parsed AST Tree, a interperter, follows pre determined syntax rules and interprets the language.

## Table of Contents

- [About](#about)
- [Features](#features)
- [Usage](#usage)
- [Current Status](#current-status)
- [License](#license)

## About

This project serves as a tokenizer, parser, and interperter, which processes a stream of text into discrete tokens. These tokens are then processed according to specific syntactic rules, useful for applications like compilers, interpreters, or custom data parsing tasks. The project is written in C++ and is still under development, with ongoing improvements and features being added.

## Features

- **Tokenizer**: Splits input text into tokens based on predefined patterns.
- **Parser**: Uses tokens to analyze and process the input based on syntactic rules.
- **Interpreter**: Uses AST Tree to "execute" the nodes.
- **Error Handling**: Reports errors when tokens do not match expected patterns.
- **Extensibility**: The design allows easy addition of new tokens and grammar rules.

## Usage

### Building and Running the Parser

To build and run the parser, follow these steps:

### **1. Navigate to the scripts directory:**
    '''bash
    cd scripts
### **3. Build the project:**
    ./build.sh
### **4. Run the parser with a test file (specified in run.sh):**
    ./run.sh
### **5. Output:
    ../output/ 
    compiled files will be called output_date_time.txt
### **6. Clean up build artifacts:**
    ./clean.sh

### **Note: To parse a different input file, you can modify run.sh script:**
    ../build/parser path/to/your/file.txt
## Current Status

- **Development Stage**: The project is still in progress.
- **What’s Done**: Tokenizer, Parser, and Interperter are built.
- **Next Steps**: Implementing Value class to handle different values. Implementing more coding aspects such as functions, if statements, and loops.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
