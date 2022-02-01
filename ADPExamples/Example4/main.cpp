#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

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

int main()
{ 
    using namespace std::chrono;

    MyMDP mdp;
    mdp.SetGamma(1.);

    int initPop = 2000;
    MyStateSharedPtr initialState = std::make_shared<MyState>(initPop,0,0,0);  
    mdp.SetInitialState(initialState);

    if(true)
    {
        QLearner<StateDecisionPair, StateDecisionHash> qLearner(1., 0.75);

        qLearner.Learn(mdp, 1001, *MyParamterUpdater);

        QTablePolicySharedPtr qPolicy = std::make_shared<QTablePolicy>( mdp.GetFullDecisionSpace(),
                                                                        qLearner.GetLearnedQTable());

        mdp.SetPolicy(qPolicy);
        
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        std::cout << "Learned Policy: " << std::endl;
        for(int iRun = 0; iRun < 1000; iRun++)
        {
            mdp.Reset();
            mdp.RunEpisode();
            //std::cout << mdp.GetCumulativeReward() << std::endl;
        }

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        const auto deltaTime = duration_cast<duration<double>>(t2 - t1);
        std::cout << "QTable size: " << qLearner.GetLearnedQTable()->GetSize() << std::endl;
        std::cout << "deltaTime: " << deltaTime.count() << std::endl;

        std::cout << mdp.GetdeltaTimeGetDecision() << std::endl;
        std::cout << mdp.GetdeltaTimeCalculateContribution() << std::endl;
        std::cout << mdp.GetdeltaTimeUpdateDecision() << std::endl;
        std::cout << mdp.GetdeltaTimeUpdateExogenous() << std::endl;

        std::cin.get();
    }

    if(false)
    {
        CriticalFirstPolicySharedPtr critFirstPolicy = std::make_shared<CriticalFirstPolicy>();
        mdp.SetPolicy(critFirstPolicy);

        std::cout << "Critical First: " << std::endl;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for(int iRun = 0; iRun < 1000; iRun++)
        {
            mdp.Reset();
            mdp.RunEpisode();
        // std::cout << "Total Reward: " << mdp.GetCumulativeReward() << std::endl;
        }
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        const auto deltaTime = duration_cast<duration<double>>(t2 - t1);
        std::cout << deltaTime.count() << std::endl;
        std::cin.get();
    }
    return 0;
}

