#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <memory>
#include <map>

#include "token_metadata.h"
#include "token_parsing.h"


static const std::map<std::string, Token> token_keywords = {
	{ "def", Token::token_definition },
	{ "extern", Token::token_extern }
};

std::pair<Token, std::string> get_token(std::ifstream& input_stream) {
	static char last_char = ' ';
	
	// skip all empty spaces and comments
	while (isspace(last_char)) {
		last_char = input_stream.get();
	}

	if (isalpha(last_char)) {	// identifier or keyword
		// reading all of identifier
		std::string identifier = std::string(1, last_char);
		while (isalnum(last_char = input_stream.get()))
			identifier += last_char;
		
		// check for known keywords
		for (std::pair<std::string, Token> token_match : token_keywords) {
			if (token_match.first == identifier) {
				return std::make_pair(token_match.second, identifier);
			}
		}
		
		// no matches, return 
		return std::make_pair(Token::token_identifier, identifier);
	}
	else if (isdigit(last_char) || last_char == '.') {	// number or dot in decimal/floating numbers
		// Reading number data
		// NOTE: possible error not handled for input such as: 12.23.33
		std::string number_str;
		do {
			number_str += last_char;
			last_char = input_stream.get();
		} while (isdigit(last_char) || last_char == '.');

		// TODO: Determining number type (integer, float, double)

		return std::make_pair(Token::token_double, number_str);
	}
	else if (last_char == '#') {	// single line comment skipping
		// skipping whole new line 
		do
			last_char = input_stream.get();
		while (last_char != EOF || last_char != '\n' || last_char != '\r');

		// continue parsing unless eof
		if (last_char != EOF)
			return get_token(input_stream);
	} else {	// special character
		// possible end of file
		if (last_char == EOF)
			return std::make_pair(Token::token_eof, nullptr);

		// possible operator
		char this_char = last_char;
		last_char = input_stream.get();
		switch (this_char) {
		case '=':
			return std::make_pair(Token::token_assign_op, std::string(1, this_char));		// assignment operator
		case '+':
		case '-':
		case '*':
		case '/':
			return std::make_pair(Token::token_arithmetic_op, std::string(1, this_char));	// arithmetic operator
		case '<':
		case '>':
			return std::make_pair(Token::token_compare_op, std::string(1, this_char));		// compare operator
		case '(':
		case ')':
			return std::make_pair(Token::token_parenthesis, std::string(1, this_char));		// parenthesis
		case ',':
			return std::make_pair(Token::token_comma, std::string(1, this_char));			// comma
		}
	}
}