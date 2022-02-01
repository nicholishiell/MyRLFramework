#ifndef VALUE_FUNCTION_H
#define VALUE_FUNCTION_H

#include <unordered_map>

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
};

class LUTHash
{
public:
    virtual size_t operator()(const LUTKey& k) const = 0;  
};

template<class KEY, class HASH>
class LUTValueFunction : public ValueFunction<KEY>
{
    // These make sure that the KEY and HASH are of type LUTKey and LUTHash respectively.
    static_assert(std::is_base_of<LUTKey, KEY>::value, "KEY must inherit from LUTKey");
    static_assert(std::is_base_of<LUTHash, HASH>::value, "HASH must inherit from LUTHash");

public:
 
    LUTValueFunction(){};
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

private:

    std::unordered_map<KEY, double, HASH> lookupTable_;
    double defaultValue_;  
};

#endif
