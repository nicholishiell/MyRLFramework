#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

#include "MyMDP.h"
#include "MyValueFunction.h"
#include "MyState.h"
#include "MyDecision.h"
#include "MyPolicy.h"
#include "MyUtil.h"

#include "QLearner.h"

using namespace Util;
using namespace MyUtil;

void MyParamterUpdater(const int nIter, double& alpha, double& epsilon)
{
    double a = 1e5;
    alpha = a / (a + static_cast<double>(nIter) - 1.);
}

void MyParamterUpdaterEmpty(const int nIter, double& alpha, double& epsilon)
{
}

int main()
{ 
    using namespace std::chrono;

    MyMDP mdp;
    mdp.SetGamma(1.);

    //MyStateSharedPtr initialState = std::make_shared<MyState>(650,40,30,30);  
    MyStateSharedPtr initialState = std::make_shared<MyState>(1900,40,30,30);  
    mdp.SetInitialState(initialState);

    MyDecisionSharedPtr defaultDecision = std::make_shared<MyDecision>(0,0,0,0);  

    if(true)
    {
        QLearner<   StateDecisionPair, 
                    StateDecisionHash, StateDecisionEqual,
                    MyUtil::StateHash, MyUtil::StateEqual> qLearner(1., 0.05);

        qLearner.Learn(mdp, 500000, *MyParamterUpdater);

        QTablePolicySharedPtr qPolicy = std::make_shared<QTablePolicy>( mdp.GetFullDecisionSpace(),
                                                                        qLearner.GetLearnedQTable());

        auto learnedPolicy = qLearner.GetLearnedPolicy();
        learnedPolicy->SetDefaultDecision(defaultDecision);

        learnedPolicy->WriteToFile("./learnedPolicy.dat");

        mdp.SetPolicy(learnedPolicy);
        
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        std::cout << "Learned Policy: " << std::endl;
        for(int iRun = 0; iRun < 100; iRun++)
        {
            learnedPolicy->ResetDefaultDecisionUsed();
            mdp.Reset();
            mdp.RunEpisode();
            std::cout << "Total Reward: " << mdp.GetCumulativeReward() << std::endl;
        }
    }

    if(true)
    {
        CriticalFirstPolicySharedPtr critFirstPolicy = std::make_shared<CriticalFirstPolicy>();
        mdp.SetPolicy(critFirstPolicy);

        std::cout << "Critical First Policy: " << std::endl;
        for(int iRun = 0; iRun < 100; iRun++)
        {
            mdp.Reset();
            mdp.RunEpisode();
            std::cout << "Total Reward: " << mdp.GetCumulativeReward() << std::endl;
        }
    }

    if(true)
    {
        GreenFirstPolicySharedPtr greenFirstPolicy = std::make_shared<GreenFirstPolicy>();
        mdp.SetPolicy(greenFirstPolicy);

        std::cout << "Green First Policy: " << std::endl;
        for(int iRun = 0; iRun < 100; iRun++)
        {
            mdp.Reset();
            mdp.RunEpisode();
            std::cout << "Total Reward: " << mdp.GetCumulativeReward() << std::endl;
        }
    }

    if(true)
    {
        RandomPolicySharedPtr randomPolicy = std::make_shared<RandomPolicy>();
        randomPolicy->SetDecisionSpace(mdp.GetFullDecisionSpace());
        mdp.SetPolicy(randomPolicy);

        std::cout << "Random Policy: " << std::endl;
        for(int iRun = 0; iRun < 100; iRun++)
        {
            mdp.Reset();
            mdp.RunEpisode();
            std::cout << "Total Reward: " << mdp.GetCumulativeReward() << std::endl;
        }
    }

    return 0;
}

