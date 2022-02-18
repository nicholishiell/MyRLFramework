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

#include "AVILearner.h"

using namespace Util;
using namespace MyUtil;

void MyParamterUpdater(const int nIter, double& alpha, double& epsilon)
{
    // if(nIter % 1000 == 0)
    // {  
    //     alpha = 0.95*alpha;
    //     epsilon = 0.9*epsilon;

    //     if(alpha < 0.1) alpha = 0.1;
    //     if(epsilon < 0.01) epsilon = 0.01;
    // }

    // if(nIter == 0 ) alpha = 1.;
    // else alpha = 1./pow(static_cast<double>(nIter), 0.1);
    double a = 1e6;

    alpha = a/(a+nIter-1);

}

void MyParamterUpdaterEmpty(const int nIter, double& alpha, double& epsilon)
{
}

int main()
{ 
    int numberOfTestRuns = 1;

    MyMDP mdp;
    mdp.SetGamma(1.);

    MyStateSharedPtr initialState = std::make_shared<MyState>(900,40,30,30);  
    mdp.SetInitialState(initialState);

    AVILearner< PostDecisionState, 
                PostDecisionStateHash, PostDecisionStateEqual,
                MyUtil::StateHash, MyUtil::StateEqual> aviLearner(0.1, 0.1);


    if(true)
    {
        aviLearner.SetDefaultValue(0.);
        //aviLearner.Learn(mdp, 10000000, *MyParamterUpdater);
        aviLearner.Learn(mdp, 5000000, *MyParamterUpdaterEmpty);
    }

    if(true)
    {
        const auto defaultDecision = std::make_shared<MyDecision>(0,0,0,0);
        auto learnedPolicy = aviLearner.GetLearnedPolicy();
        learnedPolicy->SetDefaultDecision(defaultDecision);

        //learnedPolicy->WriteToFile("./learnedPolicy.dat");

        mdp.SetPolicy(learnedPolicy);
        
        std::cout << "Learned Policy: " << std::endl;
        for(int iRun = 0; iRun < numberOfTestRuns; iRun++)
        {
            learnedPolicy->ResetDefaultDecisionUsed();
            mdp.Reset();
            const auto traj = mdp.RunEpisode();

            for(const auto step : traj)
            {
                std::cout << step.s->ToString() << " : " << step.d->ToString() << " " << step.r << std::endl;
            }

            std::cout << "Total Reward: " << mdp.GetCumulativeReward() << std::endl;
            std::cin.get();
        }
    }

    if(true)
    {
        GreenFirstPolicySharedPtr greenFirstPolicy = std::make_shared<GreenFirstPolicy>();
        mdp.SetPolicy(greenFirstPolicy);

        std::cout << "Green First Policy: " << std::endl;
        for(int iRun = 0; iRun < numberOfTestRuns; iRun++)
        {
            mdp.Reset();
            const auto traj = mdp.RunEpisode();

            for(const auto step : traj)
            {
                std::cout << step.s->ToString() << " : " << step.d->ToString() << " " << step.r << std::endl;
            }

            std::cout << "Total Reward: " << mdp.GetCumulativeReward() << std::endl;
            std::cin.get();
        }
    }

    return 0;
}

