#ifndef AVI_LEARNER_H
#define AVI_LEARNER_H

#include <unordered_set>
#include <array>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <fstream>

#include "Util.h"
#include "Decision.h"
#include "State.h"
#include "ValueFunction.h"
#include "MDP.h"

using namespace Util;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CLASS DEFINITION
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
class AVILearner
{
public:

    using LUTPolicySharedPtr = std::shared_ptr<LUTPolicy<STATE_HASH, STATE_EQUAL>>;
    using LUTValueFunctionSharedPtr = std::shared_ptr<LUTValueFunction<PDSV, PDSV_HASH, PDSV_EQUAL>>;
    using SetOfStates = std::unordered_set<StateSharedPtr, STATE_HASH, STATE_EQUAL>;

    AVILearner();
    AVILearner(const double alpha, const double epsilon);

    ~AVILearner();

    void Learn( MarkovDecisionProcess& mdp, 
                const int numberOfIterations, 
                void (*updateLearningParameters)(const int, double&, double&) = *defaultUpdateLearningParameters);

    void SetAlpha(const double alpha);      // Sets initial value
    void SetEpsilon(const double epsilon);  // Sets initial value
    void SetDefaultValue(const double val); // Sets default value in AVF

    double GetAlpha() const;                // Gets current value
    double GetEpsilon() const;              // Gets current value 

    void Reset();                           // clears all internal state data
    
    LUTPolicySharedPtr GetLearnedPolicy() const;

    void WriteToFile() const {approximateValueFunction_->WriteToFile();}

private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Private Functions
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    virtual void defaultUpdateLearningParameters(const int nIter, double& alpha, double& epsilon);

    DecisionSharedPtr getEpsilonGreedyAction(const MarkovDecisionProcess& mdp);

    DecisionSharedPtr argMaxDecision(   const StateSharedPtr state, 
                                        const MarkovDecisionProcess& mdp);

    void updateValueFunction(const Trajectory t);

    void outputPolicy(const MarkovDecisionProcess& mdp);

    double getRandom();

    void generatePolicy(const MarkovDecisionProcess& mdp);

    void resetValueHistory()
    {
        for(int i = 0; i < valueHistory_.size(); i++)   
            valueHistory_[i] = 0.;
    }

