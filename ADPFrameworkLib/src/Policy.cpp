#include "Util.h"
#include "Policy.h"


Policy::Policy()
{

}

Policy::~Policy()
{

}

DecisionSharedPtr 
Policy::operator() (const StateSharedPtr s) const
{
    std::cout << "[WARNING] Policy base class () operator used..." << std::endl;
    return std::make_shared<Decision>();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LUTPolicy::LUTPolicy()
{

}

LUTPolicy::~LUTPolicy()
{

}

DecisionSharedPtr 
LUTPolicy::operator() (const StateSharedPtr s) const
{
    DecisionSharedPtr d = nullptr;

    const auto iter = lookupTable_.find(s);

    if(iter != lookupTable_.end())
    {
        d = lookupTable_.at(s);
    }

    return d;
}

void 
LUTPolicy::UpdatePolicy(const StateSharedPtr s, const DecisionSharedPtr d)
{
    lookupTable_.emplace(s, d);
}