/**
 * @file   RayGuiNodes.hpp
 * @brief  RayGuiNodes class prototype.
 * @author zer0
 * @date   2019-06-02
 * @date   2019-06-07 (Change backend: rokups/ImNodes -> Nelarius/imnodes)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUINODES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUINODES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ray/RayBypass.hpp>

#include <cstddef>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

/**
 * RayGuiNodes class prototype.
 *
 * @author zer0
 * @date   2019-06-07
 */
class TBAG_API RayGuiNodes
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    RayGuiNodes();
    RayGuiNodes(std::nullptr_t) TBAG_NOEXCEPT;
    RayGuiNodes(RayGuiNodes const & obj) TBAG_NOEXCEPT;
    RayGuiNodes(RayGuiNodes && obj) TBAG_NOEXCEPT;
    ~RayGuiNodes();

public:
    RayGuiNodes & operator =(RayGuiNodes const & obj) TBAG_NOEXCEPT;
    RayGuiNodes & operator =(RayGuiNodes && obj) TBAG_NOEXCEPT;

public:
    void copy(RayGuiNodes const & obj) TBAG_NOEXCEPT;
    void swap(RayGuiNodes & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(RayGuiNodes & lh, RayGuiNodes & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

public:
    friend inline bool operator <(RayGuiNodes const & x, RayGuiNodes const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(RayGuiNodes const & x, RayGuiNodes const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(RayGuiNodes const & x, RayGuiNodes const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(RayGuiNodes const & x, RayGuiNodes const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(RayGuiNodes const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(RayGuiNodes const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();
};

enum GuiNodesColorStyle
{
    GuiNodesColorStyle_NodeBackground = 0,
    GuiNodesColorStyle_NodeBackgroundHovered,
    GuiNodesColorStyle_NodeBackgroundSelected,
    GuiNodesColorStyle_NodeOutline,
    GuiNodesColorStyle_TitleBar,
    GuiNodesColorStyle_TitleBarHovered,
    GuiNodesColorStyle_TitleBarSelected,
    GuiNodesColorStyle_Link,
    GuiNodesColorStyle_LinkHovered,
    GuiNodesColorStyle_LinkSelected,
    GuiNodesColorStyle_Pin,
    GuiNodesColorStyle_PinHovered,
    GuiNodesColorStyle_PinOutline,
    GuiNodesColorStyle_GridBackground,
    GuiNodesColorStyle_GridLine,
    GuiNodesColorStyle_Count
};

enum GuiNodesFlags
{
    GuiNodesFlags_None = 0,
    GuiNodesFlags_NodeOutline = 1 << 0,
    GuiNodesFlags_PinOutline = 1 << 1
};

struct GuiNodesStyle
{
    GuiNodesFlags flags;
    unsigned int colors[GuiNodesColorStyle_Count];
};

TBAG_API void GuiNodesEditorContextSet(RayGuiNodes const & nodes);
TBAG_API void GuiNodesInitialize();
TBAG_API void GuiNodesShutdown();

TBAG_API GuiNodesStyle GuiNodesGetStyle();
TBAG_API void GuiNodesSetStyle(GuiNodesStyle const & style);

TBAG_API void GuiNodesBeginNodeEditor();
TBAG_API void GuiNodesEndNodeEditor();

TBAG_API void GuiNodesPushColorStyle(GuiNodesColorStyle item, unsigned int color);
TBAG_API void GuiNodesPopColorStyle();

TBAG_API void GuiNodesBeginNode(int id);
TBAG_API void GuiNodesEndNode();
TBAG_API void GuiNodesName(char const * name);

TBAG_API void GuiNodesBeginInputAttribute(int id);
TBAG_API void GuiNodesBeginOutputAttribute(int id);
TBAG_API void GuiNodesEndAttribute();

TBAG_API void GuiNodesLink(int id, int start_attr, int end_attr);
TBAG_API void GuiNodesSetNodePos(int node_id, Vector2 const & pos);

TBAG_API bool GuiNodesIsNodeHovered(int * node_id);
TBAG_API bool GuiNodesIsLinkHovered(int * link_id);
TBAG_API bool GuiNodesIsPinHovered(int * attribute_id);

TBAG_API bool GuiNodesIsNodeSelected(int * node_id);
TBAG_API bool GuiNodesIsLinkSelected(int * link_id);

TBAG_API bool GuiNodesIsAttributeActive();
TBAG_API bool GuiNodesIsAnyAttributeActive(int * attribute_id = nullptr);

TBAG_API bool GuiNodesIsLinkStarted(int * started_at_attr);
TBAG_API bool GuiNodesIsLinkDropped();
TBAG_API bool GuiNodesIsLinkCreated(int * started_at_attr, int * ended_at_attr);

TBAG_API char const * GuiNodesSaveCurrentEditorStateToMemory(size_t * data_size = nullptr);
TBAG_API char const * GuiNodesSaveEditorStateToMemory(RayGuiNodes const & editor, size_t * data_size = nullptr);

TBAG_API void GuiNodesLoadCurrentEditorStateFromMemory(char const * data, size_t data_size);
TBAG_API void GuiNodesLoadEditorStateFromMemory(RayGuiNodes & editor, char const * data, size_t data_size);

TBAG_API void GuiNodesSaveCurrentEditorStateToDisk(char const * file_name);
TBAG_API void GuiNodesSaveEditorStateToDisk(RayGuiNodes const & editor, char const * file_name);

TBAG_API void GuiNodesLoadCurrentEditorStateFromDisk(char const * file_name);
TBAG_API void GuiNodesLoadEditorStateFromDisk(RayGuiNodes & editor, char const * file_name);

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUINODES_HPP__

