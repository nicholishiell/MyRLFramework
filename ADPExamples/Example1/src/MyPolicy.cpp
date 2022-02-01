#include "MyPolicy.h"
#include "MyState.h"
#include "MyDecision.h"

using namespace Util;

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

    IntArray4d decisionArray = {0,0,0,0};

    int CRITICAL_SPACE = 3;
    int NONCRITICAL_SPACE = 1;
    int TOTAL_SPACE = 10;
    int remainingSpace = TOTAL_SPACE;

    while(remainingSpace > 0 && SumArray(populationArray) > 0)
    {
        if(populationArray[3] > 0)
        {
            populationArray[3]--;
            decisionArray[3]++;

            remainingSpace -= CRITICAL_SPACE;
        }
        else if(populationArray[2] > 0)
        {
            populationArray[2]--;
            decisionArray[2]++;

            remainingSpace -= CRITICAL_SPACE;  
        }
        else if(populationArray[1] > 0)
        {
            populationArray[1]--;
            decisionArray[1]++;

            remainingSpace -= NONCRITICAL_SPACE;  
        }
        else if(populationArray[0] > 0)
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