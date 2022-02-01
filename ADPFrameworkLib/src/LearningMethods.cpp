

#include <map>
#include <chrono>
#include <algorithm>

#include "LearningMethods.h"
#include "ValueFunction.h"

namespace LearningMethods
{
/*    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Action 
    GetRandomAction(const State& s, ActionSpace legalActionSpace, std::mt19937& gen)
    {
        std::random_shuffle(legalActionSpace.begin(),legalActionSpace.end());
        return legalActionSpace[0];
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
    // DecisionSharedPtr 
    // GetEpsilonGreedyAction( const StateSharedPtr state, 
    //                         const PolicySharedPtr decisionPolicy, 
    //                         DecisionSpace legalDecisionSpace, 
    //                         const double epsilon, 
    //                         std::mt19937& gen)
    // {
    //     DecisionSharedPtr d;

    //     std::uniform_real_distribution<double> dist(0.0,1.0);
    //     const double randomNumber = dist(gen);

    //     if(randomNumber < epsilon)
    //     {
    //         std::random_shuffle(legalDecisionSpace.begin(),legalDecisionSpace.end());
    //         d = legalDecisionSpace[0];
    //     }
    //     else
    //     {
    //         d = (*decisionPolicy)(state);
    //     }

    //     return d;
    // }

    // // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // template<class KEY, class HASH>
    // double MaxDecision( const LUTValueFunction<KEY, HASH>& vf, 
    //                     const StateSharedPtr state, 
    //                     DecisionSpace legalDecisionSpace)
    // {
    //     // TODO: Define a large number in Util
    //     double highestValue = -1.e9;
        
    //     for(const auto d : legalDecisionSpace)
    //     {
    //         LUTKey k(state, d);

    //         if(vf(k) > highestValue)
    //         {
    //             highestValue = vf(k);
    //         }
    //     }

    //     return highestValue;
    // }

    // // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // template<class KEY, class HASH>
    // void InitializeQTable(  LUTValueFunction<KEY, HASH>& vf,
    //                         const MarkovDecisionProcess& mdp,
    //                         std::mt19937& gen)
    // {
    //     const auto stateSpace = mdp.GetStateSpace();

    //     for(const auto& s : stateSpace)
    //     {
    //         const auto decisionSpace = mdp.GetLegalDecisions(s);

    //         for(const auto& d : decisionSpace)
    //         {
    //             LUTKey k (s,d);
    //             vf.SetValue(k, 0.);
    //         }
    //     }
    // }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   /* template<class KEY, class HASH>
    void QLearning(MarkovDecisionProcess& mdp, const int numberOfIterations)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);

        int iterCounter = 0;
          
        // Algorithm parameters (initial values)
        double alpha = 1.; // learning rate
        double epsilon = 0.75; // greedy
       
        LUTValueFunction<KEY, HASH> sdvf;

        // Arbitrarily define the Q table
        InitializeQTable<KEY, HASH>(sdvf, mdp);

        VFAPolicy<KEY> qTablePolicy(sdvf);

        // Start Q-Learning
        for(uint iRun = 0; iRun < numberOfIterations; iRun++)
        {   
            mdp.SetPolicy(qTablePolicy);
            mdp.Reset();
            
            // Adjust the alpha and epsilon values to refine the search
            if(iRun % 10000 == 0)
            {
                alpha = 0.9*alpha;

                if(alpha < 0.0001)
                {
                    alpha = 0.0001;
                    
                    epsilon = 0.9*epsilon;
                    if(epsilon < 0.1) epsilon = 0.1;
                }
            }

            // Generate a trajectory through the MDP
            while(!mdp.IsEndState())
            {               
                // ToDo: this function should just take in the mdp to shorten the arg list
                DecisionSharedPtr d = GetEpsilonGreedyAction(   mdp.GetCurrentState(),
                                                                mdp.GetDecisionPolicy(), 
                                                                mdp.GetLegalDecisions(),
                                                                epsilon, 
                                                                gen);

                // Create the state-decision pair object
                LUTKey k(mdp.GetCurrentState(),d); // Q(s,a)

                // Update the internal state of the MDP
                mdp.SetDecision(d); 
                mdp.UpdateDecision();
                mdp.UpdateExogenous();

                // Get the reward form the MDP
                const auto reward = mdp.CalculateContribution();

                // update the q table  for the sd-pair previously defined
                const auto md = MaxDecision(sdvf,mdp.GetCurrentState(), mdp.GetLegalDecisions());
                double sdvfUpdate = sdvf(k) + alpha*(reward + mdp.GetGamma()*md - sdvf(k));
                sdvf.SetValue(k,sdvfUpdate);               
            }       
        }
    }*/

};
