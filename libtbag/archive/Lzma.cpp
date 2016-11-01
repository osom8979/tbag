/**
 * @file   Lzma.cpp
 * @brief  Lzma class implementation.
 * @author zer0
 * @date   2016-10-30
 */

#include <libtbag/archive/Lzma.hpp>

#include <libtbag/3rd/lzma/Alloc.h>
#include <libtbag/3rd/lzma/7zFile.h>
#include <libtbag/3rd/lzma/7zVersion.h>
#include <libtbag/3rd/lzma/LzmaDec.h>
#include <libtbag/3rd/lzma/LzmaEnc.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

/**
 * Pointer to implementation of LZMA SDK.
 *
 * @author zer0
 * @date   2016-10-30
 */
struct Lzma::LzmaPimpl
{
private:

public:
    LzmaPimpl()
    {
    }

    ~LzmaPimpl()
    {
    }
};

// --------------------
// Lzma implementation.
// --------------------

Lzma::Lzma() : _lzma(new LzmaPimpl)
{
    // EMPTY.
}

Lzma::~Lzma()
{
    // EMPTY.
}

bool Lzma::write(std::string const & path)
{
    return false;
}

bool Lzma::read(std::string const & path)
{
    return false;
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

