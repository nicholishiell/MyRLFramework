#include "MyState.h"

MyState::MyState(IntArray2d s)
{
    triagePopulations_ = s;
}

MyState::MyState(const int a, const int b)
{
    triagePopulations_ = {a,b};
}

MyState::~MyState()
{

}

IntArray2d 
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