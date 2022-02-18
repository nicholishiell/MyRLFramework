#include <type_traits>

#include "MyMDP.h"
#include "MyUtil.h"

MyMDP::MyMDP()
{
    transitionProbs_ = {0.1, 0.9}; 
    spaceRequirements_ = {1,2}   ;
    heloCapacity_ = 4;

    initializeDecisionSpace();

    outputCounter_ = 0;
    historyCounter_ = 0;
    
    for(int i = 0; i < 50; i++)
    {
        decisionHistory_[i] = {0.,0.};
        stateHistory_[i] = {0,0.};
    }
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

    for(uShort i = initialPopulation[0]; i >= 0; i--)
    {
        for(uShort j = maxPopulation-i; j >= 0; j--)
        {
            ShortArray2d pop = {i, j};

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
    return CalculateContribution(currentState_, currentDecision_);
}

double 
MyMDP::CalculateContribution(   const StateSharedPtr state, 
                                const DecisionSharedPtr decision) const
{
    const auto dCast = std::dynamic_pointer_cast<MyDecision>(decision);

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

    // This update calculates the expected number of transitions from 
    // each state, as oppose to sampling the distribution
    
    // for(int iTriage = 0; iTriage < population.size(); iTriage++) 
    // {
    //     exogenousInfo[iTriage] = population[iTriage]*transitionProbs_[iTriage];

    //     if(population[iTriage] == 1) exogenousInfo[iTriage] = 1;

    //     if(population[iTriage] == 0) exogenousInfo[iTriage] = 0;
    // }

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
    else if(policyType == typeid(LUTPolicy<MyUtil::StateHash, MyUtil::StateEqual>).name())
    {
        auto pCast = std::dynamic_pointer_cast<LUTPolicy<MyUtil::StateHash, MyUtil::StateEqual>>(decisionPolicy);   
        decisionPolicy_ = std::make_shared<LUTPolicy<MyUtil::StateHash, MyUtil::StateEqual>>(*pCast);
    }
    else
    {
        std::cout << "[WARNING] Unknown policy type: " << policyType << std::endl;
    }
}

void 
MyMDP::RecordLearning(const Step& s)
{   
    return;
    const auto sArray = std::dynamic_pointer_cast<MyState>(s.s)->GetTriagePopulations();
    
    const auto isEndState = sArray[0] == 0 &&
                            sArray[1] == 0;
    if(!isEndState)
    {
        const auto dArray = std::dynamic_pointer_cast<MyDecision>(s.d)->GetDecisionArray();
        const auto sArray = std::dynamic_pointer_cast<MyState>(s.s)->GetTriagePopulations();

        for(int i = 0; i < dArray.size(); i++)
        {
            decisionHistory_[historyCounter_][i] += static_cast<double>(dArray[i]);
            stateHistory_[historyCounter_][i] += static_cast<double>(sArray[i]);
        }

        historyCounter_++;
    }
    else
    {
        historyCounter_ = 0;
        outputCounter_++;

        if(outputCounter_ % 100 == 0 && outputCounter_ > 0)
        {
            std::ofstream out;
            out.open("./history.dat", std::fstream::trunc);
            
            for(int i = 0; i < 150; i++)
            {
                const auto d = decisionHistory_[i];
                const auto s = stateHistory_[i];

                out << i << " ";
                for(int j = 0; j < d.size(); j++)
                {
                    out << d[j] / 100.<< " ";
                }
                for(int j = 0; j < d.size(); j++)
                {
                    out << (s[j]+d[j]) / 100.<< " ";
                }
                out << std::endl;

                decisionHistory_[i] = {0.,0.};
                stateHistory_[i] = {0.,0.};
            }
            out << std::endl;
            out.close();
            outputCounter_ = 0;
        }
    }

}