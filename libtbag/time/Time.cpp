/**
 * @file   Time.cpp
 * @brief  Time class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/time/Time.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/pattern/Singleton2.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <libtbag/3rd/date/date.h>

#include <cassert>
#include <cstring>
#include <cmath>

#include <ratio>
#include <mutex>
#include <atomic>
#include <type_traits>

// Where is 'timeval' structure?
// [WARNING] Don't change include order.
#if defined(TBAG_PLATFORM_WINDOWS)
# include <Windows.h>
#else
# include <sys/time.h>
# include <libtbag/dummy/Win32.hpp>
using namespace ::libtbag::dummy::win32;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/* inline */ namespace __impl {

/**
 * SafetyTimeGetter class prototype & implementation.
 *
 * @translate{ko, 이 클래스는 표준C 라이브러리의 데이터 경쟁(Data race)상태를 해결하기 위해 작성되었다.}
 *
 * @author zer0
 * @date   2016-09-22
 *
 * @remarks
 *  This class is the solution of the data race conditions.
 */
class SafetyTimeGetter : public ::libtbag::pattern::Singleton2<SafetyTimeGetter>
{
public:
    SINGLETON2_PROTOTYPE(SafetyTimeGetter);

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    mutable Mutex _mutex;

private:
    template <typename Predicated>
    inline bool tryCatch(Predicated predicated) const
    {
        try {
            Guard g(_mutex);
            predicated();
        } catch (...) {
            return false;
        }
        return true;
    }

public:
    inline bool getGmtTime(time_t const & t, tm * output) const
    {
        return tryCatch([&](){
            if (output != nullptr) {
                memcpy(output, gmtime(&t), sizeof(tm)); // Position of data race!
            }
        });
    }

    inline bool getLocalTime(time_t const & t, tm * output) const
    {
        return tryCatch([&](){
            if (output != nullptr) {
                memcpy(output, localtime(&t), sizeof(tm)); // Position of data race!
            }
        });
    }

public:
};

/**
 * @author zer0
 * @date   2017-04-11
 */
class LocalTimeDiff : public ::libtbag::pattern::Singleton2<LocalTimeDiff>
{
public:
    friend class ::libtbag::pattern::Singleton2<LocalTimeDiff>;

private:
    std::chrono::system_clock::duration _local_diff;
    std::atomic_bool _update_local_diff;

protected:
    LocalTimeDiff() : _local_diff(0)
    {
        _update_local_diff = false;
    }

public:
    ~LocalTimeDiff()
    {
        // EMPTY.
    }

private:
    bool updateLocalDiff()
    {
        time_t time = getCurrentTime();

        struct tm   gmt_time = {0,};
        struct tm local_time = {0,};

        bool is_gmt   = __impl::SafetyTimeGetter::getInstance()->getGmtTime(time, &gmt_time);
        bool is_local = __impl::SafetyTimeGetter::getInstance()->getLocalTime(time, &local_time);

        if (is_gmt == false || is_local == false) {
            return false;
        }

        auto   gmt_time_point = std::chrono::system_clock::from_time_t(mktime(  &gmt_time));
        auto local_time_point = std::chrono::system_clock::from_time_t(mktime(&local_time));

        _local_diff = (local_time_point - gmt_time_point);
        _update_local_diff = true;

        return true;
    }

public:
    std::chrono::system_clock::duration getLocalDiff()
    {
        if (_update_local_diff == false) {
            updateLocalDiff();
        }
        return _local_diff;
    }
};

/**
 * @warning
 *  Epoch time은 UTC 이전(1970/01/01T00:00:00)일 경우 음수로 표현된다. @n
 *  이 경우 정상적인 계산이 이루어지지 않는다. @n
 *  이 현상을 해결하기 위해 Date(날짜)의 기준을 1970년 이후로 맞춰주면 된다.
 */
template <typename Cut, typename Destination, typename Clock, typename ClockDuration>
typename Destination::rep getTimeFloor(std::chrono::time_point<Clock, ClockDuration> const & time)
{
    static_assert(std::ratio_less_equal<typename Cut::period, std::chrono::hours::period>::value,
                  "Unsupported Cut type.");
    static_assert(std::ratio_less<typename Destination::period, typename Cut::period>::value,
                  "Destination must be less than Cut.");

    using TimePoint = std::chrono::time_point<Clock, ClockDuration>;

    ClockDuration epoch_time = time.time_since_epoch();
    if (epoch_time.count() < 0) {
        auto DAYS_OF_EPOCH = date::floor<date::days>(date::years(1970));
        auto days = date::floor<date::days>(time);

        epoch_time += (DAYS_OF_EPOCH - days.time_since_epoch());
        assert(epoch_time.count() >= 0);
    }

    epoch_time -= std::chrono::duration_cast<Cut>(epoch_time);
    assert(epoch_time.count() >= 0);

    // [WARNING] It does not work on some platforms:
    // return static_cast<int>(result / std::chrono::milliseconds(1));

    return std::abs(std::chrono::duration_cast<Destination>(epoch_time).count());
}

void createInstance()
{
    LocalTimeDiff::createInstance();
    SafetyTimeGetter::createInstance();
}

void releaseInstance()
{
    SafetyTimeGetter::releaseInstance();
    LocalTimeDiff::releaseInstance();
}

} // namespace __impl

int getYear(std::chrono::system_clock::time_point const & time)
{
    return static_cast<int>(date::year_month_day(date::floor<date::days>(time)).year());
}

int getMonth(std::chrono::system_clock::time_point const & time)
{
    return static_cast<unsigned>(date::year_month_day(date::floor<date::days>(time)).month());
}

int getDay(std::chrono::system_clock::time_point const & time)
{
    return static_cast<unsigned>(date::year_month_day(date::floor<date::days>(time)).day());
}

int getHours(std::chrono::system_clock::time_point const & time)
{
    return static_cast<int>(date::make_time(time - date::floor<date::days>(time)).hours().count());
}

int getMinutes(std::chrono::system_clock::time_point const & time)
{
    return static_cast<int>(date::make_time(time - date::floor<date::days>(time)).minutes().count());
}

int getSeconds(std::chrono::system_clock::time_point const & time)
{
    return static_cast<int>(date::make_time(time - date::floor<date::days>(time)).seconds().count());
}

int getSubSeconds(std::chrono::system_clock::time_point const & time)
{
    return static_cast<int>(date::make_time(time - date::floor<date::days>(time)).subseconds().count());
}

int getWeek(std::chrono::system_clock::time_point const & time)
{
    return static_cast<unsigned>(date::weekday(date::floor<date::days>(time)));
}

