#pragma once

#include <map>
#include <string>
#include <fstream>

#include "../lexer/tokens/token_metadata.h"
#include "../lexer/tokens/token_parsing.h"
#include "expression_nodes.h"
#include "parser_errors.h"
#include "parser.h"

static std::map<char, int> binary_operator_precedence = {
	{ '<', 10 },  { '+', 20 }, { '-', 20 }, { '*', 40 }, { '/', 40 }, { '^', 50 }
};
int get_token_precedence(std::pair<Token, std::string> token) {
	if (!(token.first == Token::token_arithmetic_op))
		return -1;

	int precedence = binary_operator_precedence[token.second[0]];
	if (precedence <= 0)
		return -1;
	return precedence;
}

static std::pair<Token, std::string> current_token;
std::pair<Token, std::string> get_next_token(std::ifstream& input_stream) {
	return current_token = get_token(input_stream);
}


std::unique_ptr<ExprAST> parse_binary_operator_rhs(std::ifstream& input_stream, int expression_precedency, std::unique_ptr<ExprAST> lhs) {
	while (true) {
		int token_precedency = get_token_precedence(current_token);

		if (token_precedency < expression_precedency)
			return lhs;

		// get binary operator
		char binary_operator = current_token.second[0];
		get_next_token(input_stream);

		std::unique_ptr<ExprAST> rhs = parse_primary(input_stream);
		if (!rhs)
			return nullptr;

		int next_precedency = get_token_precedence(current_token);

		// if binary_operator binds less tightly with RHS than the operator after RHS, let
		// the pending operator take RHS as its LHS.
		if (token_precedency < next_precedency) {
			rhs = parse_binary_operator_rhs(input_stream, token_precedency + 1, std::move(rhs));
			if (!rhs) {
				return nullptr;
			}
		}

		// merge lhs with rhs
		lhs = std::make_unique<BinaryExprAST>(binary_operator, std::move(lhs), std::move(rhs));
	}
}

std::unique_ptr<ExprAST> parse_double_expr(std::ifstream& input_stream) {
	auto result = std::make_unique<DoubleExprAST>(std::stod(current_token.second));
	get_next_token(input_stream);
	return std::move(result);
}

std::unique_ptr<ExprAST> pasre_parenthesis_expr(std::ifstream& input_stream) {
	get_next_token(input_stream);	// eat '('
	auto result = parse_expression(input_stream);
	if (!result) {
		return nullptr;
	}
	if (current_token.first != Token::token_parenthesis) {
		return log_error("expected ')'");
	}
	get_next_token(input_stream);	// eat ')'
	return std::move(result);
}

std::unique_ptr<ExprAST> parse_identifier_expr(std::ifstream& input_stream) {
	std::string identifier_name = current_token.second;
	get_next_token(input_stream); // skip identifier name

	// check for simple variable reference
	if(current_token.first == Token::token_parenthesis && current_token.second != "(") {
		return std::make_unique<VariableExprAST>(identifier_name);
	}

	// check for function call
	get_next_token(input_stream); // skip '('
	std::vector<std::unique_ptr<ExprAST>> args;

	if (current_token.first != token_parenthesis && current_token.second != ")") {
		while (true) {
			if (auto argument = parse_expression(input_stream))
				args.push_back(std::move(argument));
			else
				return nullptr;

			if (current_token.first == token_parenthesis && current_token.second == ")")
				break;
			if (current_token.first != Token::token_comma)
				return log_error("expected ')' or ',' in argument list");
			get_next_token(input_stream);
		}
	}

	get_next_token(input_stream); // skip ')'
	return std::make_unique<CallExprAST>(identifier_name, std::move(args));
}

std::unique_ptr<ExprAST> parse_primary(std::ifstream& input_stream) {
	switch (current_token.first) {
	case Token::token_identifier:
		return parse_identifier_expr(input_stream);
	case Token::token_double:
		return parse_double_expr(input_stream);
	case Token::token_parenthesis:
		if (current_token.second == "(")
			return pasre_parenthesis_expr(input_stream);
	default:
		return log_error("unknown token when expecting an expression");
	}
}

std::unique_ptr<ExprAST> parse_expression(std::ifstream& input_stream) {
	auto lhs = parse_primary(input_stream);
	if (!lhs)
		return nullptr;
	return parse_binary_operator_rhs(input_stream, 0, std::move(lhs));
}

std::unique_ptr<FunctionPrototypeAST> parse_function_prototype(std::ifstream& input_stream) {
	if(current_token.first != Token::token_identifier)
		return log_error_prototype("expected function name in prototype");

	// get function name and move to the next token
	std::string function_name = current_token.second;
	get_next_token(input_stream);

	// check for argument list
	if (current_token.first != Token::token_parenthesis || current_token.second != "(")
		return log_error_prototype("expected '(' in prototype");
	
	std::vector<std::string> argument_names;
	while (get_next_token(input_stream).first == Token::token_identifier)
		argument_names.push_back(current_token.second);
	if(current_token.first != Token::token_parenthesis || current_token.second != ")")
		return log_error_prototype("expected ')' in prototype");
	
	return std::make_unique<FunctionPrototypeAST>(function_name, std::move(argument_names));
}

std::unique_ptr<FunctionPrototypeAST> parse_extern_function_prototype(std::ifstream& input_stream) {
	// eat 'extern' keyword
	get_next_token(input_stream);
	return parse_function_prototype(input_stream);
}

std::unique_ptr<FunctionAST> parse_function_definition(std::ifstream& input_stream) {
	// eat 'function' keyword
	get_next_token(input_stream);

	// get function prototype
	auto prototype = parse_function_prototype(input_stream);
	if (!prototype) {
		return nullptr;
	}

	if(auto body = parse_expression(input_stream)) {
		return std::make_unique<FunctionAST>(std::move(prototype), std::move(body));
	}
	return nullptr;
}