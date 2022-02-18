#include "MyState.h"
#include "MyDecision.h"
#include "MyUtil.h"


using namespace MyUtil;

MyState::MyState(ShortArray4d s)
{
    triagePopulations_ = s;
}

MyState::MyState(const uShort a, const uShort b, const uShort c, const uShort d)
{
    triagePopulations_ = {a,b,c,d};
}

MyState::~MyState()
{

}

ShortArray4d 
MyState::GetTriagePopulations() const
{
    return triagePopulations_;
}

uShort 
MyState::GetTriagePopulations(int i) const
{
    return triagePopulations_[i];
}

std::string 
MyState::ToString() const
{
    return Util::ToString(triagePopulations_);
}    

bool 
MyState::operator== (const State& s) const
{
    const auto sCast = dynamic_cast<const MyState&>(s);
    const auto arrayB = sCast.GetTriagePopulations();
    const auto arrayA = this->GetTriagePopulations();

    bool test = arrayB[0] == arrayA[0] && 
                arrayB[1] == arrayA[1] &&
                arrayB[2] == arrayA[2] &&
                arrayB[3] == arrayA[3];
   
    return test;
}

bool 
MyState::IsDecisionLegal(const DecisionSharedPtr& d) const
{
    const auto dArray = std::dynamic_pointer_cast<MyDecision>(d)->GetDecisionArray();
        
    // Check to make sure there are enough people at the IS
    for(int i = 0; i < dArray.size(); i++)
    {
        if(dArray[i] > triagePopulations_[i])
        {
            return false;
        }
    }

    // Check to make sure the helicopter is as full as possible
    int decisionCap = 0;
    IntArray4d sArrayPrime = {0,0,0,0};

    for(int i = 0; i < dArray.size(); i++)
    {
        decisionCap += spaceRequirements_[i]*dArray[i];

        sArrayPrime[i] = triagePopulations_[i] - dArray[i];
    }

    if(decisionCap != heloCapacity_)
    {
        int deltaCap = heloCapacity_ - decisionCap;

        for(int i = 0; i < sArrayPrime.size(); i++)
        {
            if(sArrayPrime[i] > 0)
            {
                if(deltaCap >= spaceRequirements_[i])
                {
                    return false;
                }
            }
        }
    }

    return true;
}


StateSharedPtr 
MyState::clone() const
{
    return std::make_shared<MyState>(this->GetTriagePopulations());
}