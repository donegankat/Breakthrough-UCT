#ifndef TIMER_H
#define TIMER_H

// Determine how timing should occur
#if defined(__APPLE__)
    // Fall back to POSIX style gettimeofday timing on OSX
    #define POSIX_TIMING
#elif (defined(__cplusplus) && __cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1700L)
    // Use C++11 std::chrono for timing
    // _MSC_VER >= 1700L implies VS2012 or newer
    // __cplusplus >= 201203L is defined by the C++11 standard
    // it should be correctly defined for clang (3.1+) and gcc (4.7+)
    #define CPP11_TIMING
#else
    // Not sure what timer system to use
    // If this #error were commented out, everything should still work, but
    // the timer will do nothing
    #error "No Timer implemented for current compiler settings"
#endif

#include <ostream>
using std::ostream;

#if defined(CPP11_TIMING)
#include <chrono>
typedef std::chrono::system_clock Clock;
#elif defined(POSIX_TIMING)
#include <sys/time.h>
#endif


class Timer
{
 public:
    friend ostream& operator<<(ostream& os, Timer& t);

    Timer();
    ~Timer();

    void start();
    void stop();
	double TimeElapsed(Timer& t);

 private:
#if defined(CPP11_TIMING)
    Clock::time_point start_time;
    Clock::time_point stop_time;
#elif defined(POSIX_TIMING)
    struct timeval start_time;
    struct timeval stop_time;
	struct timeval current_time;
#endif
};

#endif
