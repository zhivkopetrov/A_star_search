//Corresponding header
#include "TimerMgr.h"

#include <cstdlib>

//Other libraries headers

//Own components headers
#include "utils/time/TimerClient.h"
#include "utils/Log.h"

TimerMgr *gTimerMgr = nullptr;

int32_t TimerMgr::init() {
  return EXIT_SUCCESS;
}

void TimerMgr::deinit() {
}

const char* TimerMgr::getName() {
  return "TimerMgr";
}

void TimerMgr::process() {
  const int64_t millisecondsElapsed =
      _timeInternal.getElapsed().toMilliseconds();

  //do the loop update by hand so we can safely remove elements
  for (auto it = _timerMap.begin(); it != _timerMap.end(); ++it) {
    it->second.remaining -= millisecondsElapsed;
    if (0 > it->second.remaining) {
      onTimerTimeout(it->first, it->second);
    }
  }

  //check for timers that requested external closing
  removeTimersInternal();
}

void TimerMgr::startTimer(TimerClient *tcIstance,
                          const int64_t interval,
                          const int32_t timerId,
                          const TimerType timerType) {
  //The check for isActiveTimerId is invoked from TimerClient class

  //at start the remaining interval is equal to whole interval
  const TimerData timerData(interval,   //original interval
                            interval,   //remaining interval
                            timerType,  //ONESHOT or PULSE
                            tcIstance); //TimerClient instance

  _timerMap.emplace(timerId, timerData);
}

void TimerMgr::stopTimer(const int32_t timerId) {
  if (isActiveTimerId(timerId)) {
    _removeTimerSet.insert(timerId);
  } else {
    LOGERR(
        "Warning, trying to remove a non-existing timer with ID: %d."
        " Be sure to check your timerId with .isActiveTimerId(timerId) "
        "before calling .stopTimer(timerId)", timerId);
  }
}

void TimerMgr::stopTimerAndDetachTimerClient(const int32_t timerId) {
  /** NOTE: timer could have already been stopped in some ::deinit() func
   * which will lead for it to be present in the _removeTimerSet so
   * invoking of ::isActiveTimer() here will result in false -> leading to
   * no detaching of the TimerClient instance.
   * Instead just search whether the timer is present in the timerMap
   * */
  if (isTimerLocatedInTheTimerMap(timerId)) {
    //but be sure to add it to the _removeTimerSet though :)
    _removeTimerSet.insert(timerId);

    /** isActiveTimerId() already assured that such key exists.
     *  Now, detach TimerClient instance, because it is about to be
     *                              destroyed by TimerClient desctructor.
     * */
    _timerMap[timerId].tcInstance = nullptr;
  } else {
    LOGERR(
        "Warning, trying to remove a non-existing timer with ID: %d."
        " Be sure to check your timerId with .isActiveTimerId(timerId) "
        "before calling .stopTimer(timerId)", timerId);
  }
}

void TimerMgr::onTimerTimeout(const int32_t timerId, TimerData &timerData) {
  if (_removeTimerSet.end() != _removeTimerSet.find(timerId)) {
    /** Someone could have requested external closure of the timer
     * in this case do not attempt to execute callback, because it could be
     * invalid.
     * Example: TimerClient() destructor asked for stopTimer() and it's
     * instance is already destroyed
     * */
    return;
  }

  timerData.tcInstance->onTimeout(timerId);

  if (timerData.timerType == TimerType::ONESHOT) {
    //If timer was on TimerType::ONESHOT it should close on it's own
    _removeTimerSet.insert(timerId);

    return;
  }

  //at this point remaining will be zero or negative value
  //we need to restart the remaining time while taking into account
  //that the timer probably postponed the original entered user interval
  //Example: timerData.interval = 5000ms;
  //process() is called every 2000ms.
  //after first process()  -> remaining interval is +3000ms;
  //after second process() -> remaining interval is +1000ms;
  //after third process()  -> remaining interval is -1000ms;
  //at this point execute function callback and if TimerTime is set to PULSE
  //restart timer's remaining interval to original interval (5000ms)
  //minus the postponed period (-1000ms) ->
  //new remaining interval value is 5000ms - 1000ms = 4000ms
  timerData.remaining += timerData.interval;
}

void TimerMgr::removeTimersInternal() {
  //set is empty -> no timers requested external closing
  if (_removeTimerSet.empty()) {
    return;
  }

  for (const int32_t timerId : _removeTimerSet) {
    auto mapIt = _timerMap.find(timerId);

    //key not found
    if (mapIt == _timerMap.end()) {
      LOGERR(
          "Warning, trying to remove a non-existing timer with ID: %d."
          " Be sure to check your timerId with .isActiveTimerId(timerId)"
          " before calling .stopTimer(timerId)", timerId);
    } else //key found
    {
      //check if TimerClient instance is still active
      if (nullptr != mapIt->second.tcInstance) {
        //send signal to TimerClient instance to remove timerId
        //from it's list of managed timers

        if (EXIT_SUCCESS != mapIt->second.tcInstance->removeTimerIdFromList(
                mapIt->first)) {
          LOGERR("Warning, internal error in removeTimerIdFromList() with "
              "timerId: %d", mapIt->first);
        }
      }

      //erase the timer from the _timerMap
      _timerMap.erase(mapIt);
    }
  }

  //clear the removeTimerSet
  _removeTimerSet.clear();
}

void TimerMgr::onInitEnd() {
  //reset the timer so it can clear the "stored" time since the creation
  //of the TimerMgr instance and this function call
  _timeInternal.getElapsed();
}

