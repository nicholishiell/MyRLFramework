#ifndef QLEARNER_H
#define QLEARNER_H

#include <map>
#include <chrono>
#include <algorithm>
#include <ctime>

#include "Util.h"
#include "Decision.h"
#include "State.h"
#include "ValueFunction.h"
#include "MDP.h"

using namespace Util;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CLASS DEFINITION
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class KEY, class HASH>
class QLearner
{
public:

    using LUTSharedPtr = std::shared_ptr<LUTValueFunction<KEY,HASH>>;

    QLearner();
    QLearner(const double alpha, const double epsilon);

    ~QLearner();

    void Learn( MarkovDecisionProcess& mdp, 
                const int numberOfIterations, 
                void (*updateLearningParameters)(const int, double&, double&) = *defaultUpdateLearningParameters);

    void SetAlpha(const double alpha);      // Sets initial value
    void SetEpsilon(const double epsilon);  // Sets initial value

    double GetAlpha() const;                  // Gets current value
    double GetEpsilon() const;                // Gets current value

    LUTSharedPtr GetLearnedQTable() const;

    void Reset();                           // clears all internal state data
    
private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Private Functions
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    void initializeQTable(const MarkovDecisionProcess& mdp);
    void initializeQTable(const StateSharedPtr initialState, const MarkovDecisionProcess& mdp);

    virtual void defaultUpdateLearningParameters(const int nIter, double& alpha, double& epsilon);

    DecisionSharedPtr getEpsilonGreedyAction(   const StateSharedPtr state, 
                                                DecisionSpace legalDecisionSpace);

    double maxDecision( const StateSharedPtr state, 
                        const DecisionSpace legalDecisionSpace);

    DecisionSharedPtr argMaxDecision(   const StateSharedPtr state, 
                                        const DecisionSpace legalDecisionSpace);

    void appendQTable(  const StateSharedPtr state, 
                        const DecisionSpace legalDecisionSpace);

    void updatePolicy(  const StateSharedPtr state, 
                        const DecisionSpace legalDecisionSpace);

    void outputPolicy(const MarkovDecisionProcess& mdp);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Private Members
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    double alphaInitial_;   // learning rate
    double alphaCurrent_;  

    double epsilonInitial_; // greedy "rate"
    double epsilonCurrent_;

    LUTSharedPtr qTable_;

    LUTPolicy qTablePolicy_;

