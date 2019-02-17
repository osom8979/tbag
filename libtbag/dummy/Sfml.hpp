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

template <typename T>
struct Vector2
{
    T x, y;

    Vector2() {}
    Vector2(T x_, T y_) {}
};

typedef Vector2<int>          Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float>        Vector2f;

template <typename T>
struct Rect
{
    T left, top, width, height;

    Rect() {}
    Rect(T l, T t, T w, T h) {}
    ~Rect() {}
};

typedef Rect<int>   IntRect;
typedef Rect<float> FloatRect;

// ----------
// System API
// ----------

struct SFML_SYSTEM_API NonCopyable
{
    NonCopyable() {}
    ~NonCopyable() {}
};

struct SFML_SYSTEM_API String
{
    String() {}
    String(std::string const & text) {}
    ~String() {}
};

// ----------
// Window API
// ----------

struct SFML_WINDOW_API GlResource
{
    GlResource() {}
    ~GlResource() {}
};

struct SFML_WINDOW_API VideoMode
{
    VideoMode() {}
    VideoMode(unsigned int modeWidth, unsigned int modeHeight, unsigned int modeBitsPerPixel = 32) {}
    ~VideoMode() {}
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
                    bool rgb = false) {}
    ~ContextSettings() {}
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

struct SFML_WINDOW_API Cursor : NonCopyable
{
    Cursor() {}
    ~Cursor() {}

    enum Type {
        Arrow, ArrowWait, Wait, Text, Hand, SizeHorizontal, SizeVertical,
        SizeTopLeftBottomRight, SizeBottomLeftTopRight, SizeAll, Cross, Help, NotAllowed
    };

    bool loadFromPixels(Uint8 const * pixels, Vector2u size, Vector2u hotspot) { return false; }
    bool loadFromSystem(Type type) { return false; }
};

struct Event
{
    Event() {}
    ~Event() {}
};

struct SFML_WINDOW_API Window : GlResource, NonCopyable
{
    ContextSettings __context_settings;

    Window() {}
    ~Window() {}

    void close() {}
    bool isOpen() const { return false; }
    ContextSettings const & getSettings() const { return __context_settings; }
    bool pollEvent(Event & event) { return false; }
    bool waitEvent(Event & event) { return false; }
    Vector2i getPosition() const { return Vector2i(); }
    void setPosition(Vector2i const & position) {}
    void setSize(Vector2u const & size) {}
    void setTitle(String const & title) {}
    void setIcon(unsigned int width, unsigned int height, Uint8 const * pixels) {}
    void setVisible(bool visible) {}
    void setVerticalSyncEnabled(bool enabled) {}
    void setMouseCursorVisible(bool visible) {}
    void setMouseCursorGrabbed(bool grabbed) {}
    void setMouseCursor(Cursor const & cursor) {}
    void setKeyRepeatEnabled(bool enabled) {}
    void setFramerateLimit(unsigned int limit) {}
    void setJoystickThreshold(float threshold) {}
    void requestFocus() {}
    bool hasFocus() const { return false; }
    void display() {}
};

// ------------
// Graphics API
// ------------

struct RenderTarget;

struct SFML_GRAPHICS_API Color
{
    Uint8 r, g, b, a;

    Color() {}
    Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255) {}
    ~Color() {}
};

struct SFML_GRAPHICS_API Transform
{
    Transform() {}
    Transform(float a00, float a01, float a02,
              float a10, float a11, float a12,
              float a20, float a21, float a22) {}
    ~Transform() {}

    float const * getMatrix() const { return nullptr; }
    Transform getInverse() const { return Transform(); }
    Vector2f transformPoint(float x, float y) const { return Vector2f(); }
    Vector2f transformPoint(Vector2f const & point) const { return Vector2f(); }
    FloatRect transformRect(FloatRect const & rectangle) const { return FloatRect(); }
    Transform & combine(Transform const & transform) { return *this; }
    Transform & translate(float x, float y) { return *this; }
    Transform & translate(Vector2f const & offset) { return *this; }
    Transform & rotate(float angle) { return *this; }
    Transform & rotate(float angle, float centerX, float centerY) { return *this; }
    Transform & rotate(float angle, Vector2f const & center) { return *this; }
    Transform & scale(float scaleX, float scaleY) { return *this; }
    Transform & scale(float scaleX, float scaleY, float centerX, float centerY) { return *this; }
    Transform & scale(Vector2f const & factors) { return *this; }
    Transform & scale(Vector2f const & factors, Vector2f const & center) { return *this; }
};

