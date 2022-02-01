#ifndef MY_MDP_H
#define MY_MDP_H

#include <memory>

#include "MDP.h"
#include "MyDecision.h"
#include "MyState.h"
#include "MyPolicy.h"

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

    DecisionSpace GetLegalDecisions() const;
    DecisionSpace GetLegalDecisions(const StateSharedPtr state) const;
    const DecisionSpace GetFullDecisionSpace() const;

    void Reset();

private:

    bool isLegal(const MyDecisionSharedPtr d, const MyStateSharedPtr s) const;
    void initializeDecisionSpace();
    void initializeStateSpace();

    DecisionSpace fullDecisionSpace_;

    DoubleArray4d transitionProbs_;
    IntArray4d spaceRequirements_;
    int heloCapacity_;    
};

#endif