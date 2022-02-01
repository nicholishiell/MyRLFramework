#ifndef MY_POLICY_H
#define MY_POLICY_H

#include "Policy.h"

class MyPolicy : public Policy
{
public:

    MyPolicy();
    ~MyPolicy();

    DecisionSharedPtr operator() (const StateSharedPtr s) const;

private:

    
};

using MyPolicySharedPtr = std::shared_ptr<MyPolicy>;

#endif