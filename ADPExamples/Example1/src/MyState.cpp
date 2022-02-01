#include "MyState.h"

MyState::MyState(IntArray4d s)
{
    triagePopulations_ = s;
}

MyState::MyState(const int a, const int b, const int c, const int d)
{
    triagePopulations_ = {a,b,c,d};
}

MyState::~MyState()
{

}

IntArray4d 
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