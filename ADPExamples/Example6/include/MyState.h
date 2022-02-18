#ifndef MY_STATE_H
#define MY_STATE_H

#include <memory>

#include "Util.h"
#include "State.h"

class MyState : public State
{
public:

    MyState(const ShortArray2d& s);

    MyState(const uShort a, const uShort b);

    ~MyState();

    ShortArray2d GetTriagePopulations() const;
    
    int GetTriagePopulations(int i) const;

    std::string ToString() const;

private:

    ShortArray2d triagePopulations_;
};

using MyStateSharedPtr = std::shared_ptr<MyState>;

#endif