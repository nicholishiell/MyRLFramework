#ifndef MY_DECISION_H
#define MY_DECISION_H

#include <iostream>
#include <array>

#include "Decision.h"

class MyDecision : public Decision
{
public:

    MyDecision(const int qc);
  
    ~MyDecision();

    std::string ToString() const;

    int GetQueueChoice() const;

    bool operator== (const Decision& d);

private:

    int queueChoice_;

    Aircaraft* a;
    TaskType type_; // refuel, fly, etc
    double completitionTime;
};

using MyDecisionSharedPtr = std::shared_ptr<MyDecision>;

#endif