#include "MyDecision.h"

MyDecision::MyDecision(ShortArray4d d)
{
    individualsToEvac_ = d;
}

MyDecision::MyDecision(const uShort a, const uShort b, const uShort c, const uShort d)
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

ShortArray4d 
MyDecision::GetDecisionArray() const
{
    return individualsToEvac_;
}

uShort
MyDecision::GetDecisionArrayElement(const int i) const
{
    uShort retVal = -1;

    if(i < individualsToEvac_.size())
    {
        retVal = individualsToEvac_[i];
    }
    else
    {
        std::cout << "[ERROR]: index exceeds array size..." << i << " > " << individualsToEvac_.size() << std::endl;
    }

    return retVal;
}

bool 
MyDecision::operator== (const Decision& d)
{
    const auto dCast = dynamic_cast<const MyDecision&>(d);
    const auto arrayB = dCast.GetDecisionArray();
    const auto arrayA = this->GetDecisionArray();

    return  arrayB[0] == arrayA[0] && 
            arrayB[1] == arrayA[1] &&
            arrayB[2] == arrayA[2] &&
            arrayB[3] == arrayA[3];
}

DecisionSharedPtr 
MyDecision::clone() const
{
    return std::make_shared<MyDecision>(*this);
}