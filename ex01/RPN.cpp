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

#include <cctype>

static bool isOperator(const std::string& token) {
    return token == PLUS || token == MINUS || token == MULTIPLY || token == DIVIDE;
}

static bool isSingleDigit(const std::string& token) {
    return token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0])) != 0;
}

int RPN::evaluate(const std::string& expression) {
    std::stack<int> stack;
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        if (isSingleDigit(token)) {
            stack.push(token[0] - '0');
        } else if (isOperator(token)) {
            if (stack.size() < 2) {
                throw std::runtime_error("Error");
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
                    throw std::runtime_error("Error");
                }
                result = a / b;
            }
            stack.push(result);
        } else {
            throw std::runtime_error("Error");
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Error");
    }
    return stack.top();
}
