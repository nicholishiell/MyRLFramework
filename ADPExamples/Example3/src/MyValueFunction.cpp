#include "MyValueFunction.h"

StateDecisionPair::StateDecisionPair(const StateSharedPtr s, const DecisionSharedPtr d)
{
    const auto sCast = std::dynamic_pointer_cast<MyState>(s);
    const auto dCast = std::dynamic_pointer_cast<MyDecision>(d);

    for(int i = 0; i < 2; i++)
        stateDecisionPair_[i] = sCast->GetTriagePopulations(i);

    for(int i = 0; i < 2; i++)
    {
        stateDecisionPair_[i+2] = dCast->GetDecisionArrayElement(i);
    }
}

int 
StateDecisionPair::GetArrayElement(const int i) const
{
    return stateDecisionPair_[i];
}

bool 
StateDecisionPair::operator==(const LUTKey& k) const
{
    const auto keyCast = static_cast<const StateDecisionPair&>(k);
    
    bool retVal = true;

    for(int i = 0; i < stateDecisionPair_.size(); i++)
    {
        if(stateDecisionPair_[i] != keyCast.GetArrayElement(i))
        {
            retVal = false;
            break;
        }
    }

    return retVal;
}

std::array<int,4> 
StateDecisionPair::GetArray() const
{
    return stateDecisionPair_;
}

std::string
StateDecisionPair::ToString() const
{
    return Util::ToString(stateDecisionPair_);
}

size_t 
StateDecisionHash::operator()(const LUTKey& k) const
{
    const auto keyCast = static_cast<const StateDecisionPair&>(k);
    const auto a = keyCast.GetArray();

    std::size_t h = 0;

    for(auto e : a) 
    {
        h ^= std::hash<int>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
    }
    return h;
}