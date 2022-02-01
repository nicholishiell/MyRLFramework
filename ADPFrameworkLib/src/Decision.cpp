#include "Decision.h"

Decision::Decision(bool isNull)
{
    isNull_ = isNull;
}

Decision::~Decision()
{

}

std::string 
Decision::ToString() const
{
    std::cout << "[WARNING] Decision base class ToString methods used..." << std::endl;

    return "";
}

bool 
Decision::operator== (const Decision& d) const
{
    std::cout << "[WARNING] Decision base class == operator used..." << std::endl;
    return true;
}

DecisionSharedPtr 
Decision::clone() const
{
    std::cout << "[WARNING] Decision base class clone method used..." << std::endl;
    return nullptr;    
}