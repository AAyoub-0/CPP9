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

#include <cctype>

BitcoinExchange::BitcoinExchange(const std::string& dataFile) {
    parseDataFile(dataFile);
}

BitcoinExchange::~BitcoinExchange() {}

static void printError(const std::string& message, const std::string& line) {
    std::cerr << "Error: " << message;
    if (!line.empty()) {
        std::cerr << " => " << line;
    }
    std::cerr << std::endl;
}

static std::string trim(const std::string& s) {
    std::string::size_type start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    std::string::size_type end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(start, end - start);
}

static bool isAllDigits(const std::string& s) {
    if (s.empty()) {
        return false;
    }
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
            return false;
        }
    }
    return true;
}

static bool isLeapYear(int year) {
    if (year % 400 == 0) {
        return true;
    }
    if (year % 100 == 0) {
        return false;
    }
    return year % 4 == 0;
}

static bool isValidDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }

    const std::string yearStr = date.substr(0, 4);
    const std::string monthStr = date.substr(5, 2);
    const std::string dayStr = date.substr(8, 2);
    if (!isAllDigits(yearStr) || !isAllDigits(monthStr) || !isAllDigits(dayStr)) {
        return false;
    }

    std::istringstream yearIss(yearStr);
    std::istringstream monthIss(monthStr);
    std::istringstream dayIss(dayStr);
    int year = 0;
    int month = 0;
    int day = 0;
    if (!(yearIss >> year) || !(monthIss >> month) || !(dayIss >> day)) {
        return false;
    }
    if (month < 1 || month > 12) {
        return false;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        daysInMonth[1] = 29;
    }
    if (day < 1 || day > daysInMonth[month - 1]) {
        return false;
    }

    return true;
}

static bool parseAmount(const std::string& valueStr, double& amount) {
    std::istringstream valueIss(valueStr);
    char extra = '\0';
    if (!(valueIss >> amount)) {
        return false;
    }
    if (valueIss >> extra) {
        return false;
    }
    return true;
}

void BitcoinExchange::processInput(const std::string& inputFile) {
    std::ifstream infile(inputFile.c_str());
    if (!infile.is_open()) {
        throw std::runtime_error("could not open file.");
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(infile, line)) {
        if (line.empty()) {
            continue;
        }

        if (firstLine && trim(line) == "date | value") {
            firstLine = false;
            continue;
        }
        firstLine = false;

        const std::string::size_type sep = line.find('|');
        if (sep == std::string::npos || line.find('|', sep + 1) != std::string::npos) {
            printError("bad input", line);
            continue;
        }

        std::string date = trim(line.substr(0, sep));
        std::string valueStr = trim(line.substr(sep + 1));
        double amount = 0.0;

        if (!isValidDate(date)) {
            printError("bad input", line);
            continue;
        }
        if (!parseAmount(valueStr, amount)) {
            printError("bad input", line);
            continue;
        }

        if (amount < 0) {
            printError("not a positive number.", "");
            continue;
        }
        if (amount > 1000) {
            printError("too large a number.", "");
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
            printError("bad input", date);
            return;
        }
    }
    double value = it->second * amount;
    std::cout << date << " => " << amount << " = " << std::setprecision(10) << value << std::endl;
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