    std::mt19937 gen_;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FUNCTION DEFINITIONS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class KEY, class HASH>
void
QLearner<KEY,HASH>::outputPolicy(const MarkovDecisionProcess& mdp)
{
    for(const auto s : mdp.GetStateSpace())
    {
        DecisionSharedPtr decision;
        double highestValue = VERY_NEGATIVE_NUMBER;

        for(const auto d : mdp.GetLegalDecisions(s))
        {
            KEY k(s,d);  
            const auto value = (*qTable_)(k);

            if(value > highestValue)
            {
                highestValue = value;
                decision = d;
            }

        }

        std::cout << s->ToString() << " | " << decision->ToString() << "\t" << highestValue << std::endl;
        std::cin.get();
    }
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::Learn(  MarkovDecisionProcess& mdp, 
                            const int numberOfIterations,
                            void (*updateLearningParameters)(const int, double&, double&))
{
    using namespace std::chrono;

    int iterCounter = 0;
           
    // Arbitrarily initialize the Q table
    initializeQTable(mdp.GetInitialState(), mdp);

    updatePolicy(mdp.GetInitialState(), mdp.GetLegalDecisions());

    // Start Q-Learning
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(uint iRun = 0; iRun < numberOfIterations; iRun++)
    {    
        // Reset for th next iteration
        mdp.Reset();
        double totalReward = 0.;

        // Adjust the alpha and epsilon values to refine the search
        updateLearningParameters(iRun, alphaCurrent_,epsilonCurrent_);           
        
        // Generate a trajectory through the MDP
        while(!mdp.IsEndState())
        {             
            // ToDo: this function should just take in the mdp to shorten the arg list
            DecisionSharedPtr d = getEpsilonGreedyAction(   mdp.GetCurrentState(), 
                                                            mdp.GetLegalDecisions());
            
            if(d == nullptr) std::cout << "null" << std::endl;

            // Create the state-decision pair object
            const auto currentState = mdp.GetCurrentState();
            KEY k(currentState,d); // Q(s,a)            
            
            // Update the internal state of the MDP
            mdp.SetDecision(d); 
            mdp.UpdateDecision();
            mdp.UpdateExogenous();
        
            // add the new state and all it's potential decisions into the qTable
            appendQTable(mdp.GetCurrentState(), mdp.GetLegalDecisions());
            updatePolicy(mdp.GetCurrentState(), mdp.GetLegalDecisions());

            // Get the reward form the MDP
            const auto reward = mdp.CalculateContribution();
            totalReward += reward;
        
            // update the q table for the sd-pair previously defined
            const auto md = maxDecision(mdp.GetCurrentState(), mdp.GetLegalDecisions());
            double sdvfUpdate = (*qTable_)(k) + alphaCurrent_*(reward + mdp.GetGamma()*md - (*qTable_)(k));
            qTable_->SetValue(k,sdvfUpdate);

            updatePolicy(currentState, mdp.GetLegalDecisions(currentState));

            std::cout   << "Current Step: "
                        << currentState->ToString() << " + " 
                        << d->ToString() << " -> "
                        << mdp.GetCurrentState()->ToString() << "\t"
                        << reward << std::endl;

            std::cout << "current policy:" << std::endl;
            for(const auto& kv : qTablePolicy_.GetTable())
            {
                std::cout << kv.first->ToString() << " -> " << kv.second->ToString() << std::endl;
            }

            std::cin.get();
        }

        if(iRun % 1000 == 0) 
        {
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            const auto deltaTime = duration_cast<duration<double>>(t2 - t1);
            t1 = t2;

            std::cout   << iRun << "\t" 
                        << alphaCurrent_ << "\t" 
                        << epsilonCurrent_ << "\t" 
                        << totalReward <<  "\t"
                        << qTable_->GetSize() << "\t"
                        << deltaTime.count() << std::endl;
        }
    }

} 

template<class KEY, class HASH>
QLearner<KEY,HASH>::QLearner()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    gen_ = std::mt19937(seed);
}

template<class KEY, class HASH>
QLearner<KEY,HASH>::~QLearner()
{

}

template<class KEY, class HASH>
QLearner<KEY,HASH>::QLearner(const double alpha, const double epsilon)
{
    alphaInitial_ = alpha;
    alphaCurrent_ = alpha;

    epsilonInitial_ = epsilon;
    epsilonCurrent_ = epsilon;
    
    QLearner();
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::SetAlpha(const double alpha)
{
    alphaInitial_ = alpha;
    alphaCurrent_ = alpha;
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::SetEpsilon(const double epsilon)
{
    epsilonInitial_ = epsilon;
    epsilonCurrent_ = epsilon;
}

template<class KEY, class HASH>
double
QLearner<KEY,HASH>::GetAlpha() const
{
    return alphaCurrent_;
}

template<class KEY, class HASH>
double 
QLearner<KEY,HASH>::GetEpsilon() const
{
    return epsilonCurrent_;
}

template<class KEY, class HASH>
std::shared_ptr<LUTValueFunction<KEY,HASH>>
QLearner<KEY,HASH>::GetLearnedQTable() const
{
    return qTable_;
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::Reset()
{
    qTable_.reset();
    alphaCurrent_ = alphaInitial_;
    epsilonCurrent_ = epsilonInitial_;    
}

template<class KEY, class HASH>
DecisionSharedPtr 
QLearner<KEY,HASH>::getEpsilonGreedyAction( const StateSharedPtr state, 
                                            DecisionSpace legalDecisionSpace)
{
    DecisionSharedPtr decision = nullptr;

    std::uniform_real_distribution<double> dist(0.0,1.0);
    const double randomNumber = dist(gen_);

    if(randomNumber < epsilonCurrent_)
    {
        std::random_shuffle(legalDecisionSpace.begin(),legalDecisionSpace.end());
        decision = legalDecisionSpace[0];
    }
    else
    {
        decision = qTablePolicy_(state);
    }

    return decision;
}

template<class KEY, class HASH>
DecisionSharedPtr 
QLearner<KEY,HASH>::argMaxDecision( const StateSharedPtr state, 
                                    const DecisionSpace legalDecisionSpace)
{
    DecisionSharedPtr decision = nullptr;

    double highestValue = VERY_NEGATIVE_NUMBER;
  
    for(const auto& d : legalDecisionSpace)            
    {
        KEY k(state, d);
        const auto value = (*qTable_)(k);

        if(value > highestValue)
        {
            highestValue = value;
            decision = d->clone();
        }
    }

    return decision;
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::updatePolicy(   const StateSharedPtr state, 
                                    const DecisionSpace legalDecisionSpace)
{
    const auto decision = argMaxDecision(state, legalDecisionSpace);

    qTablePolicy_.UpdatePolicy(state, decision);
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::appendQTable(   const StateSharedPtr state, 
                                    const DecisionSpace legalDecisionSpace)
{
    std::uniform_real_distribution<double> dist(0.,1.);

    for(const auto& d : legalDecisionSpace)            
    {
        KEY k(state, d);

        if(qTable_->Contains(k)) break;

        qTable_->SetValue(k,dist(gen_));
    }    
}

template<class KEY, class HASH>
double 
QLearner<KEY,HASH>::maxDecision(const StateSharedPtr state, 
                                const DecisionSpace legalDecisionSpace)
{
    double highestValue = VERY_NEGATIVE_NUMBER;
    
    for(const auto d : legalDecisionSpace)
    {
        KEY k(state, d);

        if((*qTable_)(k) > highestValue)
        {
            highestValue = (*qTable_)(k);
        }
    }

    return highestValue;
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::defaultUpdateLearningParameters(const int nIter, 
                                                    double& alpha, 
                                                    double& epsilon)
{
    std::cout << "[WARNING] QLearner base class updateLearningParameters method used..." << std::endl;
    
    return;
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::initializeQTable(   const StateSharedPtr initialState, 
                                        const MarkovDecisionProcess& mdp)
{
    qTable_ = std::make_shared<LUTValueFunction<KEY,HASH>>();

    std::uniform_real_distribution<double> dist(0.,1.);

    const auto decisionSpace = mdp.GetLegalDecisions(initialState);

    for(const auto& d : decisionSpace)
    {
        KEY k (initialState,d);
        qTable_->SetValue(k, dist(gen_));
    } 
}

template<class KEY, class HASH>
void 
QLearner<KEY,HASH>::initializeQTable(const MarkovDecisionProcess& mdp)
{
    qTable_ = std::make_shared<LUTValueFunction<KEY,HASH>>();

    const auto stateSpace = mdp.GetStateSpace();

    std::uniform_real_distribution<double> dist(0.,1.);

    for(const auto& s : stateSpace)
    {
        const auto decisionSpace = mdp.GetLegalDecisions(s);

        for(const auto& d : decisionSpace)
        {
            KEY k (s,d);
            qTable_->SetValue(k, dist(gen_));
        }
    }
}

#endif