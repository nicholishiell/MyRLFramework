#include <type_traits>

#include "MyMDP.h"

MyMDP::MyMDP()
{
    transitionProbs_ = {0.1, 0.9}; 
    spaceRequirements_ = {1,2}   ;
    heloCapacity_ = 4;

    initializeDecisionSpace();
}

MyMDP::~MyMDP()
{
    
}

void 
MyMDP::initializeDecisionSpace()
{
    // Fill the decision space
    for(int i = 0; i <= heloCapacity_; i++)
    {
        for(int j = 0; j <= heloCapacity_; j++)
        {
            const auto spaceRequired =  i*spaceRequirements_[0] + 
                                        j*spaceRequirements_[1];

            if(spaceRequired <= heloCapacity_)
            {
                fullDecisionSpace_.emplace_back(std::make_shared<MyDecision>(i,j));
            }
        }
    }
}

void
MyMDP::initializeStateSpace()
{
    auto sCast = std::dynamic_pointer_cast<MyState>(initialState_);

    const auto initialPopulation = sCast->GetTriagePopulations();

    const auto maxPopulation = SumArray(initialPopulation);

    for(int i = initialPopulation[0]; i >= 0; i--)
    {
        for(int j = maxPopulation-i; j >= 0; j--)
        {
            IntArray2d pop = {i, j};

            if(SumArray(pop)<=maxPopulation && SumArray(pop) >= 0)
            {
                stateSpace_.emplace_back(std::make_shared<MyState>(pop));
            }
        }
    }
}

bool 
MyMDP::IsEndState() const
{
    auto sCast = std::dynamic_pointer_cast<MyState>(currentState_);
    int totalPopulation = 0;

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

    IntArray4d exogenousInfo = {0,0};

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
            if(iTriage < population.size()-1) population[iTriage+1] += exogenousInfo[iTriage];
    }

    // Set the current state to the new population
    currentState_ = std::make_shared<MyState>(population);
}

DecisionSpace 
MyMDP::GetLegalDecisions() const
{
    return GetLegalDecisions(currentState_);
}

DecisionSpace 
MyMDP::GetLegalDecisions(const StateSharedPtr state) const
{
    auto sCast = std::dynamic_pointer_cast<MyState>(state);

    DecisionSpace legalDecisions;

    for(const auto d : fullDecisionSpace_)
    {

        auto dCast = std::dynamic_pointer_cast<MyDecision>(d);
        if(isLegal(dCast,sCast))
        {
            legalDecisions.emplace_back(d);
        }
    }

    return legalDecisions;    
}

const DecisionSpace 
MyMDP::GetFullDecisionSpace() const
{
    return fullDecisionSpace_;
}

bool 
MyMDP::isLegal(const MyDecisionSharedPtr d, const MyStateSharedPtr s) const
{
    const auto dArray = d->GetDecisionArray();
    const auto sArray = s->GetTriagePopulations();
    
    bool retVal = true;

    for(int i = 0; i < dArray.size(); i++)
    {
        if(dArray[i] > sArray[i])
        {
            retVal = false;
            break;
        }
    }

    return retVal;
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
    currentState_ = std::make_shared<MyState>(sCast->GetTriagePopulations());
    
    initializeStateSpace();
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
    currentDecision_ = std::dynamic_pointer_cast<MyDecision>(decision);
}

void 
MyMDP::SetPolicy(const PolicySharedPtr& decisionPolicy)
{
    const auto policyType = typeid(*decisionPolicy).name();

    if(policyType == typeid(CriticalFirstPolicy).name())
    {
        auto pCast = std::dynamic_pointer_cast<CriticalFirstPolicy>(decisionPolicy);   
        decisionPolicy_ = std::make_shared<CriticalFirstPolicy>(*pCast);
    }
    else if(policyType == typeid(QTablePolicy).name())
    {
        auto pCast = std::dynamic_pointer_cast<QTablePolicy>(decisionPolicy);   
        decisionPolicy_ = std::make_shared<QTablePolicy>(*pCast);
    }
    else if(policyType == typeid(VFAPolicy<StateDecisionPair>).name())
    {
        auto pCast = std::dynamic_pointer_cast<VFAPolicy<StateDecisionPair>>(decisionPolicy);   
        decisionPolicy_ = std::make_shared<VFAPolicy<StateDecisionPair>>(*pCast);
    }
    else
    {
        std::cout << "[WARNING] Unknown policy type: " << policyType << std::endl;
    }
}
