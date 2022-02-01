#include "MyDecision.h"

MyDecision::MyDecision(const int qc)
{
    queueChoice_ = qc;
}

MyDecision::~MyDecision()
{

}

std::string 
MyDecision::ToString() const
{
    return std::to_string(queueChoice_);
}

int 
MyDecision::GetQueueChoice() const
{
    return queueChoice_;
}

bool 
MyDecision::operator== (const Decision& d)
{
   return false;
}