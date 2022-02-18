#include "MyValueFunction.h"

PostDecisionState::PostDecisionState(const StateSharedPtr s, const DecisionSharedPtr d)
{
    const auto sCast = std::dynamic_pointer_cast<MyState>(s);
    const auto dCast = std::dynamic_pointer_cast<MyDecision>(d);

    for(unsigned int i = 0; i < PostDecisionStateVariable_.size(); i++)
    {
        const auto value = sCast->GetTriagePopulations(i)-dCast->GetDecisionArrayElement(i);
        PostDecisionStateVariable_[i] = value;
    }
}

int 
PostDecisionState::GetArrayElement(const int i) const
{
    return PostDecisionStateVariable_[i];
}

bool 
PostDecisionState::operator==(const LUTKey& k) const
{
    const auto keyCast = static_cast<const PostDecisionState&>(k);
    
    bool retVal = true;

    for(int i = 0; i < PostDecisionStateVariable_.size(); i++)
    {
        if(PostDecisionStateVariable_[i] != keyCast.GetArrayElement(i))
        {
            retVal = false;
            break;
        }
    }

    return retVal;
}

std::string
PostDecisionState::ToString() const
{
    return Util::ToString(PostDecisionStateVariable_);
}

std::array<uShort,2> 
PostDecisionState::GetArray() const
{
    return PostDecisionStateVariable_;
}

size_t 
PostDecisionStateHash::operator()(const LUTKey& k) const
{
    const auto keyCast = static_cast<const PostDecisionState&>(k);
    const auto a = keyCast.GetArray();

    std::size_t h = 0;

    for(auto e : a) 
    {
        h ^= std::hash<int>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
    }
    
    return h;
}

