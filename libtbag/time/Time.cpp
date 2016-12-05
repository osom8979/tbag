/**
 * @file   Time.cpp
 * @brief  Time class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/time/Time.hpp>
#include <libtbag/pattern/Singleton.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/* inline */ namespace impl {

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
class SafetyTimeGetter : SINGLETON_INHERITANCE(SafetyTimeGetter)
{
public:
    SINGLETON_RESTRICT(SafetyTimeGetter);

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
};

} // namespace impl

int getMillisec(std::chrono::system_clock::time_point const & time)
{
    std::chrono::system_clock::duration epoch = time.time_since_epoch();
    epoch -= std::chrono::duration_cast<std::chrono::seconds>(epoch);

    // It does not work on some platforms:
    // return static_cast<int>(epoch / std::chrono::milliseconds(1));
    return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
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
    return impl::SafetyTimeGetter::getInstance()->getGmtTime(t, output);
}

bool getLocalTime(time_t const & t, tm * output)
{
    return impl::SafetyTimeGetter::getInstance()->getLocalTime(t, output);
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

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

