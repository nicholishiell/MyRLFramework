#ifndef MY_DECISION_H
#define MY_DECISION_H

#include <iostream>
#include <array>

#include "Decision.h"

class MyDecision : public Decision
{
public:

    MyDecision(ShortArray2d d);

    MyDecision(const uShort a, const uShort b);
  
    ~MyDecision();

    std::string ToString() const;

    ShortArray2d GetDecisionArray() const;
    int GetDecisionArrayElement(const int i) const;

    bool operator== (const Decision& d);

    DecisionSharedPtr clone() const;

private:

    ShortArray2d individualsToEvac_;
};

using MyDecisionSharedPtr = std::shared_ptr<MyDecision>;

#endif