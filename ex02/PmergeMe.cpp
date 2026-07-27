#include "PmergeMe.hpp"

#include <climits>
#include <cstddef>
#include <cctype>
#include <ctime>
#include <deque>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

struct PairItemV {
    int small;
    int large;
    int id;
};

struct PairItemD {
    int small;
    int large;
    int id;
};

struct NodeV {
    int value;
    int id;
    bool isLarge;
};

struct NodeD {
    int value;
    int id;
    bool isLarge;
};

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

bool PmergeMe::isPositiveIntegerToken(const std::string &s) {
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

int PmergeMe::parseIntToken(const std::string &s) {
    std::istringstream iss(s);
    long long value = 0;
    char extra = '\0';
    if (!(iss >> value) || (iss >> extra) || value <= 0 || value > INT_MAX) {
        throw std::runtime_error("invalid number");
    }
    return static_cast<int>(value);
}

void PmergeMe::parseArguments(int argc, char **argv) {
    if (argc < 2) {
        throw std::runtime_error("missing input");
    }

    _input.clear();
    _input.reserve(static_cast<size_t>(argc - 1));

    for (int i = 1; i < argc; ++i) {
        std::string token(argv[i]);
        if (!isPositiveIntegerToken(token)) {
            throw std::runtime_error("invalid token");
        }
        _input.push_back(parseIntToken(token));
    }
}

void PmergeMe::printSequence(const std::string &label, const std::vector<int> &seq) {
    std::cout << label;
    for (std::vector<int>::size_type i = 0; i < seq.size(); ++i) {
        std::cout << seq[i];
        if (i + 1 < seq.size()) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

static std::vector<NodeV>::size_type lowerBoundNodesVector(
    const std::vector<NodeV> &chain,
    std::vector<NodeV>::size_type endPos,
    int value) {
    std::vector<NodeV>::size_type left = 0;
    std::vector<NodeV>::size_type right = endPos;

    while (left < right) {
        std::vector<NodeV>::size_type mid = left + (right - left) / 2;
        if (chain[mid].value < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

std::vector<size_t> PmergeMe::buildJacobsthalOrderVector(size_t pairCount) {
    std::vector<size_t> order;
    if (pairCount <= 1) {
        return order;
    }

    size_t insertedUntil = 1;
    size_t jPrev = 1;
    size_t jCurr = 3;

    while (insertedUntil < pairCount) {
        size_t upper = jCurr;
        if (upper > pairCount) {
            upper = pairCount;
        }

        for (size_t idx = upper; idx > insertedUntil; --idx) {
            order.push_back(idx);
        }

        insertedUntil = upper;
        size_t next = jCurr + 2 * jPrev;
        jPrev = jCurr;
        jCurr = next;
    }

    return order;
}

static std::vector<NodeV>::size_type findBoundLargeVector(
    const std::vector<NodeV> &chain,
    int id) {
    for (std::vector<NodeV>::size_type i = 0; i < chain.size(); ++i) {
        if (chain[i].id == id && chain[i].isLarge) {
            return i;
        }
    }
    return chain.size();
}

void PmergeMe::fordJohnsonSortVector(std::vector<int> &arr) {
    if (arr.size() <= 1) {
        return;
    }

    std::vector<PairItemV> pairs;
    pairs.reserve(arr.size() / 2);

    bool hasStraggler = (arr.size() % 2) != 0;
    int straggler = 0;
    int pairId = 1;

    for (std::vector<int>::size_type i = 0; i < arr.size(); i += 2) {
        if (i + 1 >= arr.size()) {
            straggler = arr[i];
            break;
        }

        PairItemV p;
        if (arr[i] <= arr[i + 1]) {
            p.small = arr[i];
            p.large = arr[i + 1];
        } else {
            p.small = arr[i + 1];
            p.large = arr[i];
        }
        p.id = pairId++;
        pairs.push_back(p);
    }

    if (!pairs.empty()) {
        std::vector<int> largeValues;
        largeValues.reserve(pairs.size());
        for (std::vector<PairItemV>::size_type i = 0; i < pairs.size(); ++i) {
            largeValues.push_back(pairs[i].large);
        }

        fordJohnsonSortVector(largeValues);

        std::vector<PairItemV> sortedPairs;
        sortedPairs.reserve(pairs.size());
        std::vector<bool> used(pairs.size(), false);

        for (std::vector<int>::size_type i = 0; i < largeValues.size(); ++i) {
            for (std::vector<PairItemV>::size_type j = 0; j < pairs.size(); ++j) {
                if (!used[j] && pairs[j].large == largeValues[i]) {
                    sortedPairs.push_back(pairs[j]);
                    used[j] = true;
                    break;
                }
            }
        }

        pairs.swap(sortedPairs);
        for (std::vector<PairItemV>::size_type i = 0; i < pairs.size(); ++i) {
            pairs[i].id = static_cast<int>(i + 1);
        }
    }

    std::vector<NodeV> chain;
    chain.reserve(arr.size());

    if (!pairs.empty()) {
        NodeV a1;
        a1.value = pairs[0].small;
        a1.id = pairs[0].id;
        a1.isLarge = false;
        chain.push_back(a1);

        NodeV b1;
        b1.value = pairs[0].large;
        b1.id = pairs[0].id;
        b1.isLarge = true;
        chain.push_back(b1);

        for (std::vector<PairItemV>::size_type i = 1; i < pairs.size(); ++i) {
            NodeV bi;
            bi.value = pairs[i].large;
            bi.id = pairs[i].id;
            bi.isLarge = true;
            chain.push_back(bi);
        }
    }

    std::vector<PairItemV> byId(pairs.size() + 1);
    for (std::vector<PairItemV>::size_type i = 0; i < pairs.size(); ++i) {
        byId[pairs[i].id] = pairs[i];
    }

    std::vector<size_t> order = buildJacobsthalOrderVector(pairs.size());
    for (std::vector<size_t>::size_type i = 0; i < order.size(); ++i) {
        int id = static_cast<int>(order[i]);
        if (id < 2 || static_cast<std::size_t>(id) >= byId.size()) {
            continue;
        }

        int value = byId[id].small;
        std::vector<NodeV>::size_type boundPos = findBoundLargeVector(chain, id);
        std::vector<NodeV>::size_type insertPos = lowerBoundNodesVector(chain, boundPos, value);

        NodeV node;
        node.value = value;
        node.id = id;
        node.isLarge = false;
        chain.insert(chain.begin() + static_cast<std::ptrdiff_t>(insertPos), node);
    }

    if (hasStraggler) {
        std::vector<NodeV>::size_type insertPos = lowerBoundNodesVector(chain, chain.size(), straggler);
        NodeV node;
        node.value = straggler;
        node.id = -1;
        node.isLarge = false;
        chain.insert(chain.begin() + static_cast<std::ptrdiff_t>(insertPos), node);
    }

    arr.clear();
    arr.reserve(chain.size());
    for (std::vector<NodeV>::size_type i = 0; i < chain.size(); ++i) {
        arr.push_back(chain[i].value);
    }
}

static std::deque<NodeD>::size_type lowerBoundNodesDeque(
    const std::deque<NodeD> &chain,
    std::deque<NodeD>::size_type endPos,
    int value) {
    std::deque<NodeD>::size_type left = 0;
    std::deque<NodeD>::size_type right = endPos;

    while (left < right) {
        std::deque<NodeD>::size_type mid = left + (right - left) / 2;
        if (chain[mid].value < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

std::deque<size_t> PmergeMe::buildJacobsthalOrderDeque(size_t pairCount) {
    std::deque<size_t> order;
    if (pairCount <= 1) {
        return order;
    }

    size_t insertedUntil = 1;
    size_t jPrev = 1;
    size_t jCurr = 3;

    while (insertedUntil < pairCount) {
        size_t upper = jCurr;
        if (upper > pairCount) {
            upper = pairCount;
        }

        for (size_t idx = upper; idx > insertedUntil; --idx) {
            order.push_back(idx);
        }

        insertedUntil = upper;
        size_t next = jCurr + 2 * jPrev;
        jPrev = jCurr;
        jCurr = next;
    }

    return order;
}

static std::deque<NodeD>::size_type findBoundLargeDeque(
    const std::deque<NodeD> &chain,
    int id) {
    for (std::deque<NodeD>::size_type i = 0; i < chain.size(); ++i) {
        if (chain[i].id == id && chain[i].isLarge) {
            return i;
        }
    }
    return chain.size();
}

void PmergeMe::fordJohnsonSortDeque(std::deque<int> &arr) {
    if (arr.size() <= 1) {
        return;
    }

    std::deque<PairItemD> pairs;

    bool hasStraggler = (arr.size() % 2) != 0;
    int straggler = 0;
    int pairId = 1;

    for (std::deque<int>::size_type i = 0; i < arr.size(); i += 2) {
        if (i + 1 >= arr.size()) {
            straggler = arr[i];
            break;
        }

        PairItemD p;
        if (arr[i] <= arr[i + 1]) {
            p.small = arr[i];
            p.large = arr[i + 1];
        } else {
            p.small = arr[i + 1];
            p.large = arr[i];
        }
        p.id = pairId++;
        pairs.push_back(p);
    }

    if (!pairs.empty()) {
        std::deque<int> largeValues;
        for (std::deque<PairItemD>::size_type i = 0; i < pairs.size(); ++i) {
            largeValues.push_back(pairs[i].large);
        }

        fordJohnsonSortDeque(largeValues);

        std::deque<PairItemD> sortedPairs;
        std::deque<bool> used(pairs.size(), false);

        for (std::deque<int>::size_type i = 0; i < largeValues.size(); ++i) {
            for (std::deque<PairItemD>::size_type j = 0; j < pairs.size(); ++j) {
                if (!used[j] && pairs[j].large == largeValues[i]) {
                    sortedPairs.push_back(pairs[j]);
                    used[j] = true;
                    break;
                }
            }
        }

        pairs.swap(sortedPairs);
        for (std::deque<PairItemD>::size_type i = 0; i < pairs.size(); ++i) {
            pairs[i].id = static_cast<int>(i + 1);
        }
    }

    std::deque<NodeD> chain;
    if (!pairs.empty()) {
        NodeD a1;
        a1.value = pairs[0].small;
        a1.id = pairs[0].id;
        a1.isLarge = false;
        chain.push_back(a1);

        NodeD b1;
        b1.value = pairs[0].large;
        b1.id = pairs[0].id;
        b1.isLarge = true;
        chain.push_back(b1);

        for (std::deque<PairItemD>::size_type i = 1; i < pairs.size(); ++i) {
            NodeD bi;
            bi.value = pairs[i].large;
            bi.id = pairs[i].id;
            bi.isLarge = true;
            chain.push_back(bi);
        }
    }

    std::deque<PairItemD> byId(pairs.size() + 1);
    for (std::deque<PairItemD>::size_type i = 0; i < pairs.size(); ++i) {
        byId[pairs[i].id] = pairs[i];
    }

    std::deque<size_t> order = buildJacobsthalOrderDeque(pairs.size());
    for (std::deque<size_t>::size_type i = 0; i < order.size(); ++i) {
        int id = static_cast<int>(order[i]);
        if (id < 2 || static_cast<std::size_t>(id) >= byId.size()) {
            continue;
        }

        int value = byId[id].small;
        std::deque<NodeD>::size_type boundPos = findBoundLargeDeque(chain, id);
        std::deque<NodeD>::size_type insertPos = lowerBoundNodesDeque(chain, boundPos, value);

        NodeD node;
        node.value = value;
        node.id = id;
        node.isLarge = false;
        chain.insert(chain.begin() + static_cast<std::ptrdiff_t>(insertPos), node);
    }

    if (hasStraggler) {
        std::deque<NodeD>::size_type insertPos = lowerBoundNodesDeque(chain, chain.size(), straggler);
        NodeD node;
        node.value = straggler;
        node.id = -1;
        node.isLarge = false;
        chain.insert(chain.begin() + static_cast<std::ptrdiff_t>(insertPos), node);
    }

    arr.clear();
    for (std::deque<NodeD>::size_type i = 0; i < chain.size(); ++i) {
        arr.push_back(chain[i].value);
    }
}

void PmergeMe::run() {
    printSequence("Before: ", _input);

    std::clock_t startVec = std::clock();
    std::vector<int> v(_input.begin(), _input.end());
    fordJohnsonSortVector(v);
    std::clock_t endVec = std::clock();

    std::clock_t startDeq = std::clock();
    std::deque<int> d(_input.begin(), _input.end());
    fordJohnsonSortDeque(d);
    std::clock_t endDeq = std::clock();

    std::vector<int> after(v.begin(), v.end());
    printSequence("After: ", after);

    double vecUs = static_cast<double>(endVec - startVec) * 1000000.0 / CLOCKS_PER_SEC;
    double deqUs = static_cast<double>(endDeq - startDeq) * 1000000.0 / CLOCKS_PER_SEC;

    std::cout << "Time to process a range of " << _input.size()
              << " elements with std::vector : " << vecUs << " us" << std::endl;
    std::cout << "Time to process a range of " << _input.size()
              << " elements with std::deque : " << deqUs << " us" << std::endl;
}
