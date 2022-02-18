#ifndef MY_VF_H
#define MY_VF_H

#include <memory>
#include <array>

#include "Util.h"
#include "ValueFunction.h"

#include "MyState.h"
#include "MyDecision.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class StateDecisionPair: public LUTKey
{

public:
    
    StateDecisionPair(  const StateSharedPtr state, 
                        const DecisionSharedPtr decision);

    bool operator==(const LUTKey& p) const;

    int GetArrayElement(const int i) const;
    
    std::array<uShort,8> GetArray() const;

    std::string ToString() const;

private:

    std::array<uShort,8> stateDecisionPair_;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class StateDecisionHash : public LUTHash
{
public:
    size_t operator()(const LUTKey& k) const;

private:

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class StateDecisionEqual : public LUTEqual
{
public:
    bool operator()(const LUTKey& lhs,
                    const LUTKey& rhs) const
    {
        const auto lhsKeyCast = static_cast<const StateDecisionPair&>(lhs);
        const auto rhsKeyCast = static_cast<const StateDecisionPair&>(rhs);
        const auto lhsArray = lhsKeyCast.GetArray();
        const auto rhsArray = rhsKeyCast.GetArray();

        bool test = true;

        for(int i = 0; i < lhsArray.size(); i++)
        {
            test = test && (lhsArray[i] == rhsArray[i]);
        }        
        
        return test;
    }

private:

};
#endif