#include <iostream>

#include "MyState.h"
#include "MyPolicy.h"
#include "MyDecision.h"
#include "MyMDP.h"

using namespace Util;

int main()
{
    MyStateSharedPtr initialState = std::make_shared<MyState>(100,0,0,0);
    MyPolicySharedPtr decisionPolicy = std::make_shared<CriticalFirstPolicy>();
    MyDecisionSharedPtr decision = std::make_shared<MyDecision>(0,0,0,0);

    MyMDP mdp;   
    
    mdp.SetInitialState(initialState);
    mdp.SetCurrentState(initialState);
    mdp.SetPolicy(decisionPolicy);
    mdp.SetDecision(decision);

    for(int i = 0; i < 100; i++)
    {
        std::cout << "Running Episode #" << i << std::endl;

        const auto trajectory = mdp.RunEpisode();
    
        for(const auto& step : trajectory)
        {
            std::cout << step.s->ToString() << "\t" << step.d->ToString() << "\t" << step.r << std::endl;
        }

        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        
        mdp.Reset();
    }

    return 0;
}

