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

    ShortArray2d decisionArray = {0,0};

    int CRITICAL_SPACE = 2;
    int NONCRITICAL_SPACE = 1;
    int TOTAL_SPACE = 2;
    int remainingSpace = TOTAL_SPACE;

    // Continue whilst there is space on the helo and people at the evac site
    while(remainingSpace > 0 && SumArray(populationArray) > 0)
    {
        if(populationArray[1] > 0 && remainingSpace >= CRITICAL_SPACE)
        {
            populationArray[1]--;
            decisionArray[1]++;

            remainingSpace -= CRITICAL_SPACE;
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