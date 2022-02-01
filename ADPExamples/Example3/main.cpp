#include <iostream>
#include <fstream>

#include "MyMDP.h"
#include "MyValueFunction.h"
#include "MyState.h"
#include "MyDecision.h"
#include "MyPolicy.h"

#include "QLearner.h"

using namespace Util;

void MyParamterUpdater(const int nIter, double& alpha, double& epsilon)
{
    if(nIter % 10000 == 0)
    {
        alpha = 0.9*alpha;

        if(alpha < 0.0001)
        {
            alpha = 0.0001;
            
            epsilon = 0.9*epsilon;
            if(epsilon < 0.1) epsilon = 0.1;
        }
    }
}

int GetDecisionCode(const IntArray2d& a)
{
    int retVal = -1;

    if(a[0] == 0 && a[1] == 0) retVal = 0;
    else if(a[0] == 1 && a[1] == 0) retVal = 1;
    else if(a[0] == 2 && a[1] == 0) retVal = 2;
    else retVal = 3;

    return retVal;
}

int main()
{ 
    MyMDP mdp;
    mdp.SetGamma(1.);

    int initPop = 1000;
    MyStateSharedPtr initialState = std::make_shared<MyState>(initPop,0);  
    mdp.SetInitialState(initialState);

    QLearner<StateDecisionPair, StateDecisionHash> qLearner(1., 0.75);

    qLearner.Learn(mdp, 1000000, *MyParamterUpdater);

    QTablePolicySharedPtr qPolicy = std::make_shared<QTablePolicy>(mdp.GetFullDecisionSpace(), qLearner.GetLearnedQTable());

    std::ofstream out;
    out.open("./policy.dat");
    
    for(int i = 0; i < initPop; i++)
    {
        for(int j = 0; j < initPop; j++)
        {
            MyStateSharedPtr s = std::make_shared<MyState>(i,j);
            const auto d = (*qPolicy)(s);

            const auto dArray = std::dynamic_pointer_cast<MyDecision>(d)->GetDecisionArray();

            out << i << " " << j << " " << GetDecisionCode(dArray) << std::endl;
        }
    }

    mdp.SetPolicy(qPolicy);

    std::cout << "Learned Policy: " << std::endl;
    for(int iRun = 0; iRun < 25; iRun++)
    {
        mdp.Reset();
        mdp.RunEpisode();
        std::cout << mdp.GetCumulativeReward() << std::endl;
    }

    CriticalFirstPolicySharedPtr critFirstPolicy = std::make_shared<CriticalFirstPolicy>();
    mdp.SetPolicy(critFirstPolicy);

    std::cout << "Critical First: " << std::endl;
    for(int iRun = 0; iRun < 25; iRun++)
    {
        mdp.Reset();
        mdp.RunEpisode();
        std::cout << mdp.GetCumulativeReward() << std::endl;
    }

    return 0;
}

