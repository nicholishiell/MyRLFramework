#include <iostream>
#include <fstream>

#include "MyMDP.h"
#include "MyValueFunction.h"
#include "MyState.h"
#include "MyDecision.h"
#include "MyPolicy.h"
#include "MyUtil.h"

#include "AVILearner.h"

using namespace Util;

void MyParamterUpdater(const int nIter, double& alpha, double& epsilon)
{
    double a = 5e6;
    alpha = a / (a + static_cast<double>(nIter) - 1.);
}

int GetDecisionCode(const ShortArray2d& a)
{
    int retVal = -1;

    // if(a[0] == 0 && a[1] == 0) retVal = 0;
    // else if(a[0] == 0 && a[1] == 1) retVal = 1;
    // else if(a[0] == 1 && a[1] == 0) retVal = 2;
    // else if(a[0] == 2 && a[1] == 0) retVal = 3;
    // else retVal = -1;

    if(a[0] == 0 && a[1] == 0) retVal = 0;
    else if(a[0] == 0 && a[1] == 1) retVal = 1;
    else if(a[0] == 0 && a[1] == 2) retVal = 2;
    else if(a[0] == 1 && a[1] == 1) retVal = 3;
    else if(a[0] == 1 && a[1] == 0) retVal = 4;
    else if(a[0] == 2 && a[1] == 1) retVal = 5;
    else if(a[0] == 2 && a[1] == 0) retVal = 6;
    else if(a[0] == 3 && a[1] == 0) retVal = 7;
    else if(a[0] == 4 && a[1] == 0) retVal = 8;
    else retVal = -1;

    return retVal;
}

void WritePolicyToFile(const int initPop, const PolicySharedPtr policy, const std::string filename)
{
    std::ofstream out;
    out.open(filename.c_str());
    
    for(int i = 0; i < initPop; i++)
    {
        for(int j = 0; j < initPop; j++)
        {
            MyStateSharedPtr s = std::make_shared<MyState>(i,j);

            const auto d = (*policy)(s);
            int value = -1;
            if(d != nullptr)
            {
                const auto dArray = std::dynamic_pointer_cast<MyDecision>(d)->GetDecisionArray();
                value = GetDecisionCode(dArray);
            }
            out << i << " " << j << " " <<  value << std::endl;
        }
    }
}

int main()
{ 
    int numberOfTestRuns = 25;

    MyMDP mdp;
    mdp.SetGamma(1.);

    int initPop = 100;
    MyStateSharedPtr initialState = std::make_shared<MyState>(initPop,0);  
    mdp.SetInitialState(initialState);

    for(const auto d : mdp.GetFullDecisionSpace())
    {
        std::cout << d->ToString() << std::endl;
    }

    AVILearner< PostDecisionState, 
                PostDecisionStateHash, PostDecisionStateEqual,
                MyUtil::StateHash, MyUtil::StateEqual> aviLearner(1., 0.05);

    aviLearner.SetDefaultValue(0.);
    aviLearner.Learn(mdp, 500000000, *MyParamterUpdater);
    
    const auto learnedPolicy = aviLearner.GetLearnedPolicy();
   
    WritePolicyToFile(initPop, learnedPolicy, "learnedPolicy.dat");
   
    mdp.SetPolicy(learnedPolicy);
    for(int iRun = 0; iRun < numberOfTestRuns; iRun++)
    {
        mdp.Reset();
        const auto trajectory = mdp.RunEpisode();

        for(const auto& step : trajectory )
        {
            std::cout << step.s->ToString() << "\t" << step.d->ToString() << "\t" << step.r << std::endl;
        }

        std::cout << "Total Reward: " << mdp.GetCumulativeReward() << std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

        std::cin.get();
    }


    return 0;
}

