/**
 * @file   TraceDyMem.cpp
 * @brief  TraceDyMem class implementation.
 * @author zer0
 * @date   2017-06-08
 */

#include <libtbag/debug/TraceDyMem.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

void * calloc(std::size_t num, std::size_t size)
{
    void * result = ::calloc(num, size);
    if (getTraceLevel() == TBAG_MEMORY_SIMPLE_TRACE) {
    } else if (getTraceLevel() == TBAG_MEMORY_STACK_TRACE) {
    }
    return result;
}

void free(void * ptr)
{
    if (getTraceLevel() == TBAG_MEMORY_SIMPLE_TRACE) {
    } else if (getTraceLevel() == TBAG_MEMORY_STACK_TRACE) {
    }
    ::free(ptr);
}

void * malloc(std::size_t size)
{
    void * result = ::malloc(size);
    if (getTraceLevel() == TBAG_MEMORY_SIMPLE_TRACE) {
    } else if (getTraceLevel() == TBAG_MEMORY_STACK_TRACE) {
    }
    return result;
}

void * realloc(void * ptr, std::size_t size)
{
    void * result = ::realloc(ptr, size);
    if (getTraceLevel() == TBAG_MEMORY_SIMPLE_TRACE) {
    } else if (getTraceLevel() == TBAG_MEMORY_STACK_TRACE) {
    }
    return result;
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

