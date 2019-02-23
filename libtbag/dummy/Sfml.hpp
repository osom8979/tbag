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
#include <map>

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
    ~Vector2() {}
};

typedef Vector2<int>          Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float>        Vector2f;

template <typename T>
struct Vector3
{
    T x, y, z;

    Vector3() {}
    Vector3(T x_, T y_, T z_) {}
    ~Vector3() {}
};

typedef Vector3<int>   Vector3i;
typedef Vector3<float> Vector3f;

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

struct SFML_SYSTEM_API InputStream
{
    InputStream() {}
    ~InputStream() {}

    Int64 read(void * data, Int64 size) { return 0; }
    Int64 seek(Int64 position) { return 0; }
    Int64 tell() { return 0; }
    Int64 getSize() { return 0; }
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
enum {
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

namespace priv {

template <std::size_t Columns, std::size_t Rows>
struct Matrix
{
    float array[Columns*Rows];

    Matrix() {}
    Matrix(float const * pointer) {}
    Matrix(Transform const & transform) {}
    ~Matrix() {}
};

template <typename T>
struct Vector4
{
    T x, y, z, w;

    Vector4() {}
    Vector4(T x_, T y_, T z_, T w_) {}
    Vector4(Color const & color) {}
    ~Vector4() {}
};

} // namespace priv

namespace Glsl {
typedef Vector2<float> Vec2;
typedef Vector2<int>   Ivec2;
typedef Vector2<bool>  Bvec2;
typedef Vector3<float> Vec3;
typedef Vector3<int>   Ivec3;
typedef Vector3<bool>  Bvec3;
typedef priv::Vector4<float> Vec4;
typedef priv::Vector4<int>   Ivec4;
typedef priv::Vector4<bool>  Bvec4;
typedef priv::Matrix<3, 3> Mat3;
typedef priv::Matrix<4, 4> Mat4;
} // namespace Glsl

enum PrimitiveType
{
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
    Quads,
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

struct SFML_GRAPHICS_API Image
{
    Image() {}
    ~Image() {}

    void create(unsigned int width, unsigned int height, Color const & color = Color(0, 0, 0)) {}
    void create(unsigned int width, unsigned int height, Uint8 const * pixels) {}

    bool loadFromFile(std::string const & filename) { return false; }
    bool loadFromMemory(void const * data, std::size_t size) { return false; }
    bool loadFromStream(InputStream const & stream) { return false; }
    bool saveToFile(std::string const & filename) const { return false; }

    Vector2u getSize() const { return Vector2u(); }
    void createMaskFromColor(Color const & color, Uint8 alpha = 0) {}
    void copy(Image const & source, unsigned int destX, unsigned int destY, IntRect const & sourceRect = IntRect(0, 0, 0, 0), bool applyAlpha = false) {}
    void setPixel(unsigned int x, unsigned int y, Color const & color) {}
    Color getPixel(unsigned int x, unsigned int y) const { return Color(); }
    Uint8 const * getPixelsPtr() const { return nullptr; }

    void flipHorizontally() {}
    void flipVertically() {}
};

struct SFML_GRAPHICS_API Texture : GlResource
{
    enum CoordinateType {
        Normalized, Pixels
    };

    Texture() {}
    Texture(Texture const & copy) {}
    ~Texture() {}

    bool create(unsigned int width, unsigned int height) { return false; }
    bool loadFromFile(std::string const & filename, IntRect const & area = IntRect()) { return false; }
    bool loadFromMemory(void const * data, std::size_t size, IntRect const & area = IntRect()) { return false; }
    bool loadFromStream(InputStream & stream, IntRect const & area = IntRect()) { return false; }
    bool loadFromImage(Image const & image, IntRect const & area = IntRect()) { return false; }

    Vector2u getSize() const { return Vector2u(); }
    Image copyToImage() const { return Image(); }

    void update(Uint8 const * pixels) {}
    void update(Uint8 const * pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y) {}
    void update(Texture const & texture) {}
    void update(Texture const & texture, unsigned int x, unsigned int y) {}
    void update(Image const & image) {}
    void update(Image const & image, unsigned int x, unsigned int y) {}
    void update(Window const & window) {}
    void update(Window const & window, unsigned int x, unsigned int y) {}

    void setSmooth(bool smooth) {}
    bool isSmooth() const { return false; }
    void setSrgb(bool sRgb) {}
    bool isSrgb() const { return false; }
    void setRepeated(bool repeated) {}
    bool isRepeated() const { return false; }
    bool generateMipmap() { return false; }
    Texture & operator =(Texture const & right) { return *this; }
    void swap(Texture & right) {}
    unsigned int getNativeHandle() const { return 0; }

