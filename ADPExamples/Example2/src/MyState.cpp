#include "MyState.h"

MyState::MyState(IntArray2d qs)
{
    queuePopulations_ = qs;
}


MyState::MyState(const int q1, const int q2)
{
    queuePopulations_ = {q1,q2};
}

MyState::~MyState()
{

}

IntArray2d 
MyState::GetQueuePopulations() const
{
    return queuePopulations_;
}

int 
MyState::GetQueuePopulations(int i) const
{
    return queuePopulations_[i];
}

std::string 
MyState::ToString() const
{
    return Util::ToString(queuePopulations_);
}    