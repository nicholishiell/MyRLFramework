#include "MyDecision.h"

MyDecision::MyDecision(ShortArray2d d)
{
    individualsToEvac_ = d;
}

MyDecision::MyDecision(const uShort a, const uShort b)
{
    individualsToEvac_ = {a,b};
}

MyDecision::~MyDecision()
{

}

std::string 
MyDecision::ToString() const
{
    return Util::ToString(individualsToEvac_);
}

ShortArray2d 
MyDecision::GetDecisionArray() const
{
    return individualsToEvac_;
}

int 
MyDecision::GetDecisionArrayElement(const int i) const
{
    int retVal = -1;

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

    return arrayB[0] == arrayA[0] && arrayB[1] == arrayA[1];
}

DecisionSharedPtr 
MyDecision::clone() const
{
    return std::make_shared<MyDecision>(*this);
}