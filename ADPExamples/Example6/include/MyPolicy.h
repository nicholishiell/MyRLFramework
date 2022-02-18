#ifndef MY_POLICY_H
#define MY_POLICY_H

#include "Policy.h"
#include "MyDecision.h"
#include "MyValueFunction.h"

class CriticalFirstPolicy : public Policy
{
public:

    CriticalFirstPolicy();
    ~CriticalFirstPolicy();

    DecisionSharedPtr operator() (const StateSharedPtr s) const;

private:
    
};

using CriticalFirstPolicySharedPtr = std::shared_ptr<CriticalFirstPolicy>;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif