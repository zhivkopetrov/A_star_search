//Corresponding header
#include "Time.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

//Other libraries headers

//Own components headers
#include "utils/Log.h"

Time::Time() {
  if (EXIT_SUCCESS != clock_gettime(CLOCK_MONOTONIC, &_elapsedLastCall)) {
    LOGERR("clock_gettime() failed, reason: %s", strerror(errno));
  }

  _elapsed.tv_sec = 0;
  _elapsed.tv_nsec = 0;
}

Time& Time::getElapsed() {
  timespec now;

  if (EXIT_SUCCESS != clock_gettime(CLOCK_MONOTONIC, &now)) {
    LOGERR("clock_gettime() failed, reason: %s", strerror(errno));
  }

  _elapsed.tv_sec = now.tv_sec - _elapsedLastCall.tv_sec;
  _elapsed.tv_nsec = now.tv_nsec - _elapsedLastCall.tv_nsec;

  _elapsedLastCall.tv_sec = now.tv_sec;
  _elapsedLastCall.tv_nsec = now.tv_nsec;

  if (0 > _elapsed.tv_nsec) {
    _elapsed.tv_nsec += SECOND;
    --_elapsed.tv_sec;
  }

  return *this;
}

