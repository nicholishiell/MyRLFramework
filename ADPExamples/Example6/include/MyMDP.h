#ifndef MY_MDP_H
#define MY_MDP_H

#include <memory>

#include "MDP.h"
#include "Util.h"

#include "MyDecision.h"
#include "MyState.h"
#include "MyPolicy.h"

using namespace Util;

class MyMDP : public MarkovDecisionProcess
{
public:

    MyMDP();
    
    ~MyMDP();

    bool IsEndState() const;

    double CalculateContribution() const;
    double CalculateContribution(   const StateSharedPtr state, 
                                    const DecisionSharedPtr decision) const;
    void UpdateDecision();

    void UpdateExogenous();

    void RecordLearning(const Step& s);

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

    DoubleArray2d transitionProbs_;
    ShortArray2d spaceRequirements_;
    int heloCapacity_;

    std::array<DoubleArray2d, 50> decisionHistory_;
    std::array<DoubleArray2d, 50> stateHistory_;
    int historyCounter_;
    int outputCounter_;
};

#endif