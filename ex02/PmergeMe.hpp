#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <string>
#include <vector>

class PmergeMe {
  public:
    PmergeMe();
    ~PmergeMe();

    void parseArguments(int argc, char **argv);
    void run();

  private:
    std::vector<int> _input;

    static bool isPositiveIntegerToken(const std::string &s);
    static int parseIntToken(const std::string &s);

    static void printSequence(const std::string &label, const std::vector<int> &seq);

    static std::vector<size_t> buildJacobsthalOrderVector(size_t pairCount);
    static std::deque<size_t> buildJacobsthalOrderDeque(size_t pairCount);

    static void fordJohnsonSortVector(std::vector<int> &arr);
    static void fordJohnsonSortDeque(std::deque<int> &arr);
};

#endif
