#include "MyMDP.h"
#include "MyDecision.h"
#include "MyState.h"
#include "MyPolicy.h"

MyMDP::MyMDP()
{
    transitionProbs_ = {0.01, 0.05, 0.25, 0.9};    
}

MyMDP::~MyMDP()
{
    
}

bool 
MyMDP::IsEndState() const
{
    auto sCast = std::dynamic_pointer_cast<MyState>(currentState_);
    int totalPopulation = 0;

    if(sCast == nullptr) std::cout << "here" << std::endl;

    for(const auto n : sCast->GetTriagePopulations())
    {
        totalPopulation += n;
    }
    return !(totalPopulation > 0);
}

double 
MyMDP::CalculateContribution() const
{
    const auto dCast = std::dynamic_pointer_cast<MyDecision>(currentDecision_);

    double reward = 0.;
    
    for(const auto value : dCast->GetDecisionArray())    
    {
        reward += value;
    }

    return reward;
}

void 
MyMDP::UpdateDecision()
{
    const auto dCast = std::dynamic_pointer_cast<MyDecision>(currentDecision_);
    auto sCast = std::dynamic_pointer_cast<MyState>(currentState_);

    auto population = sCast->GetTriagePopulations();
    auto decisionArray = dCast->GetDecisionArray();

    for(int i = 0; i < population.size(); i++)
        population[i] -= decisionArray[i];

    currentState_ = std::make_shared<MyState>(population);
}

void 
MyMDP::UpdateExogenous()
{
    auto sCast = std::dynamic_pointer_cast<MyState>(currentState_);
    auto population = sCast->GetTriagePopulations();

    std::uniform_real_distribution<double> randomNumber(0.0, 1.0);

    IntArray4d exogenousInfo = {0,0,0,0};

    // Generate the exogenous information
    for(int iTriage = 0; iTriage < population.size(); iTriage++) 
    {
        for(int iPerson = 0; iPerson < population[iTriage]; iPerson++)
        {
            if(randomNumber(gen_) < transitionProbs_[iTriage])
            {
                exogenousInfo[iTriage]++;
            }
        }
    }

    // Apply the exogenous information to the current population
    for(int iTriage = 0; iTriage < population.size(); iTriage++) 
    {
            population[iTriage] -= exogenousInfo[iTriage];
            if(iTriage < 3) population[iTriage+1] += exogenousInfo[iTriage];
    }

    // Set the current state to the new population
    currentState_ = std::make_shared<MyState>(population);
}

void 
MyMDP::Reset()
{
    auto sCast = std::dynamic_pointer_cast<MyState>(initialState_);
    currentState_ = std::make_shared<MyState>(sCast->GetTriagePopulations());
}

void 
MyMDP::SetInitialState(const StateSharedPtr initialState)
{
    auto sCast = std::dynamic_pointer_cast<MyState>(initialState);
    initialState_ = std::make_shared<MyState>(sCast->GetTriagePopulations());
}

void 
MyMDP::SetCurrentState(const StateSharedPtr state)
{
    auto sCast = std::dynamic_pointer_cast<MyState>(state);
    currentState_ = std::make_shared<MyState>(sCast->GetTriagePopulations());
}

void 
MyMDP::SetDecision(const DecisionSharedPtr decision)
{
    currentDecision_ = std::make_shared<MyDecision>(0,0,0,0);
}

void 
MyMDP::SetPolicy(const PolicySharedPtr& decisionPolicy)
{
    auto pCast = std::dynamic_pointer_cast<CriticalFirstPolicy>(decisionPolicy);

    decisionPolicy_ = std::make_shared<CriticalFirstPolicy>(*pCast);
}
