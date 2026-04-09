/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayoub <aayoub@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:21:56 by aayoub            #+#    #+#             */
/*   Updated: 2026/04/09 16:23:11 by aayoub           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <stdexcept>
#include <iomanip>

class BitcoinExchange {
    public:
        BitcoinExchange(const std::string& dataFile);
        ~BitcoinExchange();
    
        void processInput(const std::string& inputFile);
        void printBitcoinValue(const std::string& date, double amount) const;

    private:
        std::map<std::string, double> _data;
        
        void parseDataFile(const std::string& dataFile);
};

#endif