#ifndef MY_MDP_H
#define MY_MDP_H

#include <memory>
#include <queue>
#include <vector>

#include "MDP.h"

struct QueueParameters
{
    double departureRate_;
    
    double priceMean_;
    double priceStdev_;

    double timeMean_;
    double timeStdev_;
};

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

    QueueParameters queue1_;
    QueueParameters queue2_;
    
    std::priority_queue<double, std::vector<double>, std::greater<double> > currentJobsQueue_;

    double currentTimeStamp_ = 0.;
};

#endif