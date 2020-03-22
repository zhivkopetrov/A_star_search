#ifndef UTILS_TIME_TIMERCLIENT_H_
#define UTILS_TIME_TIMERCLIENT_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "TimerClientStructs.h"

class TimerClient {
public:
  TimerClient();
  virtual ~TimerClient();

  //forbid the copy constructor and copy assignment operator
  TimerClient(const TimerClient &movedOther) = delete;
  TimerClient& operator=(TimerClient &movedOther) = delete;

  //move constructor
  TimerClient(TimerClient &&movedOther);

  //move assignment operator
  TimerClient& operator=(TimerClient &&movedOther);

  /** @brief Callback on Timer timeout
   *
   *  @param const int32_t - timerId, which made the callback
   * */
  virtual void onTimeout(const int32_t timerId) = 0;

  /** @brief starts timer with provided arguments
   *    this functions does not return error code for performance reasons
   *
   *  @param const int64_t    - time (in milliseconds) after which
   *                                     the timer Timeout will be called
   *  @param const int32_t    - unique timer ID
   *  @param const TimerType  - ONESHOT(single tick) or
   * */
  void startTimer(const int64_t interval, const int32_t timerId,
                  const TimerType timerType);

  /** @brief stops timer with specified timerId (if such timer exits)
   *
   *  @param const int32_t - unique timerID
   * */
  void stopTimer(const int32_t timerId);

  /** @brief checks if timer with specific Id is activated
   *
   *  @param const int32_t - unique timerID
   *
   *  @returns bool - is active timer or not
   * */
  bool isActiveTimerId(const int32_t timerId) const;

  /** @brief used to remove timerId from list of managed timers
   *         NOTE: this function should be used only by TimerMgr itself.
   *         Do not call this function. Instead use .stopTimer(timerId).
   *
   *  @param const int32_t unique timerID
   *
   *  @returns int32_t - error code
   * */
  int32_t removeTimerIdFromList(const int32_t timerId);

private:
  /** Since TimerClient header will be included a lot -> try not to
   *  include heavy includes such as std::unordered_set.
   *  This will heavily influence the compile time for every file
   *  that includes the TimerClient header.
   *  */

  /** @brief used to resize the managed timer list for this specific
   *                                              TimerClient instance
   *
   *  @returns int32_t - error code
   * */
  int32_t resizeTimerList();

  /** Timer ID list for auto-clean up of timers
   *  (to automatically stop all it's timers when TimerClient instance
   *  gets destroyed)
   *  */
  int32_t *_timerIdList;

  //Holds counter for current active timers
  int32_t _currTimerCount;

  //Hold limit for currently auto-managed timers
  int32_t _maxTimersCount;
};

#endif /* UTILS_TIME_TIMERCLIENT_H_ */

