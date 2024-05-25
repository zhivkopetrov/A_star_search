//Corresponding header
#include "TimerClient.h"

//C system headers
#include <cstdlib>

//C++ system headers

//Other libraries headers

//Own components headers
#include "managers/TimerMgr.h"

#include "utils/LimitValues.hpp"
#include "utils/Log.h"

//Hold step for increase of the limit for currently auto-managed timers
#define RESIZE_STEP 5

#define MAX_TIMERS 30

//default constructor
TimerClient::TimerClient()
    : _timerIdList(nullptr), _currTimerCount(0), _maxTimersCount(0) {

}

//move constructor
TimerClient::TimerClient(TimerClient &&movedOther) {
  //scan linear and search for a started timers
  for (int32_t i = 0; i < movedOther._maxTimersCount; ++i) {
    if (INIT_INT32_VALUE != movedOther._timerIdList[i]) {
      //started timer found -> stop the search
      LOGERR(
          "Warning, TimerClient instance is being moved while "
          "there are active timers attached to it. This is an "
          "illegal operation.");

      break;
    }
  }

  //take ownership of the resources
  _timerIdList = movedOther._timerIdList;
  _currTimerCount = movedOther._currTimerCount;
  _maxTimersCount = movedOther._maxTimersCount;

  //old entity should release the resources
  movedOther._timerIdList = nullptr;
  movedOther._currTimerCount = 0;
  movedOther._maxTimersCount = 0;
}

//move assignment operator
TimerClient& TimerClient::operator=(TimerClient &&movedOther) {
  if (this != &movedOther) {
    //scan linear and search for a started timers
    for (int32_t i = 0; i < movedOther._maxTimersCount; ++i) {
      if (INIT_INT32_VALUE != movedOther._timerIdList[i]) {
        //started timer found -> stop the search
        LOGERR(
            "Warning, TimerClient instance is being moved while "
            "there are active timers attached to it. This is an "
            "illegal operation.");

        break;
      }
    }

    //take ownership of the resources
    _timerIdList = movedOther._timerIdList;
    _currTimerCount = movedOther._currTimerCount;
    _maxTimersCount = movedOther._maxTimersCount;

    //old entity should release the resources
    movedOther._timerIdList = nullptr;
    movedOther._currTimerCount = 0;
    movedOther._maxTimersCount = 0;
  }

  return *this;
}

TimerClient::~TimerClient() {
  //TimerClient is about to be destroyed -> stop all it's timers.
  for (int32_t i = 0; i < _maxTimersCount; ++i) {
    //search through non-empty slots
    if (INIT_INT32_VALUE != _timerIdList[i]) {
      //sanity check
      if (nullptr != gTimerMgr) {
        gTimerMgr->stopTimerAndDetachTimerClient(_timerIdList[i]);
      }
    }
  }

  //clean dynamically created resources
  if (_timerIdList) //sanity check
  {
    delete[] _timerIdList;
    _timerIdList = nullptr;
  }
}

void TimerClient::startTimer(const int64_t interval, const int32_t timerId,
                             const TimerType timerType) {
  //if timer already exists -> do not start it
  if (gTimerMgr->isActiveTimerId(timerId)) {
    LOGERR(
        "Warning, timer with ID: %d already exist. "
        "Will not start new timer", timerId);

    return;
  }

  if (interval < 20) {
    LOGERR(
        "Warning, timer with timerId: %d requested startTimer() with "
        "interval %ld, while minimum interval is 20ms. Timer will not "
        "be started!",
        timerId, interval);

    return;
  }

  //check if maximum active timers count is reached
  if (_currTimerCount == _maxTimersCount) {
    if (MAX_TIMERS == _currTimerCount) {
      LOGERR(
          "Warning, Maximum possible active timers: %d for current "
          "TimerClient instance was reached. Timer with ID: %d could "
          "not be started!",
          MAX_TIMERS, timerId);

      return;
    }

    if (EXIT_SUCCESS != resizeTimerList()) {
      LOGERR("Warning, timer with ID: %d could not be started", timerId);

      return;
    }
  }

  int32_t freeIndex = INIT_INT32_VALUE;

  //scan linear and search for a free slot
  for (int32_t i = 0; i < _maxTimersCount; ++i) {
    if (INIT_INT32_VALUE == _timerIdList[i]) {
      //free slot found -> stop the search
      freeIndex = i;

      break;
    }
  }

  _timerIdList[freeIndex] = timerId;
  ++_currTimerCount;

  gTimerMgr->startTimer(this,       //TimerClient instance
                        interval,   //interval
                        timerId,    //remaining interval
                        timerType); //timer type
}

void TimerClient::stopTimer(const int32_t timerId) {
  gTimerMgr->stopTimer(timerId);
}

bool TimerClient::isActiveTimerId(const int32_t timerId) const {
  return gTimerMgr->isActiveTimerId(timerId);
}

int32_t TimerClient::resizeTimerList() {
  //increase maximum timers
  _maxTimersCount += RESIZE_STEP;

  //allocate buffer for the new size
  int32_t *increasedList = new int32_t[_maxTimersCount];
  if (!increasedList) {
    LOGERR("Error, bad alloc for _timerIdList");

    _maxTimersCount -= RESIZE_STEP;
    return EXIT_FAILURE;
  }

  //copy timer id's from the smaller buffer
  for (int32_t i = 0; i < _currTimerCount; ++i) {
    increasedList[i] = _timerIdList[i];
  }

  //set new timerID slots to its starting value
  for (int32_t i = _currTimerCount; i < _maxTimersCount; ++i) {
    increasedList[i] = INIT_INT32_VALUE;
  }

  //free resources from the smaller buffer
  delete[] _timerIdList;

  //point timerIdList to new bigger buffer
  _timerIdList = increasedList;

  increasedList = nullptr;

  return EXIT_SUCCESS;
}

int32_t TimerClient::removeTimerIdFromList(const int32_t timerId) {
  int32_t err = EXIT_SUCCESS;

  bool foundTimer = false;

  for (int32_t i = 0; i < _maxTimersCount; ++i) {
    if (timerId == _timerIdList[i]) {
      foundTimer = true;
      --_currTimerCount;                  //lower total timers count
      _timerIdList[i] = INIT_INT32_VALUE; //free the slot
      break;                              //and end the search
    }
  }

  if (false == foundTimer) {
    err = EXIT_FAILURE;
  }

  return err;
}

