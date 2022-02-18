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

class GreenFirstPolicy : public Policy
{
public:

    GreenFirstPolicy();
    ~GreenFirstPolicy();

    DecisionSharedPtr operator() (const StateSharedPtr s) const;

private:
    
};

using GreenFirstPolicySharedPtr = std::shared_ptr<GreenFirstPolicy>;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class RandomPolicy : public Policy
{
public:

    RandomPolicy();
    ~RandomPolicy();

    void SetDecisionSpace(const DecisionSpace ds);

    DecisionSharedPtr operator() (const StateSharedPtr s) const;

private:
    
    DecisionSpace fullDecisionSpace_;
};

using RandomPolicySharedPtr = std::shared_ptr<RandomPolicy>;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

using QTable = LUTValueFunction<StateDecisionPair, StateDecisionHash, StateDecisionEqual>;
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