    void writeValueHistoryToFile()
    {
        std::ofstream out;
        out.open("/home/nshiell/Documents/MyCode/MyMDPCode/MyRLFramework/ADPExamples/Example5/valueHistory.dat", std::fstream::trunc);

        int counter = 0;
        for(int i = valueHistory_.size(); i >= 0; i--)
        {
            out << counter++ << " " << valueHistory_[i]/100. << std::endl;
        }

        out.close();
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Private Members
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    double alphaInitial_;   // learning rate
    double alphaCurrent_;  

    double epsilonInitial_; // greedy "rate"
    double epsilonCurrent_;

    LUTValueFunctionSharedPtr approximateValueFunction_;

    LUTPolicySharedPtr learnedPolicy_;

    std::mt19937 gen_;
    std::uniform_real_distribution<double> epsilonGreedyDist_;

    //std::array<ShortArray4d, 120> decisionHistory_;
    std::array<float, 150> valueHistory_;

    SetOfStates observedStates_;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FUNCTION DEFINITIONS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::AVILearner()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    gen_ = std::mt19937(seed);

    epsilonGreedyDist_ = std::uniform_real_distribution<double>(0.,1.);

    approximateValueFunction_ = std::make_shared<LUTValueFunction<PDSV,PDSV_HASH,PDSV_EQUAL>>();
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::AVILearner(const double alpha, const double epsilon) 
: AVILearner()
{
    alphaInitial_ = alpha;
    alphaCurrent_ = alpha;

    epsilonInitial_ = epsilon;
    epsilonCurrent_ = epsilon;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::~AVILearner()
{

}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::updateValueFunction(const Trajectory t)
{
    double v_tp1 = 0.;

    int counter = 0;

    for(auto iter = t.rbegin(); iter != t.rend(); ++iter) 
    { 
        double v_t = iter->r + v_tp1;

        PDSV k(iter->s,iter->d);

        auto prevAVF = (*approximateValueFunction_)(k);

        const auto updatedAVF = (1.-alphaCurrent_) * prevAVF + alphaCurrent_*v_t;

        approximateValueFunction_->SetValue(k, updatedAVF);

        v_tp1 = v_t;

        valueHistory_[counter++] += updatedAVF;


       //std::cout << k.ToString() << "\t" << iter->d->ToString() << "\t" << prevAVF << "\t" <<  (*approximateValueFunction_)(k) << std::endl;
    } 
    //std::cin.get();
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
AVILearner< PDSV,PDSV_HASH,PDSV_EQUAL,
            STATE_HASH,STATE_EQUAL>::Learn( MarkovDecisionProcess& mdp, 
                                            const int numberOfIterations,
                                            void (*updateLearningParameters)(const int, double&, double&))
{
    using namespace std::chrono;

    int iterCounter = 0;

    // Todo: this should be in the constructor, but get seg fault.
    learnedPolicy_ = std::make_shared<LUTPolicy<STATE_HASH,STATE_EQUAL>>();

    // Start Q-Learning
    double cumulativeTotalReward = 0.;
    double lowestReward = 100000.;
    double highestReward = -1.;

    // ToDo: The MDP should have a virtual funciton for returning the end state
    //       and a "null" action.
    Step endStep;
    endStep.s = std::make_shared<MyState>(0,0);
    endStep.d = std::make_shared<MyDecision>(0,0);
    endStep.r = 0;

    for(uint iRun = 0; iRun < numberOfIterations; iRun++)
    {    
        // Reset for th next iteration
        mdp.Reset();
        Trajectory currentTrajectory;

        // Adjust the alpha and epsilon values to refine the search
        updateLearningParameters(iRun, alphaCurrent_,epsilonCurrent_);           
        
        double totalReward = 0.;
        
        // Generate a trajectory through the MDP
        while(!mdp.IsEndState())
        {    
            // Get decision from epsilon greedy policy
            DecisionSharedPtr d = getEpsilonGreedyAction(mdp);  
            if(d == nullptr) std::cout << "decision null ptr" << std::endl;

            const auto currentState = mdp.GetCurrentState(); 

            // Update the internal state of the MDP
            mdp.SetDecision(d); 
            mdp.UpdateDecision();
            mdp.UpdateExogenous();
 
            // Get the reward form the MDP
            const auto reward = mdp.CalculateContribution();
            totalReward += reward;
            
            // record the trajectory of the iteration
            const auto currentStep = Step(currentState, d, reward);
            currentTrajectory.emplace_back(currentStep);

            // ToDo: Move this to its own function, this information is need to form the policy.
            const auto iter = observedStates_.find(currentState);
            if(iter == observedStates_.end())
            {
                observedStates_.emplace(currentState);
            }

            mdp.RecordLearning(currentStep);
        }

        // approximateValueFunction_->Print();
        // std::cin.get();

        mdp.RecordLearning(endStep);

        // Perform Backward pass update
        updateValueFunction(currentTrajectory);

        if(totalReward < lowestReward) lowestReward = totalReward;
        if(totalReward > highestReward) highestReward = totalReward;
        cumulativeTotalReward += totalReward;

        if(iRun % 1000 == 0 && iRun != 0) 
        {
            std::cout   << iRun << "\t" 
                        << alphaCurrent_ << "\t" 
                        << epsilonCurrent_ << "\t" 
                        << approximateValueFunction_->GetSize() << "\t" 
                        << observedStates_.size() << "\t"
                        << lowestReward << "\t"
                        << highestReward << "\t"
                        << cumulativeTotalReward / 1000. <<  std::endl;

            cumulativeTotalReward = 0.;
            lowestReward = 100000.;
            highestReward = -1.;
        }

        // if(iRun % 100 == 0 && iRun != 0)
        // {
        //     writeValueHistoryToFile();
        //     resetValueHistory();
        // }
    }

    generatePolicy(mdp);
} 

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
DecisionSharedPtr 
AVILearner< PDSV,
            PDSV_HASH,PDSV_EQUAL,
            STATE_HASH,STATE_EQUAL>::getEpsilonGreedyAction(const MarkovDecisionProcess& mdp)
{
    DecisionSharedPtr decision = nullptr;

    auto randomNumber = getRandom();

    if(randomNumber < epsilonCurrent_)
    {
        auto legalDecisionSpace = mdp.GetLegalDecisions(mdp.GetCurrentState());
        std::random_shuffle(legalDecisionSpace.begin(),legalDecisionSpace.end());
        decision = legalDecisionSpace[0];
    }
    else
    {
        decision = argMaxDecision(mdp.GetCurrentState(), mdp);
    }

    return decision;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
double 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::getRandom()
{
    return epsilonGreedyDist_(gen_);
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::SetAlpha(const double alpha)
{
    alphaInitial_ = alpha;
    alphaCurrent_ = alpha;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::SetEpsilon(const double epsilon)
{
    epsilonInitial_ = epsilon;
    epsilonCurrent_ = epsilon;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::SetDefaultValue(const double val)
{
    approximateValueFunction_->SetDefaultValue(val);
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
double
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::GetAlpha() const
{
    return alphaCurrent_;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
double 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::GetEpsilon() const
{
    return epsilonCurrent_;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
std::shared_ptr<LUTPolicy<STATE_HASH, STATE_EQUAL>>
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::GetLearnedPolicy() const
{
    return learnedPolicy_;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::Reset()
{
    // Todo: add learned policy here to get reset
    approximateValueFunction_.reset();
    alphaCurrent_ = alphaInitial_;
    epsilonCurrent_ = epsilonInitial_;    
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
DecisionSharedPtr 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::argMaxDecision(   const StateSharedPtr state, 
                                                                                const MarkovDecisionProcess& mdp)
{
    DecisionSharedPtr decision = nullptr;

    double highestValue = VERY_NEGATIVE_NUMBER;
    
    auto legalDecisionSpace = mdp.GetLegalDecisions(state);
    std::random_shuffle(legalDecisionSpace.begin(),legalDecisionSpace.end());

    //std::cout << state->ToString() << std::endl;
    for(const auto& d : legalDecisionSpace)            
    {
        PDSV pdsv(state, d);
        const auto valueFunction = (*approximateValueFunction_)(pdsv);
        const auto reward = mdp.CalculateContribution(state,d);
        const auto value = valueFunction + reward; 

       // std::cout <<"\t" << d->ToString() << "\t" <<  valueFunction << "\t" << reward << std::endl;        

        if(value > highestValue)
        {
            highestValue = value;
            decision = d;
        }
    }

    // std::cout << decision->ToString() << std::endl;
    // std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

    return decision;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::generatePolicy(const MarkovDecisionProcess& mdp)
{
    std::cout << "Generating Policy: " << std::flush;
    for(const auto& s : observedStates_)
    {
        const auto d = argMaxDecision(s,mdp);

        learnedPolicy_->UpdatePolicy(s,d);
    }
    std::cout << "DONE!" << std::endl;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void 
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::defaultUpdateLearningParameters(const int nIter, 
                                                                        double& alpha, 
                                                                        double& epsilon)
{
    std::cout << "[WARNING] AVILearner base class updateLearningParameters method used..." << std::endl;
    
    return;
}

template<class PDSV, class PDSV_HASH, class PDSV_EQUAL, class STATE_HASH, class STATE_EQUAL>
void
AVILearner<PDSV,PDSV_HASH,PDSV_EQUAL,STATE_HASH,STATE_EQUAL>::outputPolicy(const MarkovDecisionProcess& mdp)
{
    std::cout << "[WARNING] AVILearner::outputPolicy function is incomplete..." << std::endl;

    for(const auto s : mdp.GetStateSpace())
    {
        DecisionSharedPtr decision;
        double highestValue = VERY_NEGATIVE_NUMBER;

        for(const auto d : mdp.GetLegalDecisions(s))
        {
            PDSV k(s,d);  
            const auto value = (*approximateValueFunction_)(k) + mdp.CalculateContribution(s,d);

            if(value > highestValue)
            {
                highestValue = value;
                decision = d;
            }
        }
    }
}

#endif