#pragma once

#include <iostream>
#include <map>

std::map<std::string, std::string> parse_args(int argc, char** argv);
std::ifstream open_source_code(std::string input_file_directory);
std::ofstream open_ir_code(std::string ir_file_directory);
void sanitize_parameters(std::map<std::string, std::string>& arguments);

void print_help();
