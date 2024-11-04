#include "MathUtils.hpp"

#include <cmath>
#include <algorithm>

namespace MathUtils
{
    int getPercentFromUnitInterval(float unitInterval)
    {
        int percent = round(unitInterval * 100);
        percent = std::max(0, std::min(percent, 100));
        return percent;
    }
};