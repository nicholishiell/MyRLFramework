#ifndef MDP_H
#define MDP_H

#include <vector>
#include <memory>
#include <random>
#include <chrono>

#include "Util.h"
#include "Policy.h"
#include "State.h"
#include "Decision.h"

struct Step
{
    StateSharedPtr s;
    DecisionSharedPtr d;
    double r;
};

using Trajectory = std::vector<Step>;

class MarkovDecisionProcess
{
public:

    MarkovDecisionProcess();
    ~MarkovDecisionProcess();

    Trajectory RunEpisode();

    virtual bool IsEndState() const;

    virtual void SetInitialState(const StateSharedPtr initialState);
    virtual void SetCurrentState(const StateSharedPtr state);
    virtual void SetDecision(const DecisionSharedPtr decision);
    virtual void SetPolicy(const PolicySharedPtr& decisionPolicy);
    void SetGamma(const double gamma);

    StateSpace GetStateSpace() const;
    const StateSharedPtr GetInitialState() const;
    const StateSharedPtr GetCurrentState() const;
    const DecisionSharedPtr GetCurrentDecision() const;
    PolicySharedPtr GetDecisionPolicy() const;
    double GetGamma() const;
    double GetCumulativeReward() const;

    virtual DecisionSpace GetLegalDecisions() const;
    virtual DecisionSpace GetLegalDecisions(const StateSharedPtr state) const;

    virtual double CalculateContribution() const;

    virtual void UpdateDecision();
    
    virtual void UpdateExogenous();

    virtual void Reset();

    double GetdeltaTimeGetDecision() const {return deltaTimeGetDecision_;}
    double GetdeltaTimeCalculateContribution() const{return deltaTimeCalculateContribution_;}
    double GetdeltaTimeUpdateDecision() const{return deltaTimeUpdateDecision_;}
    double GetdeltaTimeUpdateExogenous() const{return deltaTimeUpdateExogenous_;}

protected:

    bool isReady();

    StateSharedPtr initialState_;
    StateSharedPtr currentState_;
    DecisionSharedPtr currentDecision_;

    PolicySharedPtr decisionPolicy_;

    StateSpace stateSpace_;

    std::mt19937 gen_;

    double gamma_;

    double cumulativeReward_;

    double deltaTimeGetDecision_;
    double deltaTimeCalculateContribution_;
    double deltaTimeUpdateDecision_;
    double deltaTimeUpdateExogenous_;

};

#endif