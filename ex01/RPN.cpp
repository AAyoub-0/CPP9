/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayoub <aayoub@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:14:45 by aayoub            #+#    #+#             */
/*   Updated: 2026/04/09 20:22:29 by aayoub           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

static bool isOperator(const std::string& token) {
    return token == PLUS || token == MINUS || token == MULTIPLY || token == DIVIDE;
}

int RPN::evaluate(const std::string& expression) {
    std::stack<int> stack;
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        if (std::isdigit(token[0])) {
            int value;
            std::istringstream(token) >> value;
            if (value < 0 || value > 9) {
                throw std::runtime_error("Numbers must be between 0 and 9");
            }
            stack.push(value);
        } else if (isOperator(token)) {
            if (stack.size() < 2) {
                throw std::runtime_error("Invalid expression: not enough operands");
            }
            int b = stack.top();
            stack.pop();
            int a = stack.top();
            stack.pop();
            int result;

            if (token == PLUS) result = a + b;
            else if (token == MINUS) result = a - b;
            else if (token == MULTIPLY) result = a * b;
            else if (token == DIVIDE) {
                if (b == 0) {
                    throw std::runtime_error("Division by zero");
                }
                result = a / b;
            }
            stack.push(result);
        } else {
            throw std::runtime_error("Invalid token: " + token);
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid expression: too many operands");
    }
    return stack.top();
}
