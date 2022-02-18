#include "Util.h"
#include "Policy.h"


Policy::Policy()
{

}

Policy::~Policy()
{

}

DecisionSharedPtr 
Policy::operator() (const StateSharedPtr s) const
{
    std::cout << "[WARNING] Policy base class () operator used..." << std::endl;
    return std::make_shared<Decision>();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

