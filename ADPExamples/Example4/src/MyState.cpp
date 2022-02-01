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