#include <algorithm>

#include "MyPolicy.h"
#include "MyState.h"
#include "MyDecision.h"
#include "MyUtil.h"

using namespace Util;
using namespace MyUtil;

CriticalFirstPolicy::CriticalFirstPolicy()
{

}

CriticalFirstPolicy::~CriticalFirstPolicy()
{

}

DecisionSharedPtr 
CriticalFirstPolicy::operator() (const StateSharedPtr s) const
{
    auto sCast = std::dynamic_pointer_cast<MyState>(s);
    auto populationArray = sCast->GetTriagePopulations();

    ShortArray4d decisionArray = {0,0,0,0};

    uShort remainingSpace = heloCapacity_;

    // Continue whilst there is space on the helo and people at the evac site
    while(remainingSpace > 0 && SumArray(populationArray) > 0)
    {
        if(populationArray[3] > 0 && remainingSpace >= CRITICAL_SPACE)
        {
            populationArray[3]--;
            decisionArray[3]++;

            remainingSpace -= CRITICAL_SPACE;
        }
        else if(populationArray[2] > 0 && remainingSpace >= CRITICAL_SPACE)
        {
            populationArray[2]--;
            decisionArray[2]++;

            remainingSpace -= CRITICAL_SPACE;
        }
        else if(populationArray[1] > 0 && remainingSpace >= NONCRITICAL_SPACE)
        {
            populationArray[1]--;
            decisionArray[1]++;

            remainingSpace -= NONCRITICAL_SPACE;
        }
        else if(populationArray[0] > 0 && remainingSpace >= NONCRITICAL_SPACE)
        {
            populationArray[0]--;
            decisionArray[0]++;

            remainingSpace -= NONCRITICAL_SPACE;  
        }
        else
        {
            // Should NOT get here
            break;
        }
    }

    return std::make_shared<MyDecision>(decisionArray);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

GreenFirstPolicy::GreenFirstPolicy()
{

}

GreenFirstPolicy::~GreenFirstPolicy()
{

}

DecisionSharedPtr 
GreenFirstPolicy::operator() (const StateSharedPtr s) const
{
    auto sCast = std::dynamic_pointer_cast<MyState>(s);
    auto populationArray = sCast->GetTriagePopulations();

    ShortArray4d decisionArray = {0,0,0,0};
    uShort remainingSpace = heloCapacity_;

    // Continue whilst there is space on the helo and people at the evac site
    while(remainingSpace > 0 && SumArray(populationArray) > 0)
    {
        if(populationArray[1] > 0 && remainingSpace >= NONCRITICAL_SPACE)
        {
            populationArray[1]--;
            decisionArray[1]++;

            remainingSpace -= NONCRITICAL_SPACE;
        }
        else if(populationArray[0] > 0 && remainingSpace >= NONCRITICAL_SPACE)
        {
            populationArray[0]--;
            decisionArray[0]++;

            remainingSpace -= NONCRITICAL_SPACE;  
        }
        else if(populationArray[2] > 0 && remainingSpace >= CRITICAL_SPACE)
        {
            populationArray[2]--;
            decisionArray[2]++;

            remainingSpace -= CRITICAL_SPACE;
        }
        else if(populationArray[3] > 0 && remainingSpace >= CRITICAL_SPACE)
        {
            populationArray[3]--;
            decisionArray[3]++;

            remainingSpace -= CRITICAL_SPACE;
        }
        else
        {
            // Should NOT get here
            break;
        }
    }

    return std::make_shared<MyDecision>(decisionArray);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

RandomPolicy::RandomPolicy()
{}

RandomPolicy::~RandomPolicy()
{}

void
RandomPolicy::SetDecisionSpace(const DecisionSpace ds)
{
    fullDecisionSpace_ = ds;
}

DecisionSharedPtr 
RandomPolicy::operator() (const StateSharedPtr s) const
{
    DecisionSpace stateDecisionSpace;

    for(const auto d : fullDecisionSpace_)
    {
        if(s->IsDecisionLegal(d))
        {
            stateDecisionSpace.emplace_back(d);
        }
    }

    std::random_shuffle(stateDecisionSpace.begin(),
                        stateDecisionSpace.end());

    return stateDecisionSpace[0];
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

QTablePolicy::QTablePolicy(const DecisionSpace& ds, const QTableSharedPtr vf)
{
    fullDecisionSpace_ = ds;
    SetValueFunction(vf);
}

QTablePolicy::~QTablePolicy()
{

}
    
DecisionSharedPtr 
QTablePolicy::operator() (const StateSharedPtr s) const
{
    MyDecisionSharedPtr decision = nullptr;

    const auto sCast = std::dynamic_pointer_cast<MyState>(s);
    
    double highestValue = VERY_NEGATIVE_NUMBER;

    // Check the QTable for the action with the highest value
    for(const auto& d : fullDecisionSpace_)
    {    

        const auto dCast = std::dynamic_pointer_cast<MyDecision>(d);
        StateDecisionPair k(sCast,dCast); 

        if(GetValue(k) > highestValue)
        {
            highestValue = GetValue(k);
            decision = dCast;
        }
    }

    // If no entry has been found then randomly choose one 
    // decision from the set of all legal decisions
    if(decision == nullptr)
    {
        DecisionSpace ds;
        for(const auto& d : fullDecisionSpace_)
        {    
            if(sCast->IsDecisionLegal(d))
            {
                ds.emplace_back(d);
            }
        }

        std::random_shuffle(ds.begin(),ds.end());
        decision =  std::dynamic_pointer_cast<MyDecision>(ds[0]);
    }

    return decision;
}