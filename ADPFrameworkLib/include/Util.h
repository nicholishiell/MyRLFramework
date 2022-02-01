#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <vector>
#include <array>
#include <string>
#include <limits>

class State;
class Decision;
class Policy;
class ValueFunctio;
class LUTKey;
class LUTHash;

namespace Util
{
    using IntArray4d = std::array<int,4>;
    using DoubleArray4d = std::array<double,4>;

    using IntArray2d = std::array<int,2>;
    using BoolArray2d = std::array<bool,2>;
    using DoubleArray2d = std::array<double,2>;

    using DecisionSharedPtr = std::shared_ptr<Decision>;
    using StateSharedPtr = std::shared_ptr<State>;
    using PolicySharedPtr = std::shared_ptr<Policy>;
    
    using LUTKeySharedPtr = std::shared_ptr<LUTKey>;
    using LUTHashSharedPtr = std::shared_ptr<LUTHash>;

    using DecisionSpace = std::vector<DecisionSharedPtr>;
    using StateSpace = std::vector<StateSharedPtr>;

    std::string ToString(const IntArray4d a);
    std::string ToString(const IntArray2d a);
    std::string ToString(const BoolArray2d a);

    int SumArray(IntArray4d array);
    int SumArray(IntArray2d array);

    const double VERY_NEGATIVE_NUMBER = -1.*std::numeric_limits<double>::max();
}

#endif