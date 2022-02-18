
#ifndef LEARNING_H
#define LEARNING_H

#include <map>
#include <chrono>
#include <algorithm>

#include "Util.h"
#include "State.h"
#include "MDP.h"
#include "ValueFunction.h"

/*
using namespace Util;

namespace LearningMethods
{
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Helper Functions
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    template<class KEY, class TABLE_HASH>
    DecisionSharedPtr 
    GetEpsilonGreedyAction( const StateSharedPtr state, 
                            std::shared_ptr<LUTValueFunction<KEY,TABLE_HASH>> qTable,
                            DecisionSpace legalDecisionSpace, 
                            const double epsilon, 
                            std::mt19937& gen)
    {
        DecisionSharedPtr decision;

        std::uniform_real_distribution<double> dist(0.0,1.0);
        const double randomNumber = dist(gen);

        if(randomNumber < epsilon)
        {
            std::random_shuffle(legalDecisionSpace.begin(),legalDecisionSpace.end());
            decision = legalDecisionSpace[0];
        }
        else
        {
            double highestValue = -1.e9;
            for(const auto& d : legalDecisionSpace)            
            {
                KEY k(state, d);

                if((*qTable)(k) > highestValue)
                {
                    highestValue = (*qTable)(k);
                    decision = d;
                }
            }
        }

        return decision;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    template<class KEY, class TABLE_HASH>
    double MaxDecision( const std::shared_ptr<LUTValueFunction<KEY,TABLE_HASH>> vf, 
                        const StateSharedPtr state, 
                        DecisionSpace legalDecisionSpace)
    {
        // TODO: Define a large number in Util
        double highestValue = -1.e9;
        
        for(const auto d : legalDecisionSpace)
        {
            KEY k(state, d);

            if((*vf)(k) > highestValue)
            {
                highestValue = (*vf)(k);
            }
        }

        return highestValue;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    template<class KEY, class TABLE_HASH>
    void InitializeQTable(  std::shared_ptr<LUTValueFunction<KEY,TABLE_HASH>> vf,
                            const MarkovDecisionProcess& mdp,
                            std::mt19937& gen)
    {
        const auto stateSpace = mdp.GetStateSpace();

        std::uniform_real_distribution<double> dist(0.,1.);

        for(const auto& s : stateSpace)
        {
            const auto decisionSpace = mdp.GetLegalDecisions(s);

            for(const auto& d : decisionSpace)
            {
                KEY k (s,d);
                vf->SetValue(k, dist(gen));
            }
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                                  LEARNING METHODS
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    template<class KEY, class TABLE_HASH>
    void QLearning(MarkovDecisionProcess& mdp, const int numberOfIterations)
    {
        using LUTValueFunctionSharedPtr = std::shared_ptr<LUTValueFunction<KEY,TABLE_HASH>>;

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);

        int iterCounter = 0;
          
        // Algorithm parameters (initial values)
        double alpha = 1.; // learning rate
        double epsilon = 0.75; // greedy
       
        LUTValueFunctionSharedPtr sdvf = std::make_shared<LUTValueFunction<KEY,TABLE_HASH>>();

        // Arbitrarily define the Q table
        InitializeQTable<KEY, TABLE_HASH>(sdvf, mdp, gen);
        
        // Start Q-Learning
        for(uint iRun = 0; iRun < numberOfIterations; iRun++)
        {   
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

            double totalReward = 0.;

            // Generate a trajectory through the MDP
            while(!mdp.IsEndState())
            {             
                // ToDo: this function should just take in the mdp to shorten the arg list
                DecisionSharedPtr d = GetEpsilonGreedyAction<KEY, TABLE_HASH>(mdp.GetCurrentState(),
                                                                        sdvf, 
                                                                        mdp.GetLegalDecisions(),
                                                                        epsilon, 
                                                                        gen);

                // Create the state-decision pair object
                KEY k(mdp.GetCurrentState(),d); // Q(s,a)

                // Update the internal state of the MDP
                mdp.SetDecision(d); 
                mdp.UpdateDecision();
                mdp.UpdateExogenous();

                // Get the reward form the MDP
                const auto reward = mdp.CalculateContribution();
                totalReward += reward;    

                // update the q table  for the sd-pair previously defined
                const auto md = MaxDecision(sdvf,mdp.GetCurrentState(), mdp.GetLegalDecisions());
                double sdvfUpdate = (*sdvf)(k) + alpha*(reward + mdp.GetGamma()*md - (*sdvf)(k));
                sdvf->SetValue(k,sdvfUpdate);               
            }    

            std::cout << totalReward << "\t" << alpha << "\t" << epsilon << std::endl;   
        }
    } 

};
*/
#endif
