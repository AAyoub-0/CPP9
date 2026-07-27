#include "PmergeMe.hpp"

#include <iostream>

int main(int argc, char **argv) {
    try {
        PmergeMe app;
        app.parseArguments(argc, argv);
        app.run();
    } catch (const std::exception &) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    return 0;
}
