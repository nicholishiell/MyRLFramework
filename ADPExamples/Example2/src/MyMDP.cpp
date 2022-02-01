#include <math.h>

#include "MyMDP.h"
#include "MyDecision.h"
#include "MyState.h"
#include "MyPolicy.h"

MyMDP::MyMDP()
{
    queue1_.departureRate_ = 2.;
    queue1_.priceMean_ = 100.;
    queue1_.priceStdev_ = 25.;
    queue1_.timeMean_ = 2.;
    queue1_.timeStdev_ = 0.5;

    queue2_.departureRate_ = 5.;
    queue2_.priceMean_ = 1000.;
    queue2_.priceStdev_ = 100.;
    queue2_.timeMean_ = 10.;
    queue2_.timeStdev_ = 5.;

    currentTimeStamp_ = 0.;
}

MyMDP::~MyMDP()
{
    
}

bool 
MyMDP::IsEndState() const
{
    auto sCast = std::dynamic_pointer_cast<MyState>(currentState_);
    return SumArray(sCast->GetQueuePopulations()) == 0;
}

double 
MyMDP::CalculateContribution() const
{
    auto dCast = std::dynamic_pointer_cast<MyDecision>(currentDecision_);

    const auto d = dCast->GetQueueChoice();

    double reward = 0.;

    if(d == 0)
        reward = queue1_.priceMean_;
    else
        reward = queue2_.priceMean_;

    return reward;
}

void 
MyMDP::UpdateDecision()
{
    auto sCast = std::dynamic_pointer_cast<MyState>(currentState_);
    auto dCast = std::dynamic_pointer_cast<MyDecision>(currentDecision_);

    // Remove the job from the waiting queue
    auto currentQueuePopulations = sCast->GetQueuePopulations();
    currentQueuePopulations[dCast->GetQueueChoice()]--;
    currentState_ = std::make_shared<MyState>(currentQueuePopulations);

    // Add the job to the current job queue;
    double mean = 0.;
    double stdev = 0.;

    if(dCast->GetQueueChoice() == 0)
    {
        mean = queue1_.timeMean_;
        stdev = queue1_.timeStdev_;
    }
    else
    {
        mean = queue2_.timeMean_;
        stdev = queue2_.timeStdev_;
    }

    std::normal_distribution<double> distribution(mean,stdev);
    
    double jobCompletitionTimeStamp = currentTimeStamp_ + distribution(gen_);
    currentJobsQueue_.push(jobCompletitionTimeStamp);
}

double CalcProb(const double rate, const double deltaTime)
{
    return 1. - exp(-1./rate*deltaTime);
}

void 
MyMDP::UpdateExogenous()
{
    // Get the next completed job
    const auto deltaTime = currentJobsQueue_.top() - currentTimeStamp_;
    currentJobsQueue_.pop();
    currentTimeStamp_ += deltaTime;

    auto sCast = std::dynamic_pointer_cast<MyState>(currentState_);
    auto population = sCast->GetQueuePopulations();

    std::uniform_real_distribution<double> randomNumber(0.0, 1.0);

    IntArray2d exogenousInfo = {0,0};

    DoubleArray2d transitionProbs = {CalcProb(queue1_.departureRate_,deltaTime),
                                     CalcProb(queue2_.departureRate_,deltaTime)};

    // Generate the exogenous information
    for(int i = 0; i < population.size(); i++) 
    {
        for(int j = 0; j < population[i]; j++)
        {
            if(randomNumber(gen_) < transitionProbs[i])
            {
                exogenousInfo[i]++;
            }
        }
    }

    // Apply the exogenous information to the current population
    for(int i = 0; i < population.size(); i++) 
    {
            population[i] -= exogenousInfo[i];
    }

    // Set the current state to the new population
    currentState_ = std::make_shared<MyState>(population);
}

void 
MyMDP::Reset()
{
    auto sCast = std::dynamic_pointer_cast<MyState>(initialState_);
    currentState_ = std::make_shared<MyState>(sCast->GetQueuePopulations());
}

void 
MyMDP::SetInitialState(const StateSharedPtr initialState)
{
    auto sCast = std::dynamic_pointer_cast<MyState>(initialState);
    initialState_ = std::make_shared<MyState>(sCast->GetQueuePopulations());
}

void 
MyMDP::SetCurrentState(const StateSharedPtr state)
{
    auto sCast = std::dynamic_pointer_cast<MyState>(state);
    currentState_ = std::make_shared<MyState>(sCast->GetQueuePopulations());
}

void 
MyMDP::SetDecision(const DecisionSharedPtr decision)
{
    currentDecision_ = std::make_shared<MyDecision>(0);
}

void 
MyMDP::SetPolicy(const PolicySharedPtr& decisionPolicy)
{
    auto pCast = std::dynamic_pointer_cast<CriticalFirstPolicy>(decisionPolicy);

    decisionPolicy_ = std::make_shared<CriticalFirstPolicy>(*pCast);
}
