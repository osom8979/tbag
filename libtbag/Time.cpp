/**
 * @file   Time.cpp
 * @brief  Time class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/Time.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/** millisecond part of the second 0-999. */
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

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

