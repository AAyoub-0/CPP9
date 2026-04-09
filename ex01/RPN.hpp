/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayoub <aayoub@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:13:47 by aayoub            #+#    #+#             */
/*   Updated: 2026/04/09 20:16:38 by aayoub           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <sstream>
#include <stdexcept>

#define PLUS "+"
#define MINUS "-"
#define MULTIPLY "*"
#define DIVIDE "/"

class RPN {
    public:
        RPN() {}
        ~RPN() {}

        int evaluate(const std::string& expression);
};

#endif // RPN_HPP