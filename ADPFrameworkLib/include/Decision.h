#ifndef DECISION_H
#define DECISION_H

#include <iostream>
#include <string>

#include "Util.h"

using namespace Util;

class Decision
{
public:

    Decision(bool isNull = false);
    ~Decision();

    virtual bool operator== (const Decision& d) const;

    virtual std::string ToString() const;

    virtual DecisionSharedPtr clone() const;

private:

    bool isNull_;
};

#endif