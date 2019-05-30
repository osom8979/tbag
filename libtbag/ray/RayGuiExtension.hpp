/**
 * @file   RayGuiExtension.hpp
 * @brief  RayGuiExtension class prototype.
 * @author zer0
 * @date   2019-05-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUIEXTENSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUIEXTENSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ray/RayGui.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/Err.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

template <typename ... Args>
void GuiTextFormat(std::string const & f, Args && ... args)
{
    return GuiText(libtbag::string::fformat(f, std::forward<Args>(args) ...).c_str());
}

template <typename ... Args>
void GuiTextColoredFormat(Vector4 const & col, char const * f, Args && ... args)
{
    return GuiTextColored(col, libtbag::string::fformat(f, std::forward<Args>(args) ...).c_str());
}

template <typename ... Args>
void GuiTextDisabledFormat(std::string const & f, Args && ... args)
{
    return GuiTextDisabled(libtbag::string::fformat(f, std::forward<Args>(args) ...).c_str());
}

template <typename ... Args>
void GuiTextWrappedFormat(std::string const & f, Args && ... args)
{
    return GuiTextWrapped(libtbag::string::fformat(f, std::forward<Args>(args) ...).c_str());
}

template <typename ... Args>
void GuiLabelTextFormat(std::string const & label, std::string const & f, Args && ... args)
{
    return GuiLabelText(label, libtbag::string::fformat(f, std::forward<Args>(args) ...).c_str());
}

template <typename ... Args>
void GuiBulletTextFormat(std::string const & f, Args && ... args)
{
    return GuiBulletText(libtbag::string::fformat(f, std::forward<Args>(args) ...).c_str());
}

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUIEXTENSION_HPP__

