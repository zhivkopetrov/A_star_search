#ifndef UTILS_TIME_TIMERCLIENTDEFINES_H_
#define UTILS_TIME_TIMERCLIENTDEFINES_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations
class TimerClient;

enum class TimerType : uint8_t
{
    UNKNOWN = 0,
    ONESHOT = 1,
    PULSE   = 2
};

struct TimerData
{
    TimerData() : interval(0), remaining(0), timerType(TimerType::UNKNOWN),
        tcInstance(nullptr) { }

    explicit TimerData(const int64_t        inputInterval,
                       const int64_t        inputRemaining,
                       const TimerType      inputTimerType,
                       TimerClient*         inputTcInstance) :
                               interval(inputInterval),
                               remaining(inputRemaining),
                               timerType(inputTimerType),
                               tcInstance(inputTcInstance) {}

    int64_t      interval;       //original interval
    int64_t      remaining;      //remaining interval
    TimerType    timerType;      //ONESHOT or PULSE
    TimerClient* tcInstance;     //TimerClient instance
};

#endif /* UTILS_TIME_TIMERCLIENTDEFINES_H_ */


