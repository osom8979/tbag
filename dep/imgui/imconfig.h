#pragma once

#include "raylib.h"

#define IM_VEC2_CLASS_EXTRA     \
    ImVec2(Vector2 const & v)   \
        : x(v.x), y(v.y)        \
    { /* EMPTY. */ }            \
    operator Vector2() const    \
    { return Vector2{x, y}; }   \
    /* -- END -- */

#define IM_VEC4_CLASS_EXTRA                 \
    ImVec4(Vector4 const & v)               \
        : x(v.x), y(v.y), z(v.z), w(v.w)    \
    { /* EMPTY. */ }                        \
    operator Vector4() const                \
    { return Vector4{x, y, z, w}; }         \
    ImVec4(Color const & c)                 \
        : x(c.r/255.f), y(c.g/255.f),       \
          z(c.b/255.f), w(c.a/255.f)        \
    { /* EMPTY. */ }                        \
    operator Color() const                  \
    {                                       \
        return Color{                       \
                (unsigned char)(x*255.f),   \
                (unsigned char)(y*255.f),   \
                (unsigned char)(z*255.f),   \
                (unsigned char)(w*255.f)};  \
    } /* -- END -- */

