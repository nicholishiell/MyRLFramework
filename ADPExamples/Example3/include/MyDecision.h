#ifndef MY_DECISION_H
#define MY_DECISION_H

#include <iostream>
#include <array>

#include "Decision.h"

class MyDecision : public Decision
{
public:

    MyDecision(IntArray2d d);

    MyDecision(const int a, const int b);
  
    ~MyDecision();

    std::string ToString() const;

    IntArray2d GetDecisionArray() const;
    int GetDecisionArrayElement(const int i) const;

    bool operator== (const Decision& d);

    DecisionSharedPtr clone() const;

private:

    IntArray2d individualsToEvac_;
};

using MyDecisionSharedPtr = std::shared_ptr<MyDecision>;

#endif