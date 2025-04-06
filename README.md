# Parser-Tokenizer

**Parser-Tokenizer** is a C++ project designed to tokenize and parse a given input string or file. The project aims to break down text into meaningful components (tokens) and process them according to specific syntactical rules. It is a work in progress and currently under development.

## Table of Contents

- [About](#about)
- [Features](#features)
- [Usage](#usage)
- [Current Status](#current-status)
- [License](#license)

## About

This project serves as a parser and tokenizer, which processes a stream of text into discrete tokens. These tokens are then processed according to specific syntactic rules, useful for applications like compilers, interpreters, or custom data parsing tasks. The project is written in C++ and is still under development, with ongoing improvements and features being added.

## Features

- **Tokenizer**: Splits input text into tokens based on predefined patterns.
- **Parser**: Uses tokens to analyze and process the input based on syntactic rules.
- **Error Handling**: Reports errors when tokens do not match expected patterns.
- **Extensibility**: The design allows easy addition of new tokens and grammar rules.

## Usage

### Building and Running the Parser

To build and run the parser, follow these steps:

1. Navigate to the scripts directory:
    '''bash
    cd scripts
2. Build the project:
    ./build.sh
3. Run the parser with a test file (specified in run.sh)
    ./run.sh
4. Clean up build artifacts.
    ./clean.sh

Note: To parse a different input file, you can modify run.sh script
    ../build/parser path/to/your/file.txt
## Current Status

- **Development Stage**: The project is still in progress.
- **What’s Done**: Initial tokenizer and parser components are being developed.
- **Next Steps**: Expanding error handling, refining the parsing logic, and preparing for integration with real-world input examples.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
