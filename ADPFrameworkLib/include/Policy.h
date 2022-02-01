#ifndef POLICY_H
#define POLICY_H

#include <map>

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

class LUTPolicy
{

    using LUT = std::unordered_map<StateSharedPtr, DecisionSharedPtr, State>;

public:

    LUTPolicy();
    ~LUTPolicy();

    DecisionSharedPtr operator() (const StateSharedPtr s) const;

    void UpdatePolicy(const StateSharedPtr s, const DecisionSharedPtr d);
   
    LUT GetTable() const {return lookupTable_;}

private:

    LUT lookupTable_;
};

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