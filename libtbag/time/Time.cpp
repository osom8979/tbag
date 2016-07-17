/**
 * @file   Time.cpp
 * @brief  Time class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/time/Time.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

std::chrono::system_clock::time_point getNowSystemClock() noexcept
{
    return std::chrono::system_clock::now();
}

time_t getTime(std::chrono::system_clock::time_point const & time_point) noexcept
{
    return std::chrono::system_clock::to_time_t(time_point);
}

time_t getCurrentTime() noexcept
{
    return getTime(getNowSystemClock());
}

tm * getGmtTime(time_t const & t) noexcept
{
    return gmtime(&t);
}

tm * getLocalTime(time_t const & t) noexcept
{
    return localtime(&t);
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

std::string getMillisecMbs(std::chrono::system_clock::time_point const & time)
{
    using BaseString = std::string;
    BaseString result = std::to_string(getMillisec(time));

    if (result.size() == 2) {
        return BaseString("0") + result;
    } else if (result.size() == 1) {
        return BaseString("00") + result;
    } else if (result.size() == 0) {
        return BaseString("000");
    } else {
        return result;
    }
}

std::wstring getMillisecWcs(std::chrono::system_clock::time_point const & time)
{
    using BaseString = std::wstring;
    BaseString result = std::to_wstring(getMillisec(time));

    if (result.size() == 2) {
        return BaseString(L"0") + result;
    } else if (result.size() == 1) {
        return BaseString(L"00") + result;
    } else if (result.size() == 0) {
        return BaseString(L"000");
    } else {
        return result;
    }
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

