#pragma once

#include <iostream>
#include "expression_nodes.h"

std::unique_ptr<ExprAST> log_error(const char* message);
std::unique_ptr<FunctionPrototypeAST> log_error_prototype(const char* message);
