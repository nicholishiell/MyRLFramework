#include "MDP.h"

MarkovDecisionProcess::MarkovDecisionProcess()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    gen_ = std::mt19937(seed);   

    initialState_ = nullptr;
    currentState_ = nullptr;
    currentDecision_ = nullptr;

    decisionPolicy_ = nullptr;

    double gamma_ = 0.;

    cumulativeReward_ = 0.;

    deltaTimeGetDecision_ = 0.;
    deltaTimeCalculateContribution_ = 0.;
    deltaTimeUpdateDecision_ = 0.;
    deltaTimeUpdateExogenous_ = 0.;
}

MarkovDecisionProcess::~MarkovDecisionProcess()
{

}

bool 
MarkovDecisionProcess::isReady()
{
    return  (initialState_ != nullptr) && (decisionPolicy_ != nullptr);
}

Trajectory 
MarkovDecisionProcess::RunEpisode()
{
    using namespace std::chrono;

    Trajectory t;
    cumulativeReward_ = 0.;

    if(!isReady())
    {
        std::cout << "[ERROR] MDP can't RunEpisode with out initialState and decisionPolicy defined." << std::endl;
    }

    high_resolution_clock::time_point t1, t2;

    while(!IsEndState())
    {
        Step currentStep;
        currentStep.s = currentState_;

        t1 = high_resolution_clock::now();
        currentDecision_ = (*decisionPolicy_)(currentState_);
        t2 = high_resolution_clock::now();
        deltaTimeGetDecision_ +=  duration_cast<duration<double>>(t2-t1).count();

        currentStep.d = currentDecision_;

        t1 = high_resolution_clock::now();
        const auto reward = CalculateContribution();
        t2 = high_resolution_clock::now();

        deltaTimeCalculateContribution_ +=  duration_cast<duration<double>>(t2-t1).count();

        cumulativeReward_ += reward;
        currentStep.r = reward;

        t1 = high_resolution_clock::now();       
        UpdateDecision();
        t2 = high_resolution_clock::now();

        deltaTimeUpdateDecision_ +=  duration_cast<duration<double>>(t2-t1).count();

        t1 = high_resolution_clock::now();
        UpdateExogenous();
        t2 = high_resolution_clock::now();
        deltaTimeUpdateExogenous_ +=  duration_cast<duration<double>>(t2-t1).count();

        t.emplace_back(currentStep);
    }

    return t;
}

double 
MarkovDecisionProcess::GetGamma() const
{
    return gamma_;
}

double 
MarkovDecisionProcess::GetCumulativeReward() const
{
    return cumulativeReward_;
}

void 
MarkovDecisionProcess::SetGamma(const double gamma)
{
    gamma_ = gamma;
}

StateSpace 
MarkovDecisionProcess::GetStateSpace() const
{
    return stateSpace_;    
}

DecisionSpace 
MarkovDecisionProcess::GetLegalDecisions() const
{
    std::cout << "[WARNING] MarkovDecisionProcess base class GetLegalDecisions method used..." << std::endl;
    
    DecisionSpace ds;

    return ds;
}

DecisionSpace 
MarkovDecisionProcess::GetLegalDecisions(const StateSharedPtr state) const
{
    std::cout << "[WARNING] MarkovDecisionProcess base class GetLegalDecisions method used..." << std::endl;

    DecisionSpace ds;

    return ds;
}

bool 
MarkovDecisionProcess::IsEndState() const
{
    std::cout << "[WARNING] MarkovDecisionProcess base class IsEndState method used..." << std::endl;
    
    return true;    
}

double 
MarkovDecisionProcess::CalculateContribution() const
{
    return CalculateContribution(GetCurrentState(),GetCurrentDecision());
}

double 
MarkovDecisionProcess::CalculateContribution(   const StateSharedPtr state,
                                                const DecisionSharedPtr decision) const
{
    std::cout << "[WARNING] MarkovDecisionProcess base class CalculateContribution method used..." << std::endl;
    
    return 0.;
}

void 
MarkovDecisionProcess::RecordLearning(const Step& s)
{
    std::cout << "[WARNING] MarkovDecisionProcess base class RecordLearning method used..." << std::endl;
    
    return;   
}

void 
MarkovDecisionProcess::UpdateDecision()
{
    std::cout << "[WARNING] MarkovDecisionProcess base class UpdateDecision method used..." << std::endl;
}

void 
MarkovDecisionProcess::UpdateExogenous()
{
    std::cout << "[WARNING] MarkovDecisionProcess base class UpdateExogenous method used..." << std::endl;
}

PolicySharedPtr 
MarkovDecisionProcess::GetDecisionPolicy() const
{
    return decisionPolicy_;
}

const StateSharedPtr 
MarkovDecisionProcess::GetCurrentState() const
{
    return currentState_;
}

const StateSharedPtr
MarkovDecisionProcess::GetInitialState() const
{
    return initialState_;
}

const DecisionSharedPtr 
MarkovDecisionProcess::GetCurrentDecision() const
{
    return currentDecision_;
}

void 
MarkovDecisionProcess::SetInitialState(const StateSharedPtr initialState)
{
    std::cout << "[WARNING] MarkovDecisionProcess base class SetInitialState method used..." << std::endl;

    initialState_ =  std::make_shared<State>(*initialState);
}

void 
MarkovDecisionProcess::SetCurrentState(const StateSharedPtr state)
{
    std::cout << "[WARNING] MarkovDecisionProcess base class SetCurretnState method used..." << std::endl;
    
    currentState_ = std::make_shared<State>(*state);
}

void 
MarkovDecisionProcess::SetDecision(const DecisionSharedPtr decision)
{
    std::cout << "[WARNING] MarkovDecisionProcess base class SetDecision method used..." << std::endl;
    currentDecision_ = std::make_shared<Decision>(*decision);
}

void 
MarkovDecisionProcess::SetPolicy(const PolicySharedPtr& decisionPolicy)
{
    std::cout << "[WARNING] MarkovDecisionProcess base class SetPolicy method used..." << std::endl;

    decisionPolicy_ = std::make_shared<Policy>(*decisionPolicy);
}

void 
MarkovDecisionProcess::Reset()
{
    std::cout << "[WARNING] MarkovDecisionProcess base class Reset method used..." << std::endl;

    currentState_ = std::make_shared<State>(*initialState_);
}