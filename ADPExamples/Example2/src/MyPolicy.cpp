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
    int decision;

    auto sCast = std::dynamic_pointer_cast<MyState>(s);

    // Also take a job from the longer job
    if(sCast->GetQueuePopulations(0) > sCast->GetQueuePopulations(1))
    {
        decision = 0;
    }   
    else
    {
        decision = 1;
    }

    return std::make_shared<MyDecision>(decision);
}