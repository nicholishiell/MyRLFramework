#ifndef MY_MDP_H
#define MY_MDP_H

#include <memory>

#include "MDP.h"

class MyMDP : public MarkovDecisionProcess
{
public:

    MyMDP();
    
    ~MyMDP();

    bool IsEndState() const;

    double CalculateContribution() const;

    void UpdateDecision();

    void UpdateExogenous();

    void SetInitialState(const StateSharedPtr initialState);
    void SetCurrentState(const StateSharedPtr state);
    void SetDecision(const DecisionSharedPtr decision);
    void SetPolicy(const PolicySharedPtr& decisionPolicy);

    void Reset();

private:

    DoubleArray4d transitionProbs_;

};

#endif