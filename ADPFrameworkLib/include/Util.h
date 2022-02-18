#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <vector>
#include <array>
#include <string>
#include <limits>
#include <unordered_map>

class State;
class Decision;
class Policy;
class ValueFunctio;
class LUTKey;
class LUTHash;

namespace Util
{
    struct Array2DHasher 
    {
        std::size_t operator()(const std::array<int, 2>& a) const 
        {
            std::size_t h = 0;

            for (auto e : a) 
            {
                h ^= std::hash<int>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
            }
            return h;
        }
    }; 

    using uShort = unsigned short;
    using ShortArray8d = std::array<uShort,8>;
    using ShortArray4d = std::array<uShort,4>;
    using ShortArray2d = std::array<uShort,2>;

    using IntArray8d = std::array<int,8>;
    
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

    std::string ToString(const ShortArray8d a);
    std::string ToString(const ShortArray4d a);
    std::string ToString(const ShortArray2d a);
    std::string ToString(const IntArray8d a);
    std::string ToString(const IntArray4d a);
    std::string ToString(const IntArray2d a);
    std::string ToString(const BoolArray2d a);

    uShort SumArray(ShortArray4d array);
    uShort SumArray(ShortArray2d array);
    int SumArray(IntArray4d array);
    int SumArray(IntArray2d array);

    const double VERY_NEGATIVE_NUMBER = -1.*std::numeric_limits<double>::max();

    using BinomialLookupTable = std::unordered_map<IntArray2d, float, Array2DHasher>;

    class BinomialDistribution
    {
        public:
            BinomialDistribution();
            BinomialDistribution(const double pSuccess);

            float GetProbability(const int N, const int k);

        private:

        BinomialLookupTable lookupTable_;
        double pSuccess_;
    };

    float Possion(const int n, const double lambda);

    float Binomial(const int nTrials, const int nSuccess, const double pSuccess);

    int BinomialCoeff(int n, int k);
    float Combination(const int n, const int r);

    float Factorial(const int x);
}

#endif