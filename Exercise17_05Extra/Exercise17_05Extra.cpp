/**
* Class: CSCI2490 C++ Programming
* Instructor: Y. Daniel Liang
* Description: Calculates an expression entered by the user
* Due: 11/14/2016
* @author Shaun C. Dobbs
* @version 1.0

* I pledge by honor that I have completed the programming assignment independently.
* I have not copied the code from a student or any source.
* I have not given my code to any student.

Sign here: Shaun C. Dobbs
*/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>



#ifndef IMPROVEDSTACK_H
#define IMPROVEDSTACK_H

template<typename T>
class Stack
{
public:
	Stack();
	Stack(const Stack&);
	~Stack();
	bool empty() const;
	T peek() const;
	void push(T value);
	T pop();
	int getSize() const;

private:
	T* elements;
	int size;
	int capacity;
	void ensureCapacity();
};

template<typename T>
Stack<T>::Stack() : size(0), capacity(16)
{
	elements = new T[capacity];
}

template<typename T>
Stack<T>::Stack(const Stack& stack)
{
	elements = new T[stack.capacity];
	size = stack.size;
	capacity = stack.capacity;
	for (int i = 0; i < size; i++)
	{
		elements[i] = stack.elements[i];
	}
}

template<typename T>
Stack<T>::~Stack()
{
	delete[] elements;
}

template<typename T>
bool Stack<T>::empty() const
{
	return (size == 0);
}

template<typename T>
T Stack<T>::peek() const
{
	return elements[size - 1];
}

template<typename T>
void Stack<T>::push(T value)
{
	ensureCapacity();
	elements[size++] = value;
}

template<typename T>
void Stack<T>::ensureCapacity()
{
	if (size >= capacity)
	{
		T* old = elements;
		capacity = 2 * size;
		elements = new T[size * 2];

		for (int i = 0; i < size; i++)
			elements[i] = old[i];

		delete[] old;
	}
}

template<typename T>
T Stack<T>::pop()
{
	return elements[--size];
}

template<typename T>
int Stack<T>::getSize() const
{
	return size;
}

#endif
using namespace std;

// Split an expression into numbers, operators, and parenthese
vector<string> split(const string& expression);

// Evaluate an expression and return the result
double evaluateExpression(const string& expression);

// Perform an operation
void processAnOperator(
	Stack<double>& operandStack, Stack<char>& operatorStack);

int main()
{
	string input;
	cout << "Enter an expression: ";
	getline(cin, input);

	cout << input << " = "
		<< evaluateExpression(input) << endl;

	return 0;
}

//Split from previous exercise in Chapter 12
vector<string> split(const string& expression)
{
	vector<string> v; 
	string numberString; 

	for (unsigned int i = 0; i < expression.length(); i++)
	{
		if (isdigit(expression[i]) || expression[i] == '.') {
			numberString.append(1, expression[i]); // Append a digit
			cout << numberString << endl;
		}
		else
		{
			if (numberString.size() > 0)
			{
				v.push_back(numberString); // Store the numeric string
				numberString.erase(); // Empty the numeric string
			}

			if (!isspace(expression[i]))
			{
				string s;
				s.append(1, expression[i]);
				v.push_back(s); // Store an operator and parenthesis
			}
		}
	}

	// Store the last numeric string
	if (numberString.size() > 0)
		v.push_back(numberString);

	return v;
}

// Evaluate an expression 
double evaluateExpression(const string& expression)
{
	// Create operandStack to store operands
	Stack<double> operandStack;

	// Create operatorStack to store operators
	Stack<char> operatorStack;

	// Extract operands and operators
	vector<string> tokens = split(expression);


	// Phase 1: Scan tokens
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i][0] == '+' || tokens[i][0] == '-')
		{
			// Process all +, -, *, / in the top of the operator stack
			while (!operatorStack.empty() && (operatorStack.peek() == '+'
				|| operatorStack.peek() == '-' || operatorStack.peek() == '*'
				|| operatorStack.peek() == '/'))
			{
				processAnOperator(operandStack, operatorStack);
			}

			// Push the + or - operator into the operator stack
			operatorStack.push(tokens[i][0]);
		}
		else if (tokens[i][0] == '*' || tokens[i][0] == '/')
		{
			// Process all *, / in the top of the operator stack
			while (!operatorStack.empty() && (operatorStack.peek() == '*'
				|| operatorStack.peek() == '/'))
			{
				processAnOperator(operandStack, operatorStack);
			}

			// Push the * or / operator into the operator stack
			operatorStack.push(tokens[i][0]);
		}
		else if (tokens[i][0] == '(')
		{
			operatorStack.push('('); // Push '(' to stack
		}
		else if (tokens[i][0] == ')')
		{
			// Process all the operators in the stack until seeing '('
			while (operatorStack.peek() != '(')
			{
				processAnOperator(operandStack, operatorStack);
			}

			operatorStack.pop(); // Pop the '(' symbol from the stack
		}
		else
		{ // An operand scanned. Push an operand to the stack as integer
			operandStack.push(stod(tokens[i].c_str()));
		}
	}

	// Phase 2: process all the remaining operators in the stack
	while (!operatorStack.empty())
	{
		processAnOperator(operandStack, operatorStack);
	}

	// Return the result
	return operandStack.pop();
}

// Process one opeator: Take an operator from operatorStack and 
// apply it on the operands in the operandStack 
void processAnOperator(
	Stack<double>& operandStack, Stack<char>& operatorStack)
{
	char op = operatorStack.pop();
	double op1 = operandStack.pop();
	double op2 = operandStack.pop();
	if (op == '+')
		operandStack.push(op2 + op1);
	else if (op == '-')
		operandStack.push(op2 - op1);
	else if (op == '*')
		operandStack.push(op2 * op1);
	else if (op == '/')
		operandStack.push(op2 / op1);
}