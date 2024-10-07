#ifndef __SRC_CORE_VECTORUTILS_HPP__
#define __SRC_CORE_VECTORUTILS_HPP__

#include <vector>
#include <algorithm>

namespace VectorUtils
{
    template <typename T, typename Predicate>
    void removeFromVectorIf(std::vector<T>& vec, Predicate predicate) 
    {
        vec.erase(std::remove_if(vec.begin(), vec.end(), predicate), vec.end());
    }
}

#endif // __SRC_CORE_VECTORUTILS_HPP__
