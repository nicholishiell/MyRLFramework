#ifndef MY_UTIL_H
#define MY_UTIL_H

#include "MyState.h"

namespace MyUtil
{
  const DoubleArray4d transitionProbs_ = {0.025, 0.061, 0.313, 0.865}; 
  const uShort CRITICAL_SPACE = 3;
  const uShort NONCRITICAL_SPACE = 1;
  const ShortArray4d  spaceRequirements_ = {NONCRITICAL_SPACE,
                                            NONCRITICAL_SPACE,
                                            CRITICAL_SPACE,
                                            CRITICAL_SPACE};
  const uShort heloCapacity_ = 6;

  class StateHash 
  {
  public:
      size_t operator()(const StateSharedPtr& s) const
      {
          const auto sCast = static_cast<const MyState&>(*s);
          const auto a = sCast.GetTriagePopulations();

          std::size_t h = 0;

          for(auto e : a) 
          {
              h ^= std::hash<int>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
          }

          return h;
      }
  };

  class StateEqual
  {
  public:

    bool operator()(const StateSharedPtr& lhs,
                    const StateSharedPtr& rhs) const
    {
      const auto arrayB = std::dynamic_pointer_cast<MyState>(lhs)->GetTriagePopulations();
      const auto arrayA = std::dynamic_pointer_cast<MyState>(rhs)->GetTriagePopulations();

      bool test = arrayB[0] == arrayA[0] && 
                  arrayB[1] == arrayA[1] &&
                  arrayB[2] == arrayA[2] &&
                  arrayB[3] == arrayA[3];
    
      return test;
    }
  };
};

#endif