#include <iostream>

#include "MyState.h"
#include "MyDecision.h"
#include "MyMDP.h"
#include "MyPolicy.h"

int main()
{
    MyStateSharedPtr initialState = std::make_shared<MyState>(25, 10);
    MyPolicySharedPtr policy = std::make_shared<CriticalFirstPolicy>();
    MyDecisionSharedPtr decision = std::make_shared<MyDecision>(0);

    MyMDP mdp;

    mdp.SetInitialState(initialState);
    mdp.SetCurrentState(initialState);
    mdp.SetDecision(decision);
    mdp.SetPolicy(policy);

    mdp.RunEpisode();

    return 0;
}