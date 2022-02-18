#include <type_traits>

#include "MyMDP.h"

using namespace MyUtil;

MyMDP::MyMDP()
{
    //transitionProbs_ = {0.05, 0.1, 0.25, 0.9}; 
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
            for(int k = 0; k <= heloCapacity_; k++)
            {
                for(int l = 0; l <= heloCapacity_; l++)
                {
                    const auto spaceRequired =  i*spaceRequirements_[0] + 
                                                j*spaceRequirements_[1] +
                                                k*spaceRequirements_[2] +
                                                l*spaceRequirements_[3];

                    if(spaceRequired <= heloCapacity_)
                    {
                        fullDecisionSpace_.emplace_back(std::make_shared<MyDecision>(i,j,k,l));
                    }
                }
            }
        }
    }
}

void
MyMDP::initializeStateSpace()
{
    return;

    auto sCast = std::dynamic_pointer_cast<MyState>(initialState_);

    const auto initialPopulation = sCast->GetTriagePopulations();

    const auto maxPopulation = SumArray(initialPopulation);

    int counter = 0;

    for(uShort i = initialPopulation[0]; i >= 0; i--)
    {
        for(uShort j = maxPopulation-i; j >= 0; j--)
        {
            for(uShort k = maxPopulation-i-j; k >= 0; k--)
            {
                for(uShort l = maxPopulation-i-j-k; l >= 0; l--)
                {
                    ShortArray4d pop = {i, j, k, l};

                    if(SumArray(pop)<=maxPopulation && SumArray(pop) >= 0)
                    {
                        stateSpace_.emplace_back(std::make_shared<MyState>(pop));
                    }
                }
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

    IntArray4d exogenousInfo = {0,0,0,0};

    // Generate the exogenous information
    // for(int iTriage = 0; iTriage < population.size(); iTriage++) 
    // {
    //     for(int iPerson = 0; iPerson < population[iTriage]; iPerson++)
    //     {
    //         if(randomNumber(gen_) < transitionProbs_[iTriage])
    //         {
    //             exogenousInfo[iTriage]++;
    //         }
    //     }
    // }

    // This update calculates the expected number of transitions from 
    // each state, as oppose to sampling the distribution
    
    for(int iTriage = 0; iTriage < population.size(); iTriage++) 
    {
        exogenousInfo[iTriage] = population[iTriage]*transitionProbs_[iTriage];

        if(population[iTriage] == 1) exogenousInfo[iTriage] = 1;

        if(population[iTriage] == 0) exogenousInfo[iTriage] = 0;
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
    
    // Check to make sure there are enough people at the IS
    for(int i = 0; i < dArray.size(); i++)
    {
        if(dArray[i] > sArray[i])
        {
            return false;
        }
    }

    // Check to make sure the helicopter is as full as possible
    int decisionCap = 0;
    IntArray4d sArrayPrime = {0,0,0,0};

    for(int i = 0; i < dArray.size(); i++)
    {
        decisionCap += spaceRequirements_[i]*dArray[i];

        sArrayPrime[i] = sArray[i] - dArray[i];
    }

    if(decisionCap != heloCapacity_)
    {
        int deltaCap = heloCapacity_ - decisionCap;

        for(int i = 0; i < sArrayPrime.size(); i++)
        {
            if(sArrayPrime[i] > 0)
            {
                if(deltaCap >= spaceRequirements_[i])
                {
                    return false;
                }
            }
        }
    }

    return true;
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
    else if(policyType == typeid(GreenFirstPolicy).name())
    {
        auto pCast = std::dynamic_pointer_cast<GreenFirstPolicy>(decisionPolicy);   
        decisionPolicy_ = std::make_shared<GreenFirstPolicy>(*pCast);
    }
    else if(policyType == typeid(LUTPolicy<MyUtil::StateHash, MyUtil::StateEqual>).name())
    {
        auto pCast = std::dynamic_pointer_cast<LUTPolicy<MyUtil::StateHash, MyUtil::StateEqual>>(decisionPolicy);   
        decisionPolicy_ = std::make_shared<LUTPolicy<MyUtil::StateHash, MyUtil::StateEqual>>(*pCast);
    }
    else if(policyType == typeid(RandomPolicy).name())
    {
        auto pCast = std::dynamic_pointer_cast<RandomPolicy>(decisionPolicy);   
        decisionPolicy_ = std::make_shared<RandomPolicy>(*pCast);
    }
    else
    {
        std::cout << "[WARNING] Unknown policy type: " << policyType << std::endl;
    }
}
