#ifndef MY_UTIL_H
#define MY_UTIL_H

#include "MyState.h"

namespace MyUtil
{

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
                  arrayB[1] == arrayA[1];
    
      return test;
    }
  };

};

#endif