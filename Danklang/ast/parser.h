#pragma once

#include <iostream>

#include "expression_nodes.h"

int get_token_precedence(std::pair<Token, std::string> token);
std::pair<Token, std::string> get_next_token(std::ifstream& input_stream);
std::unique_ptr<ExprAST> parse_binary_operator_rhs(std::ifstream& input_stream, int expression_precedency, std::unique_ptr<ExprAST> lhs);
std::unique_ptr<ExprAST> parse_double_expr(std::ifstream& input_stream);
std::unique_ptr<ExprAST> pasre_parenthesis_expr(std::ifstream& input_stream);
std::unique_ptr<ExprAST> parse_identifier_expr(std::ifstream& input_stream);
std::unique_ptr<ExprAST> parse_primary(std::ifstream& input_stream);
std::unique_ptr<ExprAST> parse_expression(std::ifstream& input_stream);
std::unique_ptr<FunctionPrototypeAST> parse_function_prototype(std::ifstream& input_stream);
std::unique_ptr<FunctionAST> parse_function_definition(std::ifstream& input_stream);
