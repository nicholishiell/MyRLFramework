#ifndef MY_DECISION_H
#define MY_DECISION_H

#include <iostream>
#include <array>

#include "Decision.h"

class MyDecision : public Decision
{
public:

    MyDecision(std::array<int,4> d);

    MyDecision(const int a, const int b, const int c, const int d);
  
    ~MyDecision();

    std::string ToString() const;

    IntArray4d GetDecisionArray() const;

    bool operator== (const Decision& d);

private:

    IntArray4d individualsToEvac_;
};

using MyDecisionSharedPtr = std::shared_ptr<MyDecision>;

#endif