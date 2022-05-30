#pragma once

#include <string>
#include <memory>

enum Token {
	// basic
	token_eof = -1,

	// commands
	token_definition = -2,
	token_extern = -3,

	// primary
	token_identifier = -4,
	token_parenthesis = -5,
	token_comma = -6,
	token_semicolon = -7,
	token_bracelet = -8,

	// numbers
	token_integer = -9,
	token_double = -10,

	// operators
	token_assign_op = -11,
	token_arithmetic_op = -12,
	token_compare_op = -13
};

//template <typename T> 
//class TokenMetadata {
//protected:
//	T value;
//public:
//	virtual TokenMetadata(T metadata_value) = default;
//	virtual T get_value() {
//		return this->value;
//	}
//};
//
//class StringTokenMetadata : public TokenMetadata<std::string> {
//public:
//	StringTokenMetadata(std::string value) : TokenMetadata(value) {
//		
//	}
//};