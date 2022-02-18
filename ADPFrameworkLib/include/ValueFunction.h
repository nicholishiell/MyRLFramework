#ifndef VALUE_FUNCTION_H
#define VALUE_FUNCTION_H

#include <unordered_map>
#include <fstream>
#include <chrono>
#include <ctime>
#include <random>

#include "Util.h"
#include "State.h"
#include "Decision.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <class T>
class ValueFunction
{
public:

    // TODO: Make this pure virtual
    virtual double operator()(const T& arg) const
    {
        return 0.;
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class LUTKey
{
public:
    virtual bool operator==(const LUTKey& p) const = 0; 

    virtual std::string ToString() const = 0;
};

class LUTHash
{
public:
    virtual size_t operator()(const LUTKey& k) const = 0;  
};

class LUTEqual
{
public:

  virtual bool operator()(  const LUTKey& lhs,
                            const LUTKey& rhs) const = 0;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class KEY, class TABLE_HASH, class TABLE_EQUAL>
class LUTValueFunction : public ValueFunction<KEY>
{
    // These make sure that the KEY and HASH are of type LUTKey and LUTHash respectively.
    static_assert(std::is_base_of<LUTKey, KEY>::value, "KEY must inherit from LUTKey");
    static_assert(std::is_base_of<LUTHash, TABLE_HASH>::value, "HASH must inherit from LUTHash");
    static_assert(std::is_base_of<LUTEqual, TABLE_EQUAL>::value, "EQUAL  must inherit from LUTEqual");

public:
 
    LUTValueFunction()
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        gen_ = std::mt19937(seed);

        dist_ = std::uniform_real_distribution<double>(0.,1.);
    };

    ~LUTValueFunction(){};

    void SetValue(const KEY& key, const double val)
    {
       lookupTable_[key] = val;
    }

    void SetDefaultValue(const double defaultValue)
    {
        defaultValue_ = defaultValue;
    }

    double operator()(const KEY& arg) const
    {
        //double retVal = dist_(gen_)*defaultValue_;
        double retVal = defaultValue_;

        const auto iter = lookupTable_.find(arg);
        if(iter != lookupTable_.end())
        {
            retVal = lookupTable_.at(arg);
        }
        
        return retVal;
    }

    bool Contains(const KEY& arg) const
    {
        return lookupTable_.find(arg) != lookupTable_.end(); 
    }

    unsigned int GetSize() const
    {
        return lookupTable_.size();
    }

    void Print() const
    {
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        int counter = 1;
        for(const auto& kv : lookupTable_)
        {
            std::cout << counter++ << " " << kv.first.ToString() << " -> " << kv.second << std::endl;
        }
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    }

    void WriteToFile(std::string filename =  "valueFunction.dat") const
    {
        std::ofstream out;
        out.open(filename.c_str());
        for(const auto& kv : lookupTable_)
        {
            out << kv.first.ToString() << " -> " << kv.second << std::endl;
        }
        
        out.close();
    }

private:

    std::unordered_map<KEY, float, TABLE_HASH, TABLE_EQUAL> lookupTable_;
    double defaultValue_;  

    mutable std::mt19937 gen_;
    mutable std::uniform_real_distribution<double> dist_;
};

#endif
