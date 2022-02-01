#ifndef MY_STATE_H
#define MY_STATE_H

#include <memory>

#include "Util.h"
#include "State.h"

class MyState : public State
{
public:

    MyState(IntArray2d q);

    MyState(const int q1, const int q2);

    ~MyState();

    std::string ToString() const;

    IntArray2d GetQueuePopulations() const;
    int GetQueuePopulations(int i) const;

private:

    IntArray2d queuePopulations_;
};

using MyStateSharedPtr = std::shared_ptr<MyState>;

#endif