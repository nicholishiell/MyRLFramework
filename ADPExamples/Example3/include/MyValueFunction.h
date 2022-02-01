#ifndef MY_VF_H
#define MY_VF_H

#include <memory>
#include <array>

#include "ValueFunction.h"

#include "MyState.h"
#include "MyDecision.h"

class StateDecisionPair: public LUTKey
{

public:
    
    StateDecisionPair(const StateSharedPtr state, const DecisionSharedPtr decision);

    bool operator==(const LUTKey& p) const;

    int GetArrayElement(const int i) const;
    
    std::array<int,4> GetArray() const;

private:

    std::array<int,4> stateDecisionPair_;
};

class StateDecisionHash : public LUTHash
{
public:
    size_t operator()(const LUTKey& k) const;

private:

};

#endif