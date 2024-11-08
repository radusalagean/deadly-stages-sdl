#ifndef __SRC_CORE_DEBOUNCER_HPP__
#define __SRC_CORE_DEBOUNCER_HPP__

#include <chrono>
#include <unordered_map>

class Debouncer 
{
private:
    std::chrono::milliseconds debounceTime;
    std::unordered_map<int, std::chrono::steady_clock::time_point> lastActionTimes;

public:
    Debouncer(int debounceTime = 500) : 
        debounceTime(debounceTime), 
        lastActionTimes() {}

    bool canPerformAction(int action = -1) 
    {
        auto now = std::chrono::steady_clock::now();
        if (now - lastActionTimes[action] >= debounceTime) 
        {
            lastActionTimes[action] = now;
            return true;
        }
        return false;
    }

    void resetAction(int action = -1)
    {
        lastActionTimes.erase(action);
    }
};


#endif // __SRC_CORE_DEBOUNCER_HPP__