std::string getWeekString(std::chrono::system_clock::time_point const & time)
{
    switch (getWeek(time)) {
    case 0: return "Sun";
    case 1: return "Mon";
    case 2: return "Tue";
    case 3: return "Wed";
    case 4: return "Thu";
    case 5: return "Fri";
    case 6: return "Sat";
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return "[Unknown]";
}

int getMillisec(std::chrono::system_clock::time_point const & time)
{
    using namespace std::chrono;
    auto const RESULT = __impl::getTimeFloor<seconds, milliseconds>(time);
    assert(0 <= COMPARE_AND(RESULT) < 1000);
    return static_cast<int>(RESULT);
}

int getMicrosec(std::chrono::system_clock::time_point const & time)
{
    using namespace std::chrono;
    auto const RESULT = __impl::getTimeFloor<milliseconds, microseconds>(time);
    assert(0 <= COMPARE_AND(RESULT) < 1000);
    return static_cast<int>(RESULT);
}

int getNanosec(std::chrono::system_clock::time_point const & time)
{
    using namespace std::chrono;
    auto const RESULT = __impl::getTimeFloor<microseconds, nanoseconds>(time);
    assert(0 <= COMPARE_AND(RESULT) < 1000);
    return static_cast<int>(RESULT);
}

int getDays(std::chrono::system_clock::time_point const & time)
{
    return date::floor<date::days>(time).time_since_epoch().count();
}

std::string getMillisecMbs(std::chrono::system_clock::time_point const & time)
{
    std::string millisec = std::to_string(getMillisec(time));
    switch (millisec.size()) {
    case 2:  return std::string(  "0") + millisec;
    case 1:  return std::string( "00") + millisec;
    case 0:  return std::string("000") + millisec;
    default: return millisec;
    }
}

void getMillisecString(std::chrono::system_clock::time_point const & time, std::string & result)
{
    result = getMillisecMbs(time);
}

std::chrono::system_clock::time_point getNowSystemClock() TBAG_NOEXCEPT
{
    return std::chrono::system_clock::now();
}

time_t getTime(std::chrono::system_clock::time_point const & time_point) TBAG_NOEXCEPT
{
    return std::chrono::system_clock::to_time_t(time_point);
}

time_t getCurrentTime() TBAG_NOEXCEPT
{
    return getTime(getNowSystemClock());
}

bool getGmtTime(time_t const & t, tm * output)
{
    return __impl::SafetyTimeGetter::getInstance()->getGmtTime(t, output);
}

bool getLocalTime(time_t const & t, tm * output)
{
    return __impl::SafetyTimeGetter::getInstance()->getLocalTime(t, output);
}

std::chrono::system_clock::duration getCurrentLocalDuration()
{
    return __impl::LocalTimeDiff::getInstance()->getLocalDiff();
}

std::string getFormatString(std::string const & format, tm const * t, std::size_t allocate_size)
{
    // The expected size of the buffer.
    std::vector<char> buffer;
    buffer.resize(allocate_size, static_cast<char>(0x00));

    std::size_t length = std::strftime(&buffer[0], allocate_size, format.c_str(), t);
    if (length >= allocate_size) {
        return getFormatString(format, t, allocate_size * 2);
    }
    return std::string(buffer.begin(), buffer.begin() + length);
}

std::string getLocalTimeZoneAbbreviation()
{
#if defined(TBAG_PLATFORM_UNIX_LIKE)
    struct tm local_time = {0,};
    if (getLocalTime(getCurrentTime(), &local_time)) {
        // MSVC error C2039: 'tm_zone': is not a member of 'tm'
        if (local_time.tm_zone != nullptr) {
            return std::string(local_time.tm_zone);
        }
    }
#endif
    return std::string();
}

#ifndef UINT64CONST
#define UINT64CONST(x) ((uint64_t)(x##ULL))
#endif

// FILETIME of Jan 1 1970 00:00:00.
TBAG_CONSTEXPR const ULONGLONG WIN32_FILETIME_EPOCH = UINT64CONST(116444736000000000);

static int __win32_gettimeofday(long & sec, long & micro)
{
    FILETIME        file_time;
    SYSTEMTIME      system_time;
    ULARGE_INTEGER  ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);

    ularge.LowPart  = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;

    sec = (long) ((ularge.QuadPart - WIN32_FILETIME_EPOCH) / 10000000L);
    micro = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

static int _gettimeofday(long & sec, long & micro)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return __win32_gettimeofday(src, micro);
#else
    timeval tp = {0,};
    // timezone information is stored outside the kernel so tzp isn't used anymore.
    int const CODE = gettimeofday(&tp, nullptr);
    sec = tp.tv_sec;
    micro = tp.tv_usec;
    return CODE;
#endif
}

Err getTimeOfDay(long * sec, long * micro)
{
    long s, u;
    if (_gettimeofday(s, u) != 0) {
        return libtbag::getGlobalSystemError();
    }
    if (sec != nullptr) {
        *sec = s;
    }
    if (micro != nullptr) {
        *micro = u;
    }
    return Err::E_SUCCESS;
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

