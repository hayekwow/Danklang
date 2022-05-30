#include <memory>

#include "expression_nodes.h"
#include "parser_errors.h"

std::unique_ptr<ExprAST> log_error(const char* message) {
	fprintf(stderr, "Error: %s\n", message);
}
std::unique_ptr<FunctionPrototypeAST> log_error_prototype(const char* message) {
	log_error(message);
}