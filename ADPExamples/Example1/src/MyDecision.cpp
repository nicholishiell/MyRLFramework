#include "MyDecision.h"

MyDecision::MyDecision(std::array<int,4> d)
{
    individualsToEvac_ = d;
}

MyDecision::MyDecision(const int a, const int b, const int c, const int d)
{
    individualsToEvac_ = {a,b,c,d};
}

MyDecision::~MyDecision()
{

}

std::string 
MyDecision::ToString() const
{
    return Util::ToString(individualsToEvac_);
}

IntArray4d 
MyDecision::GetDecisionArray() const
{
    return individualsToEvac_;
}

bool 
MyDecision::operator== (const Decision& d)
{
    const auto dCast = dynamic_cast<const MyDecision&>(d);
    const auto arrayB = dCast.GetDecisionArray();
    const auto arrayA = this->GetDecisionArray();

    return arrayB[0] == arrayA[0] && arrayB[1] == arrayA[1] && arrayB[2] == arrayA[2] && arrayB[3] == arrayA[3];
}