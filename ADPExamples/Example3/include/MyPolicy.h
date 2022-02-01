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

using QTable = LUTValueFunction<StateDecisionPair, StateDecisionHash>;
using QTableSharedPtr = std::shared_ptr<QTable>;

class QTablePolicy : public VFAPolicy<StateDecisionPair>
{
public:
    QTablePolicy();
    QTablePolicy(const DecisionSpace& ds, const QTableSharedPtr vf);
    ~QTablePolicy();
    
    DecisionSharedPtr operator() (const StateSharedPtr s) const;

private:

    DecisionSpace fullDecisionSpace_;

};

using QTablePolicySharedPtr = std::shared_ptr<QTablePolicy>;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif