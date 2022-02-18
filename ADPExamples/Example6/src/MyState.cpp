#include "MyState.h"

MyState::MyState(const ShortArray2d& s)
{
    triagePopulations_ = s;
}

MyState::MyState(const uShort a, const uShort b)
{
    triagePopulations_ = {a,b};
}

MyState::~MyState()
{

}

ShortArray2d
MyState::GetTriagePopulations() const
{
    return triagePopulations_;
}

int 
MyState::GetTriagePopulations(int i) const
{
    return triagePopulations_[i];
}

std::string 
MyState::ToString() const
{
    return Util::ToString(triagePopulations_);
}    