#ifndef MANAGERS_TIMERMGR_H_
#define MANAGERS_TIMERMGR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <set>
#include <map>

//Other libraries headers

//Own components headers
#include "MgrBase.h"

#include "utils/time/Time.h"
#include "utils/time/TimerClientStructs.h"

//Forward declarations
class TimerClient;

class TimerMgr: public MgrBase {
public:
  TimerMgr() = default;

  //forbid the copy and move constructors
  TimerMgr(const TimerMgr &other) = delete;
  TimerMgr(TimerMgr &&other) = delete;

  //forbid the copy and move assignment operators
  TimerMgr& operator=(const TimerMgr &other) = delete;
  TimerMgr& operator=(TimerMgr &&other) = delete;

  /** @brief used to initialize the current manager.
   *         NOTE: this is the first function that will be called.
   *
   *  @return int32_t - error code
   * */
  int32_t init() override;

  /** @brief used to deinitialize the current manager.
   * */
  void deinit() override;

  /** @brief used to process the current manager (poll him on every
   *         engine cycle so the managers can do any internal updates, if
   *                                                     such are needed).
   * */
  void process() override;

  /** @brief returns the name of the current manager
   *
   *  @return const char * - current manager name
   * */
  const char* getName() override;

  /** @brief starts timer with provided arguments
   *    this functions does not return error code for performance reasons
   *
   *  @param TimerClient *    - instance of TimerClient, which
   *                                  onTimeout() function will be called
   *  @param const int64_t    - time (in milliseconds) after which
   *                                     the timer Timeout will be called
   *  @param const int32_t    - unique timer ID
   *  @param const TimerType  - ONESHOT(single tick) or
   *                            PULSE(constant ticks)
   * */
  void startTimer(TimerClient *tcIstance, const int64_t interval,
                  const int32_t timerId, const TimerType timerType);

  /** @brief stops timer with specified timerId (if such timer exits)
   *
   *  @param const int32_t - unique timerID
   * */
  void stopTimer(const int32_t timerId);

  /** @brief stops timer with specified timerId (if such timer exits)
   *         and detach TimerClient instance, because it is about to be
   *                                                           destroyed.
   *
   *  @param const int32_t - unique timerID
   * */
  void stopTimerAndDetachTimerClient(const int32_t timerId);

  /** @brief checks if timer with specific Id is activated
   *
   *  @param const int32_t - unique timerID
   *
   *  @returns bool - is active timer or not
   * */
  inline bool isActiveTimerId(const int32_t timerId) const {
    return (_removeTimerSet.end() == _removeTimerSet.find(timerId))
        && (_timerMap.end() != _timerMap.find(timerId));
  }

  /** @brief must be called on project init() method end in order to
   *         set proper internal TimerMgr calculations */
  void onInitEnd();

private:
  /** @brief calls timer onTimeout callback function and
   *                                resets the timer (if TimerType::PULSE)
   *
   *  @param const int32_t - unique timerID
   *  @param TimerData &   - structure that holds timer specific data
   * */
  void onTimerTimeout(const int32_t timerId, TimerData &timerData);

  /** @brief used to remove timers from the _timerMap that are contained
   *                                                  in _removeTimerSet.
   * */
  void removeTimersInternal();

  /** @brief used to search whether the specific timer is present in the
   *         timerMap
   *
   *  @param const int32_t unique timerID
   *
   *  @return bool - is timer present in the timerMap or not
   * */
  inline bool isTimerLocatedInTheTimerMap(const int32_t timerId) const {
    return _timerMap.end() != _timerMap.find(timerId);
  }

  /** Used to measure elapsed time and update _timerMap
   *                                               on every engine cycle
   * */
  Time _timeInternal;

  /** @brief a map that holds all active timers
   *          std::map is used since we'll be constantly iterating over it
   *          and we need to keep it sorted in order to give priority
   *          to system timers /they have lower unique ID's and therefore
   *                                          they will be iterated first/
   *
   *  @param int32_t   - unique timerID
   *  @param TimerData - structure that holds timer specific data
   * */
  std::map<int32_t, TimerData> _timerMap;

  /** @brief a set that holds all timers that requested external closing
   *                                           /with .stopTimer(timerId)/
   * */
  std::set<int32_t> _removeTimerSet;
};

extern TimerMgr *gTimerMgr;

#endif /* MANAGERS_TIMERMGR_H_ */

