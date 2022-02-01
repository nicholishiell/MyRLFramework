#include <string>
#include <fstream>

#include "Util.h"

namespace Util
{
   std::string ToString(const IntArray4d a)
   {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";

        return str;
   }

   int SumArray(IntArray4d array)
   {
        int sum = 0;

        for(const auto value : array)
            sum += value;

        return sum;
   }

   std::string ToString(const IntArray2d a)
   {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";

        return str;
   }

    std::string ToString(const BoolArray2d a)
    {
        std::string str = "(";
       
        for(int i = 0; i < a.size(); i++)
        {    
            str += std::to_string(a[i]);

            if(i < a.size()-1) str += ",";
        }
        
        str += ")";

        return str;
    }

   int SumArray(IntArray2d array)
   {
        int sum = 0;

        for(const auto value : array)
            sum += value;

        return sum;
   }
}