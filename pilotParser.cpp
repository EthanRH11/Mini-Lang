#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include"lexer.hpp"

int main(int argc, char* argv[]) {
    if(argc > 2){
        std::cerr << "Error: not enough file arguments. <sourceFile>.\n";
        exit(1);
    } 
    //Indicator
    std::cout << "Reading from the file: " << argv[1] << std::endl;

    std::ifstream sourceFileStream(argv[1]);

    std::stringstream buffer;
    char temp;

    while(sourceFileStream.get(temp)){
        buffer << temp;
    }
    std::string sourceCode = buffer.str();
    std::cout << "This is the source code: " << std::endl << sourceCode;
    Lexer lexer(sourceCode);
    std::vector<Token*> tokens = lexer.tokenize();\

    

    std::cout << std::endl << "This is the end of the program: " << std::endl << sourceCode;
    
}