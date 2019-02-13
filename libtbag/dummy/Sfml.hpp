/**
 * @file   Sfml.hpp
 * @brief  Sfml class prototype.
 * @author zer0
 * @date   2019-02-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_SFML_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_SFML_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstddef>
#include <cstdint>

#include <string>

#ifndef SFML_SYSTEM_API
#define SFML_SYSTEM_API
#endif

#ifndef SFML_WINDOW_API
#define SFML_WINDOW_API
#endif

#ifndef SFML_GRAPHICS_API
#define SFML_GRAPHICS_API
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dummy {
namespace sf    {

// 8 bits integer types
typedef signed   char Int8;
typedef unsigned char Uint8;

// 16 bits integer types
typedef signed   short Int16;
typedef unsigned short Uint16;

// 32 bits integer types
typedef signed   int Int32;
typedef unsigned int Uint32;

// 64 bits integer types
#if defined(_MSC_VER)
typedef signed   __int64 Int64;
typedef unsigned __int64 Uint64;
#else
typedef signed   long long Int64;
typedef unsigned long long Uint64;
#endif

// ----------
// System API
// ----------

struct SFML_SYSTEM_API NonCopyable
{
};

struct SFML_SYSTEM_API String
{
};

// ----------
// Window API
// ----------

struct SFML_WINDOW_API GlResource
{
};

struct SFML_WINDOW_API VideoMode
{
    VideoMode(){}
    VideoMode(unsigned int modeWidth, unsigned int modeHeight, unsigned int modeBitsPerPixel = 32){}
};

struct ContextSettings
{
    enum Attribute
    {
        Default = 0,
        Core    = 1 << 0,
        Debug   = 1 << 2
    };

    unsigned int depthBits;
    unsigned int stencilBits;
    unsigned int antialiasingLevel;
    unsigned int majorVersion;
    unsigned int minorVersion;
    Uint32       attributeFlags;
    bool         sRgbCapable;

    ContextSettings(unsigned int depth = 0,
                    unsigned int stencil = 0,
                    unsigned int antialiasing = 0,
                    unsigned int major = 1,
                    unsigned int minor = 1,
                    unsigned int attributes = Default,
                    bool sRgb = false)
                    : depthBits        (depth),
                      stencilBits      (stencil),
                      antialiasingLevel(antialiasing),
                      majorVersion     (major),
                      minorVersion     (minor),
                      attributeFlags   (attributes),
                      sRgbCapable      (sRgb)
    { }
};

namespace Style {
enum
{
    None       = 0,
    Titlebar   = 1 << 0,
    Resize     = 1 << 1,
    Close      = 1 << 2,
    Fullscreen = 1 << 3,
    Default = Titlebar | Resize | Close
};
} // namespace Style

struct Event
{
};

struct SFML_WINDOW_API Window : GlResource, NonCopyable
{
    void close(){}
    bool isOpen() const { return false; }
    bool pollEvent(Event & event) { return false; }
    void display(){}
};

// ------------
// Graphics API
// ------------

struct SFML_GRAPHICS_API Color
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    Color(){}
    Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255){}
};

struct SFML_GRAPHICS_API Drawable
{
};

struct SFML_GRAPHICS_API Transformable
{
};

struct SFML_WINDOW_API GlResource
{
};

struct SFML_GRAPHICS_API VertexArray : public Drawable
{
};

struct SFML_GRAPHICS_API VertexBuffer : public Drawable, private GlResource
{
};

struct SFML_GRAPHICS_API Sprite : public Drawable, public Transformable
{
};

struct SFML_GRAPHICS_API Text : public Drawable, public Transformable
{
};

struct SFML_GRAPHICS_API Shape : public Drawable, public Transformable
{
};

struct SFML_GRAPHICS_API CircleShape : public Shape
{
};

struct SFML_GRAPHICS_API ConvexShape : public Shape
{
};

struct SFML_GRAPHICS_API RectangleShape : public Shape
{
};

struct SFML_GRAPHICS_API RenderTarget : NonCopyable
{
    void clear(Color const & color = Color(0, 0, 0, 255)){}
};

struct SFML_GRAPHICS_API RenderWindow : public Window, public RenderTarget
{
    RenderWindow(VideoMode mode, String const & title, Uint32 style, ContextSettings const & settings){}
    RenderWindow(VideoMode mode, std::string const & title, Uint32 style, ContextSettings const & settings){}
};

} // namespace sf
} // namespace dummy

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_SFML_HPP__