    static void bind(Texture const * texture, CoordinateType coordinateType = Normalized) {}
    static unsigned int getMaximumSize() { return 0; }
};

struct SFML_GRAPHICS_API Shader : GlResource, NonCopyable
{
    enum Type {
        Vertex, Geometry, Fragment
    };

    struct CurrentTextureType {};

    TBAG_CONSTEXPR static CurrentTextureType CurrentTexture = CurrentTextureType{};

    Shader() {}
    ~Shader() {}

    bool loadFromFile(std::string const & filename, Type type) { return false; }
    bool loadFromFile(std::string const & vertexShaderFilename, std::string const & fragmentShaderFilename) { return false; }
    bool loadFromFile(std::string const & vertexShaderFilename, std::string const & geometryShaderFilename, std::string const & fragmentShaderFilename) { return false; }

    bool loadFromMemory(std::string const & shader, Type type) { return false; }
    bool loadFromMemory(std::string const & vertexShader, std::string const & fragmentShader) { return false; }
    bool loadFromMemory(std::string const & vertexShader, std::string const & geometryShader, std::string const & fragmentShader) { return false; }

    bool loadFromStream(InputStream & stream, Type type) { return false; }
    bool loadFromStream(InputStream & vertexShaderStream, InputStream & fragmentShaderStream) { return false; }
    bool loadFromStream(InputStream & vertexShaderStream, InputStream & geometryShaderStream, InputStream & fragmentShaderStream) { return false; }

    void setUniform(std::string const & name, float x) {}
    void setUniform(std::string const & name, Glsl::Vec2 const & vector) {}
    void setUniform(std::string const & name, Glsl::Vec3 const & vector) {}
    void setUniform(std::string const & name, Glsl::Vec4 const & vector) {}
    void setUniform(std::string const & name, int x) {}
    void setUniform(std::string const & name, Glsl::Ivec2 const & vector) {}
    void setUniform(std::string const & name, Glsl::Ivec3 const & vector) {}
    void setUniform(std::string const & name, Glsl::Ivec4 const & vector) {}
    void setUniform(std::string const & name, bool x) {}
    void setUniform(std::string const & name, Glsl::Bvec2 const & vector) {}
    void setUniform(std::string const & name, Glsl::Bvec3 const & vector) {}
    void setUniform(std::string const & name, Glsl::Bvec4 const & vector) {}
    void setUniform(std::string const & name, Glsl::Mat3 const & matrix) {}
    void setUniform(std::string const & name, Glsl::Mat4 const & matrix) {}
    void setUniform(std::string const & name, Texture const & texture) {}
    void setUniform(std::string const & name, CurrentTextureType) {}

    void setUniformArray(std::string const & name, float const * scalarArray, std::size_t length) {}
    void setUniformArray(std::string const & name, Glsl::Vec2 const * vectorArray, std::size_t length) {}
    void setUniformArray(std::string const & name, Glsl::Vec3 const * vectorArray, std::size_t length) {}
    void setUniformArray(std::string const & name, Glsl::Vec4 const * vectorArray, std::size_t length) {}
    void setUniformArray(std::string const & name, Glsl::Mat3 const * matrixArray, std::size_t length) {}
    void setUniformArray(std::string const & name, Glsl::Mat4 const * matrixArray, std::size_t length) {}

    unsigned int getNativeHandle() const { return 0U; }

    static void bind(Shader const * shader) {}
    static bool isAvailable() { return false; }
    static bool isGeometryAvailable() { return false; }
};

struct SFML_GRAPHICS_API BlendMode
{
    enum Factor {
        Zero, One,
        SrcColor, OneMinusSrcColor,
        DstColor, OneMinusDstColor,
        SrcAlpha, OneMinusSrcAlpha,
        DstAlpha, OneMinusDstAlpha
    };

    enum Equation {
        Add, Subtract, ReverseSubtract
    };

    Factor   colorSrcFactor;
    Factor   colorDstFactor;
    Equation colorEquation;
    Factor   alphaSrcFactor;
    Factor   alphaDstFactor;
    Equation alphaEquation;

    BlendMode() {}
    BlendMode(Factor sourceFactor, Factor destinationFactor, Equation blendEquation = Add) {}
    BlendMode(Factor colorSourceFactor, Factor colorDestinationFactor,
              Equation colorBlendEquation, Factor alphaSourceFactor,
              Factor alphaDestinationFactor, Equation alphaBlendEquation) {}
    ~BlendMode() {}
};

struct SFML_GRAPHICS_API RenderStates
{
    BlendMode blendMode;
    Transform transform;
    Texture const * texture;
    Shader const * shader;

