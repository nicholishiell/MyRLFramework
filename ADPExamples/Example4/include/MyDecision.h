#ifndef MY_DECISION_H
#define MY_DECISION_H

#include <iostream>
#include <array>

#include "Decision.h"

class MyDecision : public Decision
{
public:

    MyDecision(ShortArray4d d);

    MyDecision(const uShort a, const uShort b, const uShort c, const uShort d);
  
    ~MyDecision();

    std::string ToString() const;

    ShortArray4d GetDecisionArray() const;
    uShort GetDecisionArrayElement(const int i) const;

    bool operator== (const Decision& d);

    DecisionSharedPtr clone() const;

private:

    ShortArray4d individualsToEvac_;
};

using MyDecisionSharedPtr = std::shared_ptr<MyDecision>;

#endif