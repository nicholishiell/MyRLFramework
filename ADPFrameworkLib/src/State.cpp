#include "State.h"

State::State()
{
}

State::~State()
{

}

bool 
State::IsDecisionLegal(const DecisionSharedPtr& d) const
{
    std::cout << "[WARNING] State base class IsLegalDecision methods used..." << std::endl;

    return false;
}

std::string 
State::ToString() const
{
    std::cout << "[WARNING] State base class ToString methods used..." << std::endl;

    return "";
}

bool 
State::operator== (const StateSharedPtr s) const
{
    std::cout << "[WARNING] State base class == operator used..." << std::endl;
    return true;
}

StateSharedPtr 
State::clone() const
{
    std::cout << "[WARNING] State base class clone method used..." << std::endl;
    return nullptr;    
}