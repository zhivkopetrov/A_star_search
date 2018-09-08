/*
 * Time.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef COMMON_TIME_H_
#define COMMON_TIME_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <ctime>

//Other libraries headers

//Own components headers

enum TimeUnits
{
    SECOND      = 1000000000,
    MICROSECOND = 1000000,
    MILLISECOND = 1000,
    NANOSECOND  = 1
};


class Time
{
    public:
        Time();
        virtual ~Time() = default;

        /** @brief used to measure elapsed time since last invoke of this method
         *         NOTE: .getElapsed() method simply measures time between 2
         *         points of time. In order to display this time chain the
         *         function with one of the below display functions.
         *         Example:
         *         Time time;
         *
         *         //something you want to measure
         *
         *         const int64_t elapsedTime =
         *                                   time.getElapsed().toMilliseconds();
         *
         *  @returns Time & - *this instance
         * */
        Time & getElapsed();

        /** @brief used to display the elapsed time between two calls of the
         *         .getElapsed() method in nanoseconds
         *
         *  @returns int32_t - elapsed time in nanoseconds
         * */
        inline int32_t toNanoseconds()
        {
            return static_cast<int32_t>(
            				_elapsed.tv_sec * NANOSECOND + _elapsed.tv_nsec);
        }

        /** @brief used to display the elapsed time between two calls of the
         *         .getElapsed() method in microseconds
         *
         *  @returns int32_t - elapsed time in microseconds
         * */
        inline int32_t toMicroseconds()
        {
            return static_cast<int32_t>(_elapsed.tv_sec * MICROSECOND +
            							_elapsed.tv_nsec / MILLISECOND);
        }

        /** @brief used to display the elapsed time between two calls of the
         *         .getElapsed() method in milliseconds
         *
         *  @returns int32_t - elapsed time in milliseconds
         * */
        inline int32_t toMilliseconds()
        {
            return static_cast<int32_t>(_elapsed.tv_sec * MILLISECOND +
            							_elapsed.tv_nsec / MICROSECOND);
        }

        /** @brief used to display the elapsed time between two calls of the
         *         .getElapsed() method in seconds
         *
         *  @returns int64_t - elapsed time in seconds
         * */
        inline int32_t toSeconds()
        {
            return static_cast<int32_t>(_elapsed.tv_sec);
        }

    private:
        //used to measure elapsed time since last getElapsed() call
        timespec _elapsed;

        //used to help calculate the elapsed time since last getElapsed() call
        timespec _elapsedLastCall;
};

#endif /* COMMON_TIME_H_ */


