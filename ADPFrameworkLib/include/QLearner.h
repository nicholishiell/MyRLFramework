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

template<   class KEY, 
            class TABLE_HASH, class TABLE_EQUAL, 
            class STATE_HASH, class STATE_EQUAL>
class QLearner
{
public:

    using QTableSharedPtr = std::shared_ptr<LUTValueFunction<KEY,TABLE_HASH, TABLE_EQUAL>>;
    using LUTPolicySharedPtr = std::shared_ptr<LUTPolicy<STATE_HASH, STATE_EQUAL>>;

    QLearner();
    QLearner(const double alpha, const double epsilon);

    ~QLearner();

    void Learn( MarkovDecisionProcess& mdp, 
                const int numberOfIterations, 
                void (*updateLearningParameters)(const int, double&, double&) = *defaultUpdateLearningParameters);

    void SetAlpha(const double alpha);      // Sets initial value
    void SetEpsilon(const double epsilon);  // Sets initial value

    double GetAlpha() const;                // Gets current value
    double GetEpsilon() const;              // Gets current value

    QTableSharedPtr GetLearnedQTable() const;

    LUTPolicySharedPtr GetLearnedPolicy() const;

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

    double getRandom();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Private Members
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    double alphaInitial_;   // learning rate
    double alphaCurrent_;  

    double epsilonInitial_; // greedy "rate"
    double epsilonCurrent_;

    QTableSharedPtr qTable_;

    LUTPolicySharedPtr learnedPolicy_;

    std::mt19937 gen_;
    std::uniform_real_distribution<double> epsilonGreedyDist_;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FUNCTION DEFINITIONS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
double 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::getRandom()
{
    return epsilonGreedyDist_(gen_);
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::QLearner()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    gen_ = std::mt19937(seed);

    epsilonGreedyDist_ = std::uniform_real_distribution<double>(0.,1.);
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::QLearner(const double alpha, const double epsilon) 
: QLearner()
{
    alphaInitial_ = alpha;
    alphaCurrent_ = alpha;

    epsilonInitial_ = epsilon;
    epsilonCurrent_ = epsilon;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::~QLearner()
{

}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
DecisionSharedPtr 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::getEpsilonGreedyAction(const StateSharedPtr state, 
                                                                                    DecisionSpace legalDecisionSpace)
{
    DecisionSharedPtr decision = nullptr;

    auto randomNumber = getRandom();

    if(randomNumber < epsilonCurrent_)
    {
        std::random_shuffle(legalDecisionSpace.begin(),legalDecisionSpace.end());
        decision = legalDecisionSpace[0];
    }
    else
    {
        decision = (*learnedPolicy_)(state);
    }

    return decision;
}


template<   class KEY, 
            class TABLE_HASH, class TABLE_EQUAL, 
            class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::Learn( MarkovDecisionProcess& mdp, 
                                                                    const int numberOfIterations,
                                                                    void (*updateLearningParameters)(const int, double&, double&))
{
    using namespace std::chrono;

    int iterCounter = 0;

    // Todo: this should be in the constructor, but get seg fault.
    learnedPolicy_ = std::make_shared<LUTPolicy<STATE_HASH,STATE_EQUAL>>();

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

            if(d == nullptr) std::cout << "null decision pointer." << std::endl;

            // Create the state-decision pair object
            const auto currentState = mdp.GetCurrentState();

            KEY k(currentState,d); // Q(s,a)            

            // Update the internal state of the MDP
            mdp.SetDecision(d); 
            mdp.UpdateDecision();
            mdp.UpdateExogenous();
        
            // add the new state and all it's potential decisions into the qTable
            appendQTable(mdp.GetCurrentState(), mdp.GetLegalDecisions());

            // update the policy
            updatePolicy(mdp.GetCurrentState(), mdp.GetLegalDecisions());

            // Get the reward form the MDP
            const auto reward = mdp.CalculateContribution();
            totalReward += reward;

            // update the q table for the sd-pair previously defined
            const auto md = maxDecision(mdp.GetCurrentState(), mdp.GetLegalDecisions());
            double sdvfUpdate = (*qTable_)(k) + alphaCurrent_*(reward + mdp.GetGamma()*md - (*qTable_)(k));
            
            qTable_->SetValue(k,sdvfUpdate);
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
                        << learnedPolicy_->GetTable().size() << "\t" 
                        << deltaTime.count() << std::endl;
        }
    }

    updatePolicy(mdp.GetCurrentState(), mdp.GetLegalDecisions());
} 

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::SetAlpha(const double alpha)
{
    alphaInitial_ = alpha;
    alphaCurrent_ = alpha;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::SetEpsilon(const double epsilon)
{
    epsilonInitial_ = epsilon;
    epsilonCurrent_ = epsilon;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
double
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::GetAlpha() const
{
    return alphaCurrent_;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
double 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::GetEpsilon() const
{
    return epsilonCurrent_;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
std::shared_ptr<LUTValueFunction<KEY,TABLE_HASH,TABLE_EQUAL>>
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::GetLearnedQTable() const
{
    return qTable_;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
std::shared_ptr<LUTPolicy<STATE_HASH, STATE_EQUAL>>
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::GetLearnedPolicy() const
{
    return learnedPolicy_;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::Reset()
{
    // Todo: add learned policy here to get reset
    qTable_.reset();
    alphaCurrent_ = alphaInitial_;
    epsilonCurrent_ = epsilonInitial_;    
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
DecisionSharedPtr 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::argMaxDecision(const StateSharedPtr state, 
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
            decision = d;//->clone();
        }
    }

    return decision;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::updatePolicy(  const StateSharedPtr state, 
                                                                            const DecisionSpace legalDecisionSpace)
{
    const auto decision = argMaxDecision(state, legalDecisionSpace);
    learnedPolicy_->UpdatePolicy(state, decision);
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::appendQTable(  const StateSharedPtr state, 
                                                                            const DecisionSpace legalDecisionSpace)
{
    for(const auto& d : legalDecisionSpace)            
    {
        KEY k(state, d);

        if(qTable_->Contains(k))
        {
            break;
        }
        else
        {
            qTable_->SetValue(k,epsilonGreedyDist_(gen_));
        }
    }    
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
double 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::maxDecision(   const StateSharedPtr state, 
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

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::defaultUpdateLearningParameters(   const int nIter, 
                                                                        double& alpha, 
                                                                        double& epsilon)
{
    std::cout << "[WARNING] QLearner base class updateLearningParameters method used..." << std::endl;
    
    return;
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::initializeQTable(  const StateSharedPtr initialState, 
                                                                                const MarkovDecisionProcess& mdp)
{
    qTable_ = std::make_shared<LUTValueFunction<KEY,TABLE_HASH,TABLE_EQUAL>>();

    std::uniform_real_distribution<double> dist(0.,1.);

    const auto decisionSpace = mdp.GetLegalDecisions(initialState);

    for(const auto& d : decisionSpace)
    {
        KEY k (initialState,d);
        qTable_->SetValue(k, dist(gen_));
    } 
}

template<class KEY, class TABLE_HASH, class TABLE_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::initializeQTable(const MarkovDecisionProcess& mdp)
{
    qTable_ = std::make_shared<LUTValueFunction<KEY,TABLE_HASH,TABLE_EQUAL>>();

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

template<   class KEY, 
            class TABLE_HASH, class TABLE_EQUAL, 
            class STATE_HASH, class STATE_EQUAL>
void
QLearner<KEY,TABLE_HASH,TABLE_EQUAL,STATE_HASH,STATE_EQUAL>::outputPolicy(const MarkovDecisionProcess& mdp)
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
    }
}

#endif