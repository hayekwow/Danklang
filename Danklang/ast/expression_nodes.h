#pragma once

#include <string>
#include <memory>
#include <vector>

class ExprAST {
public:
	virtual ~ExprAST() {}
	virtual llvm::Value* codegen();
};


// DoubleExprAST - expression for dobule precision literals (ex.: "21.37")
// TODO: add ints, floats, shorts etc.
class DoubleExprAST : public ExprAST {
private:
	double value;
public:
	DoubleExprAST(double value) : value(value) {}
};


// VariableExprAST - expression for referencing a variable
class VariableExprAST : public ExprAST {
private:
	std::string name;
public:
	VariableExprAST(const std::string& name) : name(name) {}
};


// BinaryExprAST - expression for binary operators
class BinaryExprAST : public ExprAST {
private:
	char op_code;
	std::unique_ptr<ExprAST> left_arg, right_arg;
public:
		BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
			: op_code(op), left_arg(std::move(lhs)), right_arg(std::move(rhs)) {}
};


// CallExprAST - expression for function calls
class CallExprAST : public ExprAST {
private:
	std::string callee;
	std::vector<std::unique_ptr<ExprAST>> args;
public:
		CallExprAST(const std::string& callee, std::vector<std::unique_ptr<ExprAST>> args)
			: callee(callee), args(std::move(args)) {}
};


// FunctionPrototypeAST - a function declaration / prototype for a function (name, argument names)
class FunctionPrototypeAST {
private:
	std::string name;
	std::vector<std::string> args;
public:
	FunctionPrototypeAST(const std::string& name, std::vector<std::string> args)
			: name(name), args(std::move(args)) {}
		
		const std::string& getName() const { return name; }
};

// FunctionAST - a function definition
class FunctionAST {
private:
	std::unique_ptr<FunctionPrototypeAST> prototype;
	std::unique_ptr<ExprAST> body;
public:
	FunctionAST(std::unique_ptr<FunctionPrototypeAST> proto, std::unique_ptr<ExprAST> body)
			: prototype(std::move(proto)), body(std::move(body)) {}
};