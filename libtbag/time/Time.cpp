/**
 * @file   Time.cpp
 * @brief  Time class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/time/Time.hpp>
#include <libtbag/string/Strings.hpp>
#include <libtbag/pattern/Singleton.hpp>

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
    buffer.resize(allocate_size, static_cast<typename std::vector<char>::value_type>(0x00));

    std::size_t length = std::strftime(&buffer[0], allocate_size, format.c_str(), t);
    if (length >= allocate_size) {
        return getFormatString(format, t, allocate_size * 2);
    }
    return std::string(buffer.begin(), buffer.begin() + length);
}

std::wstring getFormatString(std::wstring const & format, tm const * t, std::size_t allocate_size)
{
    // The expected size of the buffer.
    std::vector<wchar_t> buffer;
    buffer.resize(allocate_size, static_cast<typename std::vector<wchar_t>::value_type>(0x00));

    std::size_t length = std::wcsftime(&buffer[0], allocate_size, format.c_str(), t);
    if (length >= allocate_size) {
        return getFormatString(format, t, allocate_size * 2);
    }
    return std::wstring(buffer.begin(), buffer.begin() + length);
}

int getMillisec(std::chrono::system_clock::time_point const & time)
{
    std::chrono::system_clock::duration epoch = time.time_since_epoch();
    epoch -= std::chrono::duration_cast<std::chrono::seconds>(epoch);

    // It does not work on some platforms:
    // return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();

    return static_cast<int>(epoch / std::chrono::milliseconds(1));
}

/* inline */ namespace impl {

template <typename CharType
        , typename StringType = std::basic_string<CharType> >
static inline StringType
getPrecisionStringForMillisec(StringType const & millisec_string)
{
    switch (millisec_string.size()) {
    case 2:  return StringType() + CHAR_OR_WIDECHAR(CharType,   "0") + millisec_string;
    case 1:  return StringType() + CHAR_OR_WIDECHAR(CharType,  "00") + millisec_string;
    case 0:  return StringType() + CHAR_OR_WIDECHAR(CharType, "000") + millisec_string;
    default: return millisec_string;
    }
}

} // inline namespace impl

std::string getMillisecMbs(std::chrono::system_clock::time_point const & time)
{
    return impl::getPrecisionStringForMillisec<char>(std::to_string(getMillisec(time)));
}

std::wstring getMillisecWcs(std::chrono::system_clock::time_point const & time)
{
    return impl::getPrecisionStringForMillisec<wchar_t>(std::to_wstring(getMillisec(time)));
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

