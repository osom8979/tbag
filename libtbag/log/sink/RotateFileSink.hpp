/**
 * @file   RotateFileSink.hpp
 * @brief  RotateFileSink class prototype.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Create a cpp file)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <libtbag/filesystem/RotatePath.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * RotateFileSink class prototype.
 *
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Create a cpp file)
 */
class TBAG_API RotateFileSink : public Sink
{
public:
    using RotatePath = libtbag::filesystem::RotatePath;

public:
    struct default_init_t { /* EMPTY. */ };

public:
    TBAG_CONSTEXPR static default_init_t const default_init = {};

public:
    TBAG_CONSTEXPR static default_init_t const init() TBAG_NOEXCEPT
    { return default_init; }

private:
    RotatePath _rotate;

public:
    static std::string getDefaultArguments(std::string const & path);

public:
    RotateFileSink();
    RotateFileSink(default_init_t, std::string const & path);
    RotateFileSink(std::string const & arguments);
    virtual ~RotateFileSink();

public:
    virtual bool write(char const * message, int size) override;
    virtual void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

