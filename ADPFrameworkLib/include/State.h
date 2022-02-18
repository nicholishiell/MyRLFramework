#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <string>

#include "Util.h"
#include "Decision.h"

using namespace Util;

class State
{
public:

    State();
    ~State();
    
    virtual bool IsDecisionLegal(const DecisionSharedPtr& d) const;    
  
    virtual std::string ToString() const;

    virtual bool operator== (const StateSharedPtr s) const;
 
    virtual StateSharedPtr clone() const;
    
private:

};

#endif