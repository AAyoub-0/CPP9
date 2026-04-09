/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayoub <aayoub@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:23:30 by aayoub            #+#    #+#             */
/*   Updated: 2026/04/09 19:48:06 by aayoub           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(const std::string& dataFile) {
    parseDataFile(dataFile);
}

BitcoinExchange::~BitcoinExchange() {}

static void printError(const std::string& message, const std::string& line) {
    // Print error message to stderr with line content in red
    std::cerr << "\033[31m" << "[ERROR] " << message << ": " << line << "\033[0m" << std::endl;
}

static bool isValidDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }
    std::istringstream iss(date.substr(0, 4));
    int year, month, day;
    if (!(iss >> year) || year < 2009 || year > 2026)
        return false;
    iss.clear();
    iss.str(date.substr(5, 2));
    if (!(iss >> month) || month < 1 || month > 12)
        return false;
    iss.clear();
    iss.str(date.substr(8, 2));
    if (!(iss >> day) || day < 1 || day > 31)
        return false;
    return true;
}

void BitcoinExchange::processInput(const std::string& inputFile) {
    std::ifstream infile(inputFile.c_str());
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open input file: " + inputFile);
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(infile, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        std::istringstream iss(line);
        std::string date, pipe;
        double amount;
        if (!(iss >> date >> pipe >> amount) || pipe != "|") {
            printError("Invalid input format", line);
            continue;
        }
        
        if (amount < 0) {
            printError("Negative amount", line);
            continue;
        }
        else if (amount > 1000) {
            printError("Amount exceeds maximum limit", line);
            continue;
        }
        if (!isValidDate(date)) {
            printError("Invalid date", line);
            continue;
        }
        
        printBitcoinValue(date, amount);
    }
}

void BitcoinExchange::printBitcoinValue(const std::string& date, double amount) const {
    std::map<std::string, double>::const_iterator it = _data.lower_bound(date);
    if (it == _data.end() || it->first != date) {
        if (it != _data.begin())
            --it;
        else {
            printError("No data available for date", date);
            return;
        }
    }
    double value = it->second * amount;
    std::cout << date << " => " << amount << " = " << std::fixed << std::setprecision(2) << value << std::endl;
}

void BitcoinExchange::parseDataFile(const std::string& dataFile) {
    std::ifstream infile(dataFile.c_str());
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open data file: " + dataFile);
    }
    
    std::string line;
    bool firstLine = true;
    while (std::getline(infile, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        std::istringstream iss(line);
        std::string date, valueStr;
        if (!std::getline(iss, date, ',') || !std::getline(iss, valueStr)) {
            continue;
        }

        date.erase(date.find_last_not_of(" \t") + 1);
        valueStr.erase(0, valueStr.find_first_not_of(" \t"));

        double value;
        std::istringstream(valueStr) >> value;

        _data[date] = value;
    }
}
