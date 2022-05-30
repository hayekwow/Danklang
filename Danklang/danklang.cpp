#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <string>

#include "danklang.h"
#include "lexer/tokens/token_parsing.h"

// Arguments: 
//      -i <input file> (required)
//      -o <output file> (required)
//      -h <print help>
std::map<std::string, std::string> parse_args(int argc, char** argv) {
	std::map<std::string, std::string> args;
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg.find("-") == 0) {
			if (i + 1 < argc) {
				args[arg] = argv[i + 1];
				i++;
			}
			else {
				args[arg] = "";
			}
		}
	}
	return args;
}

std::ifstream open_source_code(std::string input_file_directory) {
	std::ifstream source_code_file; 
	source_code_file.open(input_file_directory);
	if (!source_code_file.is_open()) {
		std::cerr << "Could not open source code file: " << input_file_directory << std::endl;
		exit(1);
	}
	return source_code_file;
}
std::ofstream open_ir_code(std::string ir_file_directory) {
	std::filesystem::create_directory("temp");

	std::ofstream ir_code_file(ir_file_directory);
	if (!ir_code_file.is_open()) {
		std::cerr << "Could not create IR code file: " << ir_file_directory << std::endl;
		exit(1);
	}
	return ir_code_file;
}

void sanitize_parameters(std::map<std::string, std::string>& arguments)
{
	if (arguments.find("-i") == arguments.end()) {
		std::cerr << "No input file specified" << std::endl;
		exit(1);
	}
	if (arguments.find("-o") == arguments.end()) {
		std::cerr << "No output file specified" << std::endl;
		exit(1);
	}
}
void print_help()
{
	std::cout << "Danklang compiler v0.1" << std::endl << std::endl;
	std::cout << "Usage: \n\tdanklang -i <input file> -o <output file>" << std::endl << std::endl;
	std::cout << "Required Parameters: " << std::endl;
	std::cout << "\t-i <input main file> (required)" << std::endl;
	std::cout << "\t-o <output .exe file> (required)" << std::endl;
	std::cout << "\nOptional Parameters:" << std::endl;
	std::cout << "\t-h <print help>" << std::endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	// compiler initialization
	std::map<std::string, std::string> arguments = parse_args(argc, argv);
	
	if (arguments.count("-h") != 0) {
		print_help();
		return 0;
	}
	sanitize_parameters(arguments);

	// initializing file streams for input and output files
	std::ifstream input_stream = open_source_code(arguments["-i"]);
	std::ofstream output_stream = open_ir_code("temp/temp.o");

	// turn raw text into tokens (tokenization)
	// NOTE: debug code
	while (!input_stream.eof()) {
		auto token_data = get_token(input_stream);
		std::cout << token_data.first << " " << token_data.second << std::endl;
	}

	// turn tokens into abstract syntax tree (AST)
	// turn AST into intermediate representation (IR)

	// writing intermediate representation to the temporary file /temp directory in -o directory
	// invoking llvm compiler on the temporary file
	// writing the output of the llvm compiler to the output file in -o directory

	// closing all file streams
	input_stream.close();
	output_stream.close();
}
