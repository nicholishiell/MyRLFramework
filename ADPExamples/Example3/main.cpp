#include <iostream>
#include <fstream>

#include "MyMDP.h"
#include "MyValueFunction.h"
#include "MyState.h"
#include "MyDecision.h"
#include "MyPolicy.h"
#include "MyUtil.h"

#include "QLearner.h"

using namespace Util;

void MyParamterUpdater(const int nIter, double& alpha, double& epsilon)
{
    // if(nIter % 10000 == 0)
    // {
    //     alpha = 0.9*alpha;

    //     if(alpha < 0.0001)
    //     {
    //         alpha = 0.0001;
            
    //         epsilon = 0.9*epsilon;
    //         if(epsilon < 0.1) epsilon = 0.1;
    //     }
    // }

    // double a = 1e5;
    // alpha = a / (a + static_cast<double>(nIter) - 1.);
}

int GetDecisionCode(const IntArray2d& a)
{
    int retVal = -1;

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
            const auto dArray = std::dynamic_pointer_cast<MyDecision>(d)->GetDecisionArray();

            out << i << " " << j << " " << GetDecisionCode(dArray) << std::endl;
        }
    }
}

int main()
{ 
    MyMDP mdp;
    mdp.SetGamma(1.);
    
    int initPop = 100;
    MyStateSharedPtr initialState = std::make_shared<MyState>(initPop,0);  
    mdp.SetInitialState(initialState);
    
    QLearner<   StateDecisionPair, 
                StateDecisionHash, StateDecisionEqual,
                StateHash, StateEqual> qLearner(0.05, 0.1);  
                                                            
    qLearner.Learn(mdp, 2000000, *MyParamterUpdater);

    QTablePolicySharedPtr qPolicy = std::make_shared<QTablePolicy>(mdp.GetFullDecisionSpace(), qLearner.GetLearnedQTable());
   
    const auto learnedPolicy = qLearner.GetLearnedPolicy();
    MyDecisionSharedPtr defaultDecision = std::make_shared<MyDecision>(0,0);
    learnedPolicy->SetDefaultDecision(defaultDecision);
    
    WritePolicyToFile(initPop, learnedPolicy,"learnedPolicy.dat");
    WritePolicyToFile(initPop, qPolicy,"qPolicy.dat");

    for(int iRun = 0; iRun < 100; iRun++)
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

