#ifndef MY_STATE_H
#define MY_STATE_H

#include <memory>

#include "Util.h"
#include "State.h"

class MyState : public State
{
public:

    MyState(ShortArray4d s);

    MyState(const uShort a, const uShort b, const uShort c, const uShort d);

    ~MyState();

    bool IsDecisionLegal(const DecisionSharedPtr& d) const;

    ShortArray4d GetTriagePopulations() const;
    
    uShort GetTriagePopulations(int i) const;

    std::string ToString() const;

    bool operator== (const State& s) const;

    StateSharedPtr clone() const;

private:

    ShortArray4d triagePopulations_;
};

using MyStateSharedPtr = std::shared_ptr<MyState>;

#endif