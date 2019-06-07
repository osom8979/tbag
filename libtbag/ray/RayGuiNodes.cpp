/**
 * @file   RayGuiNodes.cpp
 * @brief  RayGuiNodes class implementation.
 * @author zer0
 * @date   2019-06-02
 * @date   2019-06-07 (Change backend: rokups/ImNodes -> Nelarius/imnodes)
 */

#include <libtbag/ray/RayGuiNodes.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Type.hpp>

#include <cassert>
#include <algorithm>
#include <utility>
#include <unordered_map>

#include <imgui.h>
#include <imnodes.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

/**
 * RayGuiNodes::Impl class implementation.
 *
 * @author zer0
 * @date   2019-06-07
 */
struct RayGuiNodes::Impl : private Noncopyable
{
    imnodes::EditorContext * context;

    Impl()
    {
        context = imnodes::EditorContextCreate();
        assert(context != nullptr);
    }

    ~Impl()
    {
        imnodes::EditorContextFree(context);
    }
};

// --------------------------
// RayGuiNodes implementation
// --------------------------

RayGuiNodes::RayGuiNodes() : _impl(std::make_shared<Impl>())
{
    assert(static_cast<bool>(_impl));
}

RayGuiNodes::RayGuiNodes(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

RayGuiNodes::RayGuiNodes(RayGuiNodes const & obj) TBAG_NOEXCEPT : RayGuiNodes(nullptr)
{
    (*this) = obj;
}

RayGuiNodes::RayGuiNodes(RayGuiNodes && obj) TBAG_NOEXCEPT : RayGuiNodes(nullptr)
{
    (*this) = std::move(obj);
}

RayGuiNodes::~RayGuiNodes()
{
    // EMPTY.
}

RayGuiNodes & RayGuiNodes::operator =(RayGuiNodes const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

RayGuiNodes & RayGuiNodes::operator =(RayGuiNodes && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void RayGuiNodes::copy(RayGuiNodes const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void RayGuiNodes::swap(RayGuiNodes & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void RayGuiNodes::reset()
{
    _impl.reset();
}

#ifndef STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES
#define STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(x) \
    STATIC_ASSERT_INTEGER_EQUAL(imnodes::x, GuiNodes##x)
#endif

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_NodeBackground);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_NodeBackgroundHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_NodeBackgroundSelected);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_NodeOutline);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_TitleBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_TitleBarHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_TitleBarSelected);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_Link);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_LinkHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_LinkSelected);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_Pin);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_PinHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_PinOutline);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_GridBackground);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_GridLine);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(ColorStyle_Count);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(Flags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(Flags_NodeOutline);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMNODES(Flags_PinOutline);

static void __imnodes_style_copy(GuiNodesStyle & dest, imnodes::Style const & src) TBAG_NOEXCEPT
{
    dest.flags = (GuiNodesFlags)src.flags;
    for (int i = 0; i < GuiNodesColorStyle_Count; ++i) {
        dest.colors[i] = (GuiNodesColorStyle)src.colors[i];
    }
}

static void __imnodes_style_copy(imnodes::Style & dest, GuiNodesStyle const & src) TBAG_NOEXCEPT
{
    dest.flags = (imnodes::Flags)src.flags;
    for (int i = 0; i < imnodes::ColorStyle_Count; ++i) {
        dest.colors[i] = (imnodes::ColorStyle)src.colors[i];
    }
}

void GuiNodesEditorContextSet(RayGuiNodes const & nodes)
{
    imnodes::EditorContextSet(nodes.get()->context);
}

void GuiNodesInitialize()
{
    imnodes::Initialize();
}

void GuiNodesShutdown()
{
    imnodes::Shutdown();
}

GuiNodesStyle GuiNodesGetStyle()
{
    GuiNodesStyle result{};
    __imnodes_style_copy(result, imnodes::GetStyle());
    return result;
}

void GuiNodesSetStyle(GuiNodesStyle const & style)
{
    __imnodes_style_copy(imnodes::GetStyle(), style);
}

void GuiNodesBeginNodeEditor()
{
    imnodes::BeginNodeEditor();
}

void GuiNodesEndNodeEditor()
{
    imnodes::EndNodeEditor();
}

void GuiNodesPushColorStyle(GuiNodesColorStyle item, unsigned int color)
{
    imnodes::PushColorStyle((imnodes::ColorStyle)item, color);
}

void GuiNodesPopColorStyle()
{
    imnodes::PopColorStyle();
}

void GuiNodesBeginNode(int id)
{
    imnodes::BeginNode(id);
}

void GuiNodesEndNode()
{
    imnodes::EndNode();
}

void GuiNodesName(char const * name)
{
    imnodes::Name(name);
}

void GuiNodesBeginInputAttribute(int id)
{
    imnodes::BeginInputAttribute(id);
}

void GuiNodesBeginOutputAttribute(int id)
{
    imnodes::BeginOutputAttribute(id);
}

void GuiNodesEndAttribute()
{
    imnodes::EndAttribute();
}

void GuiNodesLink(int id, int start_attr, int end_attr)
{
    imnodes::Link(id, start_attr, end_attr);
}

void GuiNodesSetNodePos(int node_id, Vector2 const & pos)
{
    imnodes::SetNodePos(node_id, pos);
}

bool GuiNodesIsNodeHovered(int * node_id)
{
    return imnodes::IsNodeHovered(node_id);
}

bool GuiNodesIsLinkHovered(int * link_id)
{
    return imnodes::IsLinkHovered(link_id);
}

bool GuiNodesIsPinHovered(int * attribute_id)
{
    return imnodes::IsPinHovered(attribute_id);
}

bool GuiNodesIsNodeSelected(int * node_id)
{
    return imnodes::IsNodeSelected(node_id);
}

bool GuiNodesIsLinkSelected(int * link_id)
{
    return imnodes::IsLinkSelected(link_id);
}

bool GuiNodesIsAttributeActive()
{
    return imnodes::IsAttributeActive();
}

bool GuiNodesIsAnyAttributeActive(int * attribute_id)
{
    return imnodes::IsAnyAttributeActive(attribute_id);
}

bool GuiNodesIsLinkStarted(int * started_at_attr)
{
    return imnodes::IsLinkStarted(started_at_attr);
}

bool GuiNodesIsLinkDropped()
{
    return imnodes::IsLinkDropped();
}

bool GuiNodesIsLinkCreated(int * started_at_attr, int * ended_at_attr)
{
    return imnodes::IsLinkCreated(started_at_attr, ended_at_attr);
}

char const * GuiNodesSaveCurrentEditorStateToMemory(size_t * data_size)
{
    return imnodes::SaveCurrentEditorStateToMemory(data_size);
}

char const * GuiNodesSaveEditorStateToMemory(RayGuiNodes const & editor, size_t * data_size)
{
    return imnodes::SaveEditorStateToMemory(editor.get()->context, data_size);
}

void GuiNodesLoadCurrentEditorStateFromMemory(char const * data, size_t data_size)
{
    imnodes::LoadCurrentEditorStateFromMemory(data, data_size);
}

void GuiNodesLoadEditorStateFromMemory(RayGuiNodes & editor, char const * data, size_t data_size)
{
    imnodes::LoadEditorStateFromMemory(editor.get()->context, data, data_size);
}

void GuiNodesSaveCurrentEditorStateToDisk(char const * file_name)
{
    imnodes::SaveCurrentEditorStateToDisk(file_name);
}

void GuiNodesSaveEditorStateToDisk(RayGuiNodes const & editor, char const * file_name)
{
    imnodes::SaveEditorStateToDisk(editor.get()->context, file_name);
}

void GuiNodesLoadCurrentEditorStateFromDisk(char const * file_name)
{
    imnodes::LoadCurrentEditorStateFromDisk(file_name);
}

void GuiNodesLoadEditorStateFromDisk(RayGuiNodes & editor, char const * file_name)
{
    imnodes::LoadEditorStateFromDisk(editor.get()->context, file_name);
}

/**
 * Demo implementation
 *
 * @author zer0
 * @date   2019-06-07
 *
 */
class GuiNodeDemo
{
public:
    struct Color3
    {
        float data[3] = {0, 0, 0};
    };

    struct Link
    {
        int start = 0;
        int end = 0;
    };

    using LinkMap = std::unordered_map<int, Link>;

private:
    int _current_id = 0;
    std::unordered_map<int, Link> _links;

    float _number1 = 0.0f;
    float _number2 = 1.0f;
    Color3 _color;

public:
    GuiNodeDemo()
    {
        imnodes::SetNodePos(0, ImVec2(100.0f, 100.0f));
        imnodes::SetNodePos(1, ImVec2(300.0f, 300.0f));
    }

    ~GuiNodeDemo()
    {
        // EMPTY.
    }

public:
    inline static int make_id(int node, int attribute) TBAG_NOEXCEPT
    {
        return (node << 16) | attribute;
    }

public:
    void show(bool * p_open = nullptr)
    {
        ImGui::Begin("Simple node example", p_open);
        ImGui::Text("X -- delete selected link");
        imnodes::BeginNodeEditor();

        {
            int const DRAGFLOAT_NODE_ID = 0;
            float const NODE_WIDTH = 150.0f;

            imnodes::BeginNode(DRAGFLOAT_NODE_ID);
            imnodes::Name("Drag float");

            // input attributes should all be unique among each other
            imnodes::BeginInputAttribute(make_id(DRAGFLOAT_NODE_ID, 0));
            ImGui::Text("input");
            imnodes::EndAttribute();
            ImGui::Spacing();

            {
                // If you don't want a a pin drawn on a node UI element, just
                // don't call Begin(Input|Output)Attribute before and after your
                // UI element!
                const float label_width = ImGui::CalcTextSize("number").x;
                ImGui::Text("number");
                ImGui::PushItemWidth(NODE_WIDTH - label_width - 6.0f);
                ImGui::SameLine();
                ImGui::DragFloat("##hidelabel", &_number1, 0.01f);
                ImGui::PopItemWidth();
            }

            {
                imnodes::BeginOutputAttribute(make_id(DRAGFLOAT_NODE_ID, 2));
                const float label_width = ImGui::CalcTextSize("output").x;
                ImGui::Indent(NODE_WIDTH - label_width - 1.5f);
                ImGui::Text("output");
                imnodes::EndAttribute();
            }

            imnodes::EndNode();
        }

        {
            int const BIG_NODE_ID = 1;
            float const NODE_WIDTH = 200.0f;
            imnodes::BeginNode(BIG_NODE_ID);
            imnodes::Name("Big node");

            imnodes::BeginInputAttribute(make_id(BIG_NODE_ID, 0));
            ImGui::Text("input1");
            imnodes::EndAttribute();

            ImGui::Spacing();

            imnodes::BeginInputAttribute(make_id(BIG_NODE_ID, 1));
            ImGui::Text("input2");
            imnodes::EndAttribute();

            {
                imnodes::BeginOutputAttribute(make_id(BIG_NODE_ID, 2));
                const float label_width = ImGui::CalcTextSize("number").x;
                ImGui::PushItemWidth(NODE_WIDTH - label_width - 6.0f);
                ImGui::DragFloat("number", &_number2, 0.01f);
                ImGui::PopItemWidth();
                imnodes::EndAttribute();
            }
            ImGui::Spacing();
            {
                imnodes::BeginOutputAttribute(make_id(BIG_NODE_ID, 3));
                const float label_width = ImGui::CalcTextSize("color").x;
                ImGui::PushItemWidth(NODE_WIDTH - label_width - 6.0f);
                ImGui::ColorEdit3("color", _color.data);
                ImGui::PopItemWidth();
                imnodes::EndAttribute();
            }
            ImGui::Spacing();
            {
                imnodes::BeginOutputAttribute(make_id(BIG_NODE_ID, 5));
                const float label_width = ImGui::CalcTextSize("output1").x;
                ImGui::Indent(NODE_WIDTH - label_width - 1.5f);
                ImGui::Text("output1");
                imnodes::EndAttribute();
            }
            ImGui::Spacing();
            {
                imnodes::BeginOutputAttribute(make_id(BIG_NODE_ID, 6));
                const float label_width = ImGui::CalcTextSize("output2").x;
                ImGui::Indent(NODE_WIDTH - label_width - 1.5f);
                ImGui::Text("output2");
                imnodes::EndAttribute();
            }

            imnodes::EndNode();
        }

        for (auto const & cursor : _links) {
            imnodes::Link(cursor.first, cursor.second.start, cursor.second.end);
        }
        imnodes::EndNodeEditor();

        int link_start = 0;
        int link_end = 0;
        if (imnodes::IsLinkCreated(&link_start, &link_end)) {
            _links.insert(std::make_pair(_current_id++, Link{link_start, link_end}));
        }

        int link_id = 0;
        if (imnodes::IsLinkSelected(&link_id)) {
            if (ImGui::IsKeyReleased(KEY_X)) {
                _links.erase(link_id);
            }
        }
        ImGui::End();
    }
};

void GuiNodesShowDemoWindow(bool * p_open)
{
    static GuiNodeDemo demo_window;
    demo_window.show(p_open);
}

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