struct SFML_GRAPHICS_API View
{
    Vector2f  __center;
    Vector2f  __size;
    FloatRect __viewport;
    Transform __transform;

    View() {}
    View(FloatRect const & rectangle) {}
    View(Vector2f const & center, Vector2f const & size) {}
    ~View() {}

    void setCenter(float x, float y) {}
    void setCenter(Vector2f const & center) {}
    void setSize(float width, float height) {}
    void setSize(Vector2f const & size) {}
    void setRotation(float angle) {}
    void setViewport(FloatRect const & viewport) {}
    void reset(FloatRect const & rectangle) {}
    Vector2f const & getCenter() const { return __center; }
    Vector2f const & getSize() const { return __size; }
    float getRotation() const { return 0.f; }
    FloatRect const & getViewport() const { return __viewport; }
    void move(float offsetX, float offsetY) {}
    void move(Vector2f const & offset) {}
    void rotate(float angle) {}
    void zoom(float factor) {}
    Transform const & getTransform() const { return __transform; }
    Transform const & getInverseTransform() const { return __transform; }
};

struct SFML_GRAPHICS_API RenderStates
{
    RenderStates() {}
    ~RenderStates() {}
};

struct SFML_GRAPHICS_API Drawable
{
    Drawable() {}
    ~Drawable() {}

    virtual void draw(RenderTarget & target, RenderStates states) const {};
};

struct SFML_GRAPHICS_API Transformable
{
    Transformable() {}
    ~Transformable() {}
};

struct SFML_GRAPHICS_API VertexArray : public Drawable
{
    VertexArray() {}
    ~VertexArray() {}
};

struct SFML_GRAPHICS_API VertexBuffer : public Drawable, private GlResource
{
    VertexBuffer() {}
    ~VertexBuffer() {}
};

struct SFML_GRAPHICS_API Sprite : public Drawable, public Transformable
{
    Sprite() {}
    ~Sprite() {}
};

struct SFML_GRAPHICS_API Text : public Drawable, public Transformable
{
    Text() {}
    ~Text() {}
};

struct SFML_GRAPHICS_API Shape : public Drawable, public Transformable
{
    Shape() {}
    ~Shape() {}
};

struct SFML_GRAPHICS_API CircleShape : public Shape
{
    CircleShape() {}
    ~CircleShape() {}
};

struct SFML_GRAPHICS_API ConvexShape : public Shape
{
    ConvexShape() {}
    ~ConvexShape() {}
};

struct SFML_GRAPHICS_API RectangleShape : public Shape
{
    RectangleShape() {}
    ~RectangleShape() {}
};

struct SFML_GRAPHICS_API RenderTarget : NonCopyable
{
    View __view;

    RenderTarget() {}
    ~RenderTarget() {}

    void clear(Color const & color = Color(0, 0, 0, 255)) {}

    void setView(View const & view) {}
    View const & getView() const { return __view; }
    View const & getDefaultView() const { return __view; }
    IntRect getViewport(View const & view) const { return IntRect(); }

    Vector2f mapPixelToCoords(Vector2i const & point) const { return Vector2f(); }
    Vector2f mapPixelToCoords(Vector2i const & point, View const & view) const { return Vector2f(); }

    Vector2i mapCoordsToPixel(Vector2f const & point) const { return Vector2i(); }
    Vector2i mapCoordsToPixel(Vector2f const & point, View const & view) const { return Vector2i(); }

    //void draw(const Drawable& drawable, const RenderStates& states = RenderStates::Default);
    //void draw(const Vertex* vertices, std::size_t vertexCount, PrimitiveType type, const RenderStates& states = RenderStates::Default);
    //void draw(const VertexBuffer& vertexBuffer, const RenderStates& states = RenderStates::Default);
    //void draw(const VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const RenderStates& states = RenderStates::Default);

    Vector2u getSize() const { return Vector2u(); };
    bool setActive(bool active = true) { return false; }

    void pushGLStates() {}
    void popGLStates() {}
    void resetGLStates() {}
};

struct SFML_GRAPHICS_API RenderWindow : public Window, public RenderTarget
{
    RenderWindow() {}
    RenderWindow(VideoMode mode, String const & title, Uint32 style, ContextSettings const & settings) {}
    RenderWindow(VideoMode mode, std::string const & title, Uint32 style, ContextSettings const & settings) {}
    ~RenderWindow() {}
};

} // namespace sf
} // namespace dummy

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_SFML_HPP__

