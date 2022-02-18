#ifndef POLICY_H
#define POLICY_H

#include <map>
#include <fstream>
#include <string>

#include "State.h"
#include "Decision.h"
#include "ValueFunction.h"

class Policy
{
public:

    Policy();
    ~Policy();

    virtual DecisionSharedPtr operator() (const StateSharedPtr s) const;

private:

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

};

template<class StateHashFn, class StateEqualFn>
class LUTPolicy : public Policy
{
    // ToDo: Complete this check to make sure the template classes have the correct interface
    //static_assert(std::is_base_of<LUTHash, StateHashFn>::value, "StateHashFn must inherit from LUTHash");
    using LUT = std::unordered_map<StateSharedPtr, DecisionSharedPtr, StateHashFn, StateEqualFn>;

public:

    LUTPolicy();
    ~LUTPolicy();

    DecisionSharedPtr operator() (const StateSharedPtr s) const;

    void UpdatePolicy(const StateSharedPtr s, const DecisionSharedPtr d);

    void SetDefaultDecision(const DecisionSharedPtr& d);

    LUT GetTable() const {return lookupTable_;}

    void WriteToFile(const std::string& filename) const;

    void Print() const;

    int GetDefaultDecisionUsed() const {return defaultDecisionUsed_;}

    void ResetDefaultDecisionUsed() {defaultDecisionUsed_ = 0;}

private:

    LUT lookupTable_;

    mutable int defaultDecisionUsed_;

    DecisionSharedPtr defaultDecision_;
};

template<class StateHashFn, class StateEqualFn>
LUTPolicy<StateHashFn, StateEqualFn>::LUTPolicy()
{
    defaultDecision_ = nullptr;
    defaultDecisionUsed_ = 0;
}


template<class StateHashFn, class StateEqualFn>
LUTPolicy<StateHashFn, StateEqualFn>::~LUTPolicy()
{

}

template<class StateHashFn, class StateEqualFn>
void 
LUTPolicy<StateHashFn, StateEqualFn>::Print() const
{
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    for(const auto kv : lookupTable_)
    {
        std::cout << kv.first->ToString() << " -> " << kv.second->ToString() << std::endl;
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

template<class StateHashFn, class StateEqualFn>
void 
LUTPolicy<StateHashFn, StateEqualFn>::WriteToFile(const std::string& filename) const
{
    std::ofstream out;
    out.open(filename.c_str());

    if(out.is_open())
    {
        for(const auto kv : lookupTable_)
        {
            out << kv.first->ToString() << " -> " << kv.second->ToString() << std::endl;
        }
    }
    else
    {
        std::cout << "[WARNING] LUTPolicy unable to open file: " << filename << std::endl;
    }
}

template<class StateHashFn, class StateEqualFn>
DecisionSharedPtr 
LUTPolicy<StateHashFn, StateEqualFn>::operator() (const StateSharedPtr s) const
{
    DecisionSharedPtr d = nullptr;

    const auto iter = lookupTable_.find(s);

    if(iter != lookupTable_.end())
    {
        d = lookupTable_.at(s);
    }
    else
    {
        defaultDecisionUsed_++;
        d = defaultDecision_;
    }

    return d;
}

template<class StateHashFn, class StateEqualFn>
void 
LUTPolicy<StateHashFn, StateEqualFn>::UpdatePolicy(const StateSharedPtr s, const DecisionSharedPtr d)
{
    lookupTable_[s] = d;
}

template<class StateHashFn, class StateEqualFn>
void 
LUTPolicy<StateHashFn, StateEqualFn>::SetDefaultDecision(const DecisionSharedPtr& d)
{
    defaultDecision_ = d;    
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class T>
class VFAPolicy : public Policy
{
    using ValueFunctionSharedPtr = std::shared_ptr<ValueFunction<T>>;

public:

    VFAPolicy(){};

    VFAPolicy(const ValueFunctionSharedPtr& vf)
    {
        SetValueFunction(vf);   
    }

    ~VFAPolicy(){}

    virtual void SetValue(const T& arg, double value)
    {
        std::cout << "[WARNING] VFAPolicy base class SetValue function used..." << std::endl;
        return;
    }

    double GetValue(const T& arg) const
    {
        return (*valueFunction_)(arg);
    }

    void SetValueFunction(const ValueFunctionSharedPtr& vf)
    {
        valueFunction_ = vf;
    }

    const ValueFunctionSharedPtr GetValueFunction() const
    {
       return valueFunction_;
    }

    virtual DecisionSharedPtr operator() (const StateSharedPtr s) const
    {
        std::cout << "[WARNING] VFAPolicy base class () operator used..." << std::endl;
        return std::make_shared<Decision>();
    }

private:

   ValueFunctionSharedPtr valueFunction_;

};

#endif