#include "Timer.h"

#include <iostream>
using std::ostream;
#include <iomanip>
using std::setw;


#if defined(CPP11_TIMING)
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
#endif


ostream& operator<<(ostream& os, Timer& t)
{
#if defined(CPP11_TIMING)
    Clock::duration elapsed = t.stop_time - t.start_time;
    hours hh = duration_cast<hours>(elapsed);
    minutes mm = duration_cast<minutes>(elapsed - hh);
    seconds ss = duration_cast<seconds>(elapsed - hh - mm);
    milliseconds ms = duration_cast<milliseconds>(elapsed - hh - mm - ss);

    os << setw(2) << hh.count() << "h "
       << setw(2) << mm.count() << "m "
       << setw(2) << ss.count() << "s "
       << setw(3) << ms.count() << "ms";
#elif defined(POSIX_TIMING)
    time_t seconds = t.stop_time.tv_sec - t.start_time.tv_sec;
    suseconds_t useconds = t.stop_time.tv_usec - t.start_time.tv_usec;
	
    // The subtraction could leave negative time which makes no sense, correct it
    if (useconds < 0)
    {
        --seconds;
        useconds += 1e6;
    }

    time_t hh = seconds / (60*60);
    time_t mm = (seconds / 60) % 60;
    time_t ss = seconds % 60;
    suseconds_t ms = useconds / 1000;

    os << setw(2) << hh << "h "
       << setw(2) << mm << "m "
       << setw(2) << ss << "s "
       << setw(3) << ms << "ms";
#endif
    return os;
}

Timer::Timer()
{}
Timer::~Timer()
{}

void Timer::start()
{
#if defined(CPP11_TIMING)
    start_time = Clock::now();
#elif defined(POSIX_TIMING)
    gettimeofday(&start_time, NULL);
#endif
}

void Timer::stop()
{
#if defined(CPP11_TIMING)
    stop_time = Clock::now();
#elif defined(POSIX_TIMING)
    gettimeofday(&stop_time, NULL);
#endif
}

double Timer::TimeElapsed(Timer& t)
{
	gettimeofday(&current_time, NULL);
	time_t durationseconds = t.current_time.tv_sec - t.start_time.tv_sec;
    suseconds_t durationsuseconds = t.current_time.tv_sec - t.start_time.tv_usec;
	
    // The subtraction could leave negative time which makes no sense, correct it
    if (durationsuseconds < 0)
    {
        --durationseconds;
        durationsuseconds += 1e6;
    }

	return durationseconds;
}
