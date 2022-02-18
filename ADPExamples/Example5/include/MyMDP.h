#ifndef MY_MDP_H
#define MY_MDP_H

#include <memory>
#include <array>

#include "MDP.h"

#include "MyDecision.h"
#include "MyState.h"
#include "MyPolicy.h"
#include "MyUtil.h"

using namespace MyUtil;

class StateHash;
class StateEqual;

class MyMDP : public MarkovDecisionProcess
{
public:

    MyMDP();
    
    ~MyMDP();

    bool IsEndState() const;

    double CalculateContribution(const StateSharedPtr state, const DecisionSharedPtr decision) const;

    void UpdateDecision();

    void UpdateExogenous();

    void SetInitialState(const StateSharedPtr initialState);
    void SetCurrentState(const StateSharedPtr state);
    void SetDecision(const DecisionSharedPtr decision);
    void SetPolicy(const PolicySharedPtr& decisionPolicy);

    void RecordLearning(const Step& s);

    DecisionSpace GetLegalDecisions() const;
    DecisionSpace GetLegalDecisions(const StateSharedPtr state) const;
    const DecisionSpace GetFullDecisionSpace() const;

    void Reset();

private:

    bool isLegal(const MyDecisionSharedPtr d, const MyStateSharedPtr s) const;
    void initializeDecisionSpace();
    void initializeStateSpace();

    DecisionSpace fullDecisionSpace_; 

    std::array<DoubleArray4d, 150> decisionHistory_;
    std::array<DoubleArray4d, 150> stateHistory_;
    int historyCounter_;
    int outputCounter_;
};

#endif