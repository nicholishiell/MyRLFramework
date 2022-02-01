#ifndef MY_STATE_H
#define MY_STATE_H

#include <memory>

#include "Util.h"
#include "State.h"

class MyState : public State
{
public:

    MyState(IntArray4d s);

    MyState(const int a, const int b, const int c, const int d);

    ~MyState();

    IntArray4d GetTriagePopulations() const;
    
    int GetTriagePopulations(int i) const;

    std::string ToString() const;

private:

    IntArray4d triagePopulations_;
};

using MyStateSharedPtr = std::shared_ptr<MyState>;

#endif