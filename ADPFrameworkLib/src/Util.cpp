#include <string>
#include <fstream>
#include <cmath>

#include "Util.h"

namespace Util
{
    std::string ToString(const ShortArray8d a)
    {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";
       
        return str;   
    }

    std::string ToString(const ShortArray4d a)
    {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";
       
        return str;   
    }

    std::string ToString(const ShortArray2d a)
    {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";
       
        return str;   
    }


   std::string ToString(const IntArray4d a)
   {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";
       
        return str;
   }

    std::string ToString(const IntArray8d a)
    {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";

        return str;
    }

    uShort SumArray(ShortArray4d array)
    {
        int sum = 0;

        for(const auto value : array)
            sum += value;

        return sum;
    }

    int SumArray(IntArray4d array)
    {
        int sum = 0;

        for(const auto value : array)
            sum += value;

        return sum;
    }

   std::string ToString(const IntArray2d a)
   {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";

        return str;
   }

    std::string ToString(const BoolArray2d a)
    {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";

        return str;
    }

   int SumArray(IntArray2d array)
   {
        int sum = 0;

        for(const auto value : array)
            sum += value;

        return sum;
   }

    uShort SumArray(ShortArray2d array)
    {
        int sum = 0;

        for(const auto value : array)
            sum += value;

        return sum;
    }

    BinomialDistribution::BinomialDistribution()
    {
        pSuccess_ = 0.;
    }

    BinomialDistribution::BinomialDistribution(const double pSuccess)
    {
        pSuccess_ = pSuccess;
    }

    float
    BinomialDistribution::GetProbability(const int N, const int k)
    {
        IntArray2d key = {N,k};

        const auto iter = lookupTable_.find(key);

        if(iter == lookupTable_.end())
        {
            lookupTable_[key] = Binomial(N,k, pSuccess_);
        }
        
        return lookupTable_[key];
    }

    float
    Possion(const int n, const double lambda)
    {
        return pow(lambda, n) * exp(-1.*lambda) / Factorial(n);
    }

    float
    Binomial(const int nTrials, const int nSuccess, const double pSuccess)
    {
        double result = 0.;
        if(nSuccess > nTrials) return result;

        if(nTrials < 100)
        {
            result = Combination(nTrials, nSuccess) 
                        * pow(pSuccess, nSuccess) 
                        * pow(1.-pSuccess, nTrials - nSuccess);                                
        }
        else
        {

            const auto r = static_cast<double>(nSuccess);
            const auto N = static_cast<double>(nTrials);

            const auto u = N*pSuccess;
            const auto s = sqrt(N*pSuccess*(1-pSuccess));
            
            const auto arg1 = (r+0.5 - u)/(sqrt(2.)*s);
            const auto arg2 = (r-0.5 - u)/(sqrt(2.)*s);

            result = 0.5*( erf(arg1) - erf(arg2));
        }

        return result;
    }

    float
    Combination(const int n, const int r)
    {
        return Factorial(n) / (Factorial(r) * Factorial(n - r));
    }

    float
    Factorial(const int x)
    {
        double result = 1.;
        
        for(int i = 1; i <= x; i++)
        {
                result *= static_cast<double>(i);
        }
    
        return result;
    }

}