    RenderStates() {}
    RenderStates(BlendMode const & theBlendMode) {}
    RenderStates(Transform const & theTransform) {}
    RenderStates(Texture const * theTexture) {}
    RenderStates(Shader const * theShader) {}
    RenderStates(BlendMode const & theBlendMode, Transform const & theTransform,
                 Texture const * theTexture, Shader const * theShader) {}
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

struct SFML_GRAPHICS_API Vertex
{
    Vector2f position;
    Color    color;
    Vector2f texCoords;

    Vertex() {}
    Vertex(Vector2f const & thePosition) {}
    Vertex(Vector2f const & thePosition, Color const & theColor) {}
    Vertex(Vector2f const & thePosition, Vector2f const & theTexCoords) {}
    Vertex(Vector2f const & thePosition, Color const & theColor, Vector2f const & theTexCoords) {}
    ~Vertex() {}
};

struct SFML_GRAPHICS_API VertexArray : public Drawable
{
    Vertex __vertex;

    VertexArray() {}
    VertexArray(PrimitiveType type, std::size_t vertexCount = 0) {}
    ~VertexArray() {}

    Vertex & operator [](std::size_t index) { return __vertex; }
    Vertex const & operator [](std::size_t index) const { return __vertex; }

    void clear() {}
    void resize(std::size_t vertexCount) {}
    void append(Vertex const & vertex) {}
    void setPrimitiveType(PrimitiveType type) {}

    std::size_t getVertexCount() const { return 0; }
    PrimitiveType getPrimitiveType() const { return Points; }
    FloatRect getBounds() const { return FloatRect(); }
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
    IntRect __rect;
    Color __color;

    Shape() {}
    ~Shape() {}

    void setTexture(Texture const * texture, bool resetRect = false) {}
    void setTextureRect(IntRect const & rect) {}
    void setFillColor(Color const & color) {}
    void setOutlineColor(Color const & color) {}
    void setOutlineThickness(float thickness) {}
    Texture const * getTexture() const { return nullptr; }
    IntRect const & getTextureRect() const { return __rect; }
    Color const & getFillColor() const { return __color; }
    Color const & getOutlineColor() const { return __color; }
    float getOutlineThickness() const { return 0.f; }

    virtual std::size_t getPointCount() const { return 0; }
    virtual Vector2f getPoint(std::size_t index) const { return Vector2f(); }

    FloatRect getLocalBounds() const { return FloatRect(); }
    FloatRect getGlobalBounds() const { return FloatRect(); }
};

struct SFML_GRAPHICS_API CircleShape : public Shape
{
    CircleShape(float radius = 0, std::size_t pointCount = 30) {}
    ~CircleShape() {}

    void setRadius(float radius) {}
    float getRadius() const { return 0; }
    void setPointCount(std::size_t count) {}

    virtual std::size_t getPointCount() const override { return 0; }
    virtual Vector2f getPoint(std::size_t index) const override { return Vector2f(); }
};

struct SFML_GRAPHICS_API ConvexShape : public Shape
{
    ConvexShape(std::size_t pointCount = 0) {}
    ~ConvexShape() {}

    void setPointCount(std::size_t count) {}
    void setPoint(std::size_t index, Vector2f const & point) {}

    virtual std::size_t getPointCount() const override { return 0; }
    virtual Vector2f getPoint(std::size_t index) const override { return Vector2f(); }
};

struct SFML_GRAPHICS_API RectangleShape : public Shape
{
    Vector2f __size;

    RectangleShape(Vector2f const & size = Vector2f(0, 0)) {}
    ~RectangleShape() {}

    void setSize(Vector2f const & size) {}
    Vector2f const & getSize() const { return __size; }

    virtual std::size_t getPointCount() const override { return 0; }
    virtual Vector2f getPoint(std::size_t index) const override { return Vector2f(); }
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

    //void draw(const Drawable& drawable, const RenderStates& states = RenderStates());
    //void draw(const Vertex* vertices, std::size_t vertexCount, PrimitiveType type, const RenderStates& states = RenderStates());
    //void draw(const VertexBuffer& vertexBuffer, const RenderStates& states = RenderStates());
    //void draw(const VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const RenderStates& states = RenderStates());

    virtual Vector2u getSize() const { return Vector2u(); };
    virtual bool setActive(bool active = true) { return false; }

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

