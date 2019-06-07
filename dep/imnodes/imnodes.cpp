// the structure of this file:
//
// [SECTION] internal data structures
// [SECTION] global struct
// [SECTION] editor context definition
// [SECTION] render helpers
// [SECTION] miscellaneous helpers
// [SECTION] API implementation

#include "imnodes.h"

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <assert.h>
#include <new>
#include <string.h> // strlen, strncmp
#include <stdio.h>  // for fwrite, ssprintf, sscanf
#include <stdlib.h>

namespace imnodes
{
namespace
{
// Additional ImGui math operators
inline ImVec2 operator*(const float lhs, const ImVec2& rhs)
{
    return ImVec2(lhs * rhs.x, lhs * rhs.y);
}

inline ImVec2 operator*(const ImVec2& lhs, const float rhs)
{
    return ImVec2(lhs.x * rhs, lhs.y * rhs);
}

// Grid appearance
static const float GRID_SIZE = 32.f;

// Node appearance
static const float NODE_CORNER_ROUNDNESS = 4.0f;
static const ImVec2 NODE_CONTENT_PADDING = ImVec2(8.f, 8.f);
static const ImVec2 NODE_DUMMY_SPACING = ImVec2(80.f, 20.f);

static const float LINK_THICKNESS = 3.f;
static const float LINK_SEGMENTS_PER_LENGTH = 0.1f;
static const float LINK_HOVER_DISTANCE = 7.0f;

static const float NODE_PIN_RADIUS = 4.f;
static const float NODE_PIN_HOVER_RADIUS = 10.f;

static const size_t NODE_NAME_STR_LEN = 32u;

static const int INVALID_INDEX = -1;

bool initialized = false;

enum ScopeFlags
{
    Scope_None = 1,
    Scope_Editor = 1 << 1,
    Scope_Node = 1 << 2,
    Scope_Attribute = 1 << 3
};

enum Channels
{
    Channel_Background = 0,
    Channel_Foreground,
    Channel_Ui,
    Channel_Count
};

enum AttributeType
{
    AttributeType_None,
    AttributeType_Input,
    AttributeType_Output
};

// [SECTION] internal data structures

// The object T must have the following interface:
//
// struct T
// {
//     T();
//
//     int id;
// };
template<typename T>
struct ObjectPool
{
    ImVector<T> pool;
    ImVector<bool> in_use;
    ImVector<int> free_list;
    ImGuiStorage id_map;

    ObjectPool() : pool(), in_use(), free_list(), id_map() {}

    inline void update()
    {
        free_list.clear();
        for (int i = 0; i < in_use.size(); ++i)
        {
            if (!in_use[i])
            {
                id_map.SetInt(pool[i].id, INVALID_INDEX);
                free_list.push_back(i);
            }
        }
        // set all values to false
        memset(in_use.Data, 0, sizeof(bool) * in_use.size());
    }

    inline T& find_or_create_new(int id)
    {
        int index = id_map.GetInt(ImGuiID(id), INVALID_INDEX);
        if (index == INVALID_INDEX)
        {
            if (free_list.size() > 0)
            {
                index = free_list.back();
                free_list.pop_back();
            }
            else
            {
                index = pool.size();
                pool.push_back(T());
                in_use.push_back(true);
            }
            id_map.SetInt(id, index);
        }
        in_use[index] = true;
        return pool[index];
    }
};

struct NodeData
{
    int id;
    char name[NODE_NAME_STR_LEN];
    ImVec2 origin;
    ImRect content_rect;

    struct
    {
        ImU32 background, background_hovered, background_selected, outline,
            titlebar, titlebar_hovered, titlebar_selected;
    } color_style;

    ImVector<ImRect> attribute_rects;

    NodeData()
        : id(0u),
          name(
              "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"),
          origin(100.0f, 100.0f),
          content_rect(ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f)), color_style(),
          attribute_rects()
    {
    }
};

struct PinData
{
    int id;
    int node_idx;
    int attribute_idx;
    AttributeType type;

    struct
    {
        ImU32 background, hovered, outline;
    } color_style;

    PinData()
        : node_idx(INVALID_INDEX), attribute_idx(INVALID_INDEX),
          type(AttributeType_None), color_style()
    {
    }

    PinData(int nidx, int aindx, AttributeType t)
        : node_idx(nidx), attribute_idx(aindx), type(t), color_style()
    {
    }
};

bool operator==(const PinData& lhs, const PinData& rhs)
{
    return lhs.node_idx == rhs.node_idx &&
           lhs.attribute_idx == rhs.attribute_idx && lhs.type == rhs.type;
}

struct LinkData
{
    int id;
    int start_attr, end_attr;

    struct
    {
        ImU32 base, hovered, selected;
    } color_style;

    LinkData()
        : start_attr(INVALID_INDEX), end_attr(INVALID_INDEX), color_style()
    {
    }
};

struct LinkBezierData
{
    // the bezier curve control points
    ImVec2 p0, p1, p2, p3;
    int num_segments;
};

struct ColorStyleElement
{
    ImU32 color;
    ColorStyle item;

    ColorStyleElement(ImU32 c, ColorStyle s) : color(c), item(s) {}
};

// [SECTION] global struct
// this stores data which only lives for one frame
struct
{
    EditorContext* default_editor_ctx;
    EditorContext* editor_ctx;
    ImVec2 grid_origin;
    ScopeFlags current_scope;

    Style style;
    ImVector<ColorStyleElement> color_style_stack;

    LinkData link_created; // per-frame data, so can be stored in g

    struct
    {
        int index;
        struct
        {
            AttributeType type;
            int index;
        } attribute;
    } node_current;

    struct
    {
        int index;
        int attribute;
    } node_active;

    struct
    {
        int index;
        ImVec2 position;
    } node_moved;

    int node_hovered;
    int node_selected;
    int link_hovered;
    int link_selected;
    int pin_hovered;
    bool link_dropped;

    ImGuiTextBuffer text_buffer;
} g;

EditorContext& editor_context_get()
{
    assert(g.editor_ctx != nullptr);
    return *g.editor_ctx;
}

inline ImVec2 editor_space_to_screen_space(const ImVec2& v)
{
    return g.grid_origin + v;
}

inline ImRect get_item_rect()
{
    return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

inline ImVec2 get_node_title_origin(const NodeData& node)
{
    return node.origin + NODE_CONTENT_PADDING;
}

inline ImVec2 get_node_content_origin(const NodeData& node)
{
    ImVec2 title_rect_height = ImVec2(
        0.f, ImGui::CalcTextSize(node.name).y + 2.f * NODE_CONTENT_PADDING.y);
    return node.origin + NODE_CONTENT_PADDING + title_rect_height;
}

inline ImRect get_title_bar_rect(const NodeData& node)
{
    const ImVec2 ss_node_origin = editor_space_to_screen_space(node.origin);
    // TODO: lots of repetition of ImGui::CalcTextSize()
    // it should be calculated once and stored (in the node?)
    // TODO: this calculation is also done in get_node_rect...
    const ImVec2 text_size = ImGui::CalcTextSize(node.name);
    const float max_width =
        ImMax(node.content_rect.Min.x + text_size.x, node.content_rect.Max.x);
    const ImVec2 min = ss_node_origin;
    const ImVec2 max = ImVec2(
        max_width + NODE_CONTENT_PADDING.x,
        ss_node_origin.y + text_size.y + 2.f * NODE_CONTENT_PADDING.y);
    // NOTE: the content rect already contains 1 x NODE_CONTENT_PADDING due to
    // setting the cursor!
    return ImRect(min, max);
}

inline ImRect get_node_rect(const NodeData& node)
{
    const ImVec2 text_size = ImGui::CalcTextSize(node.name);
    const float max_width =
        ImMax(node.content_rect.Min.x + text_size.x, node.content_rect.Max.x);
    // apply the node padding on the top and bottom of the text
    const float text_height = text_size.y + 2.f * NODE_CONTENT_PADDING.y;

    ImRect rect = node.content_rect;
    rect.Max.x = max_width;
    rect.Expand(NODE_CONTENT_PADDING);
    rect.Min.y = rect.Min.y - text_height;
    return rect;
}

inline bool is_mouse_hovering_near_point(const ImVec2& point, float radius)
{
    ImVec2 delta = ImGui::GetIO().MousePos - point;
    return (delta.x * delta.x + delta.y * delta.y) < (radius * radius);
}

inline ImVec2 eval_bezier(
    float t,
    const ImVec2 p0,
    const ImVec2 p1,
    const ImVec2 p2,
    const ImVec2 p3)
{
    // B(t) = (1-t)**3 p0 + 3(1 - t)**2 t P1 + 3(1-t)t**2 P2 + t**3 P3
    return ImVec2(
        (1 - t) * (1 - t) * (1 - t) * p0.x + 3 * (1 - t) * (1 - t) * t * p1.x +
            3 * (1 - t) * t * t * p2.x + t * t * t * p3.x,
        (1 - t) * (1 - t) * (1 - t) * p0.y + 3 * (1 - t) * (1 - t) * t * p1.y +
            3 * (1 - t) * t * t * p2.y + t * t * t * p3.y);
}

// Divides the bezier curve into n segments. Evaluates the distance to each
// segment. Chooses the segment with the smallest distance, and repeats the
// algorithm on that segment, for the given number of iterations.
inline float get_closest_point_on_cubic_bezier(
    const int num_iterations,
    const int num_segments,
    const ImVec2 pos,
    const ImVec2 p0,
    const ImVec2 p1,
    const ImVec2 p2,
    const ImVec2 p3)
{
    assert(num_iterations > 0 && num_segments > 0);
    float tstart = 0.0f;
    float tend = 1.0f;
    float tbest = 0.5f;
    float best_distance = FLT_MAX;

    for (int i = 0; i < num_iterations; i++)
    {
        // split the current t-range to segments
        const float dt = (tend - tstart) / num_segments;
        for (int s = 0; s < num_segments; s++)
        {
            const float tmid = tstart + dt * (float(s) + 0.5f);
            ImVec2 bt = eval_bezier(tmid, p0, p1, p2, p3);
            ImVec2 dv = bt - pos;
            float cur_distance = ImLengthSqr(dv);
            if (cur_distance < best_distance)
            {
                best_distance = cur_distance;
                tbest = tmid;
            }
        }
        // shrink the current t-range to the best segment
        tstart = tbest - 0.5f * dt;
        tend = tbest + 0.5f * dt;
    }

    return tbest;
}

inline float get_distance_to_cubic_bezier(
    const ImVec2& pos,
    const ImVec2& p0,
    const ImVec2& p1,
    const ImVec2& p2,
    const ImVec2& p3)
{
    const int segments = 5;
    const float length = ImSqrt(ImLengthSqr(p3 - p2)) +
                         ImSqrt(ImLengthSqr(p2 - p1)) +
                         ImSqrt(ImLengthSqr(p1 - p0));
    const float iterations_per_length = 0.01f;
    const int iterations =
        int(ImClamp(length * iterations_per_length, 2.0f, 8.f));

    const float t = get_closest_point_on_cubic_bezier(
        iterations, segments, pos, p0, p1, p2, p3);
    ImVec2 point_on_curve = eval_bezier(t, p0, p1, p2, p3);

    const ImVec2 to_curve = point_on_curve - pos;
    return ImSqrt(ImLengthSqr(to_curve));
}

inline LinkBezierData get_link_renderable(
    ImVec2 start,
    ImVec2 end,
    AttributeType start_type)
{
    assert(
        (start_type == AttributeType_Input) ||
        (start_type == AttributeType_Output));
    if (start_type == AttributeType_Input)
    {
        ImSwap(start, end);
    }
    // function arguments assed by value, since we mutate them
    const ImVec2 delta = end - start;
    const float link_length = ImSqrt(ImLengthSqr(delta));
    const ImVec2 offset = ImVec2(0.25f * link_length, 0.f);
    LinkBezierData bezier;
    bezier.p0 = start;
    bezier.p1 = start + offset;
    bezier.p2 = end - offset;
    bezier.p3 = end;
    bezier.num_segments = ImMax(int(link_length * LINK_SEGMENTS_PER_LENGTH), 1);
    return bezier;
}

inline bool is_mouse_hovering_near_link(
    const ImVec2& p0,
    const ImVec2& p1,
    const ImVec2& p2,
    const ImVec2& p3)
{
    bool near = false;
    const ImVec2 mouse_pos = ImGui::GetIO().MousePos;

    // First, do an AABB test to see whether it the distance
    // to the line is worth checking in greater detail
    float xmin = ImMin(p0.x, p3.x);
    float xmax = ImMax(p0.x, p3.x);
    float ymin = ImMin(p0.y, p3.y);
    float ymax = ImMax(p0.y, p3.y);

    if ((mouse_pos.x > xmin && mouse_pos.x < xmax) &&
        (mouse_pos.y > ymin && mouse_pos.y < ymax))
    {
        const float distance =
            get_distance_to_cubic_bezier(mouse_pos, p0, p1, p2, p3);
        if (distance < LINK_HOVER_DISTANCE)
        {
            near = true;
        }
    }

    return near;
}

inline ImVec2 pin_position(
    const ImRect& node_rect,
    const ImRect& attr_rect,
    AttributeType type)
{
    assert(type == AttributeType_Input || type == AttributeType_Output);
    const float x =
        type == AttributeType_Input ? node_rect.Min.x : node_rect.Max.x;
    return ImVec2(x, 0.5f * (attr_rect.Min.y + attr_rect.Max.y));
}
} // namespace

// [SECTION] editor context definition

struct EditorContext
{
    ObjectPool<NodeData> nodes;
    ObjectPool<PinData> pins;
    ObjectPool<LinkData> links;

    // ui related fields
    ImVec2 panning;
    ImDrawList* grid_draw_list;

    LinkData link_dragged;

    EditorContext()
        : nodes(), pins(), links(), panning(0.f, 0.f), grid_draw_list(nullptr),
          link_dragged()
    {
    }
};

// [SECTION] render helpers

namespace
{
void draw_grid(const EditorContext& editor)
{
    const ImVec2 offset = editor.panning;
    const ImVec2 canvas_size = ImGui::GetWindowSize();
    for (float x = fmodf(offset.x, GRID_SIZE); x < canvas_size.x;
         x += GRID_SIZE)
    {
        editor.grid_draw_list->AddLine(
            editor_space_to_screen_space(ImVec2(x, 0.0f)),
            editor_space_to_screen_space(ImVec2(x, canvas_size.y)),
            g.style.colors[ColorStyle_GridLine]);
    }
    for (float y = fmodf(offset.y, GRID_SIZE); y < canvas_size.y;
         y += GRID_SIZE)
    {
        editor.grid_draw_list->AddLine(
            editor_space_to_screen_space(ImVec2(0.0f, y)),
            editor_space_to_screen_space(ImVec2(canvas_size.x, y)),
            g.style.colors[ColorStyle_GridLine]);
    }
}

void draw_pin(const EditorContext& editor, const int pin_idx)
{
    const PinData& pin = editor.pins.pool[pin_idx];
    const NodeData& node = editor.nodes.pool[pin.node_idx];
    const ImRect node_rect = get_node_rect(node);
    const ImVec2 pin_pos = pin_position(
        node_rect, node.attribute_rects[pin.attribute_idx], pin.type);
    if (is_mouse_hovering_near_point(pin_pos, NODE_PIN_HOVER_RADIUS))
    {
        g.pin_hovered = pin_idx;
        editor.grid_draw_list->AddCircleFilled(
            pin_pos, NODE_PIN_RADIUS, pin.color_style.hovered);
    }
    else
    {
        editor.grid_draw_list->AddCircleFilled(
            pin_pos, NODE_PIN_RADIUS, pin.color_style.background);
    }
    if ((g.style.flags & Flags_PinOutline) != 0)
    {
        editor.grid_draw_list->AddCircle(
            pin_pos, NODE_PIN_RADIUS, pin.color_style.outline);
    }
}

void draw_node(const EditorContext& editor, int node_idx)
{
    const NodeData& node = editor.nodes.pool[node_idx];
    ImGui::PushID(node.id);

    editor.grid_draw_list->ChannelsSetCurrent(Channel_Foreground);

    const ImRect node_rect = get_node_rect(node);

    ImGui::SetCursorPos(node.origin + editor.panning);
    ImGui::InvisibleButton(node.name, node_rect.GetSize());

    if (ImGui::IsItemHovered())
    {
        g.node_hovered = node_idx;
        if (ImGui::IsMouseClicked(0))
        {
            g.node_selected = g.node_hovered;
        }
    }

    // Check to see whether the node moved during the frame. The node's position
    // is updated after the node has been drawn (because the user has already
    // rendered the UI!).
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
    {
        g.node_moved.index = node_idx;
        g.node_moved.position = node.origin + ImGui::GetIO().MouseDelta;
    }

    ImU32 node_background = node.color_style.background;
    ImU32 titlebar_background = node.color_style.titlebar;

    if (g.node_selected == node_idx)
    {
        node_background = node.color_style.background_selected;
        titlebar_background = node.color_style.titlebar_selected;
    }
    else if (g.node_hovered == node_idx)
    {
        node_background = node.color_style.background_hovered;
        titlebar_background = node.color_style.titlebar_hovered;
    }

    {
        // node base
        editor.grid_draw_list->AddRectFilled(
            node_rect.Min,
            node_rect.Max,
            node_background,
            NODE_CORNER_ROUNDNESS);

        // title bar:
        {
            ImRect title_rect = get_title_bar_rect(node);
            // TODO: better consistency here?
            // why subtract panning from Min, and then subtract it only from the
            // y-component of Max? it's because Max.x is computed from the node
            // rect, which already has the panning subtracted from it! the
            // y-component is just looked up from the text height
            title_rect.Min += editor.panning;
            title_rect.Max.y += editor.panning.y;
            editor.grid_draw_list->AddRectFilled(
                title_rect.Min,
                title_rect.Max,
                titlebar_background,
                NODE_CORNER_ROUNDNESS,
                ImDrawCornerFlags_Top);
            ImGui::SetCursorPos(get_node_title_origin(node) + editor.panning);
            ImGui::PushItemWidth(title_rect.Max.x - title_rect.Min.x);
            ImGui::TextUnformatted(node.name);
            ImGui::PopItemWidth();
        }
        if ((g.style.flags & Flags_NodeOutline) != 0)
        {
            editor.grid_draw_list->AddRect(
                node_rect.Min,
                node_rect.Max,
                node.color_style.outline,
                NODE_CORNER_ROUNDNESS);
        }
    }

    ImGui::PopID();
}

void draw_link(const EditorContext& editor, int link_idx)
{
    const LinkData& link = editor.links.pool[link_idx];

    const int start_idx = editor.pins.id_map.GetInt(link.start_attr);
    const int end_idx = editor.pins.id_map.GetInt(link.end_attr);
    assert(start_idx != INVALID_INDEX);
    assert(end_idx != INVALID_INDEX);
    const PinData& pin_start = editor.pins.pool[start_idx];
    const PinData& pin_end = editor.pins.pool[end_idx];

    ImVec2 start, end;
    {
        // TODO: maybe compute the current node_rect and store it instead of
        // recomputing it all the time
        const ImRect node_rect =
            get_node_rect(editor.nodes.pool[pin_start.node_idx]);
        start = pin_position(
            node_rect,
            editor.nodes.pool[pin_start.node_idx]
                .attribute_rects[pin_start.attribute_idx],
            pin_start.type);
    }

    {
        const ImRect node_rect =
            get_node_rect(editor.nodes.pool[pin_end.node_idx]);
        end = pin_position(
            node_rect,
            editor.nodes.pool[pin_end.node_idx]
                .attribute_rects[pin_end.attribute_idx],
            pin_end.type);
    }

    const LinkBezierData link_renderable =
        get_link_renderable(start, end, pin_start.type);

    const bool is_hovered = is_mouse_hovering_near_link(
        link_renderable.p0,
        link_renderable.p1,
        link_renderable.p2,
        link_renderable.p3);
    if (is_hovered)
    {
        g.link_hovered = link_idx;
        if (ImGui::IsMouseClicked(0))
        {
            g.link_selected = link_idx;
        }
    }

    ColorStyle style = ColorStyle_Link;
    if (g.link_selected == link_idx)
    {
        style = ColorStyle_LinkSelected;
    }
    else if (is_hovered)
    {
        style = ColorStyle_LinkHovered;
    }

    const ImU32 link_color = g.style.colors[style];

    editor.grid_draw_list->AddBezierCurve(
        link_renderable.p0,
        link_renderable.p1,
        link_renderable.p2,
        link_renderable.p3,
        link_color,
        LINK_THICKNESS,
        link_renderable.num_segments);
}

// SECTION miscellaneous helpers

void begin_attribute(int id, AttributeType type)
{
    // Make sure to call BeginNode() before calling
    // BeginAttribute()
    assert(g.current_scope == Scope_Node);
    g.current_scope = Scope_Attribute;

    ImGui::BeginGroup();
    ImGui::PushID(id);

    EditorContext& editor = editor_context_get();

    g.node_current.attribute.type = type;
    g.node_current.attribute.index =
        editor.nodes.pool[g.node_current.index].attribute_rects.size();

    PinData& pin = editor.pins.find_or_create_new(id);
    pin.id = id;
    pin.node_idx = g.node_current.index;
    pin.attribute_idx = g.node_current.attribute.index;
    pin.type = type;
    pin.color_style.background = g.style.colors[ColorStyle_Pin];
    pin.color_style.hovered = g.style.colors[ColorStyle_PinHovered];
    pin.color_style.outline = g.style.colors[ColorStyle_PinOutline];
}
} // namespace

// [SECTION] API implementation

EditorContext* EditorContextCreate()
{
    void* mem = ImGui::MemAlloc(sizeof(EditorContext));
    new (mem) EditorContext();
    return (EditorContext*)mem;
}

void EditorContextFree(EditorContext* ctx)
{
    ctx->~EditorContext();
    ImGui::MemFree(ctx);
}

void EditorContextSet(EditorContext* ctx) { g.editor_ctx = ctx; }

void Initialize()
{
    assert(initialized == false);
    initialized = true;

    g.default_editor_ctx = NULL;
    g.editor_ctx = NULL;
    g.grid_origin = ImVec2(0.0f, 0.0f);
    g.current_scope = Scope_None;

    g.default_editor_ctx = EditorContextCreate();
    EditorContextSet(g.default_editor_ctx);

    g.node_current.index = INVALID_INDEX;
    g.node_current.attribute.type = AttributeType_None;
    g.node_current.attribute.index = INVALID_INDEX;

    g.link_created = LinkData();

    g.node_hovered = INVALID_INDEX;
    g.node_selected = INVALID_INDEX;
    g.link_hovered = INVALID_INDEX;
    g.link_selected = INVALID_INDEX;

    g.style.colors[ColorStyle_NodeBackground] = IM_COL32(60, 60, 60, 255);
    g.style.colors[ColorStyle_NodeBackgroundHovered] =
        IM_COL32(75, 75, 75, 255);
    g.style.colors[ColorStyle_NodeBackgroundSelected] =
        IM_COL32(75, 75, 75, 255); // TODO: different default value for this?
    g.style.colors[ColorStyle_NodeOutline] = IM_COL32(100, 100, 100, 255);
    g.style.colors[ColorStyle_TitleBar] = IM_COL32(60, 0, 40, 255);
    g.style.colors[ColorStyle_TitleBarHovered] = IM_COL32(85, 0, 55, 255);
    g.style.colors[ColorStyle_TitleBarSelected] =
        IM_COL32(85, 0, 55, 255); // TODO: different default value for this?
    g.style.colors[ColorStyle_Link] = IM_COL32(200, 200, 100, 255);
    g.style.colors[ColorStyle_LinkHovered] = IM_COL32(250, 250, 100, 255);
    g.style.colors[ColorStyle_LinkSelected] = IM_COL32(255, 255, 255, 255);
    g.style.colors[ColorStyle_Pin] = IM_COL32(150, 150, 150, 255);
    g.style.colors[ColorStyle_PinHovered] = IM_COL32(200, 200, 100, 255);
    g.style.colors[ColorStyle_PinOutline] = IM_COL32(200, 200, 200, 255);
    g.style.colors[ColorStyle_GridBackground] = IM_COL32(40, 40, 50, 200);
    g.style.colors[ColorStyle_GridLine] = IM_COL32(200, 200, 200, 40);
    g.style.flags = Flags(Flags_NodeOutline | Flags_PinOutline);
}

void Shutdown() { EditorContextFree(g.default_editor_ctx); }

Style& GetStyle() { return g.style; }

void BeginNodeEditor()
{
    // Remember to call Initialize() before calling BeginNodeEditor()
    assert(initialized);
    assert(g.current_scope == Scope_None);
    g.current_scope = Scope_Editor;

    // Reset state from previous pass

    g.link_created = LinkData();
    g.node_active.index = INVALID_INDEX;
    g.node_active.attribute = INVALID_INDEX;
    g.node_moved.index = INVALID_INDEX;
    g.node_moved.position = ImVec2(0.0f, 0.0f);
    g.node_hovered = INVALID_INDEX;
    g.link_hovered = INVALID_INDEX;
    g.pin_hovered = INVALID_INDEX;
    g.link_dropped = false;

    // reset ui content for the current editor
    EditorContext& editor = editor_context_get();
    editor.nodes.update();
    editor.pins.update();
    editor.links.update();

    assert(editor.grid_draw_list == nullptr);

    ImGui::BeginGroup();
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.f, 1.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::PushStyleColor(
            ImGuiCol_ChildWindowBg, g.style.colors[ColorStyle_GridBackground]);
        ImGui::BeginChild(
            "scrolling_region",
            ImVec2(0.f, 0.f),
            true,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
        g.grid_origin = ImGui::GetCursorScreenPos();
        // prepare for layering the node content on top of the nodes
        // NOTE: the draw list has to be captured here, because we want all the
        // content to clip the scrolling_region child window.
        editor.grid_draw_list = ImGui::GetWindowDrawList();
        editor.grid_draw_list->ChannelsSplit(Channel_Count);
        editor.grid_draw_list->ChannelsSetCurrent(Channel_Background);

        // TODO: showing the grid should be a setting
        if (/*show_grid*/ true)
        {
            draw_grid(editor);
        }
    }

    editor.grid_draw_list->ChannelsSetCurrent(Channel_Ui);
}

void EndNodeEditor()
{
    assert(g.current_scope == Scope_Editor);
    g.current_scope = Scope_None;

    EditorContext& editor = editor_context_get();

    const bool is_mouse_clicked = ImGui::IsMouseClicked(0);

    for (int i = 0; i < editor.nodes.pool.size(); i++)
    {
        // TODO: perhaps consider putting this inside a BeginChild()  block
        // to see if that would solve the overlapping node problem
        if (editor.nodes.in_use[i])
        {
            draw_node(editor, i);
        }
    }

    for (int i = 0; i < editor.pins.pool.size(); i++)
    {
        if (editor.pins.in_use[i])
        {
            draw_pin(editor, i);
        }
    }

    // check to see if the mouse was near any node
    if (g.node_hovered == INVALID_INDEX)
    {
        if (is_mouse_clicked)
        {
            g.node_selected = INVALID_INDEX;
        }
    }

    editor.grid_draw_list->ChannelsSetCurrent(Channel_Background);

    for (int i = 0; i < editor.links.pool.size(); ++i)
    {
        if (editor.links.in_use[i])
        {
            draw_link(editor, i);
        }
    }

    if (g.link_hovered == INVALID_INDEX)
    {
        if (is_mouse_clicked)
        {
            g.link_selected = INVALID_INDEX;
        }
    }

    // See if the user started to drag a link from a pin

    if (is_mouse_clicked && g.pin_hovered != INVALID_INDEX)
    {
        editor.link_dragged.start_attr = editor.pins.pool[g.pin_hovered].id;
    }

    if (editor.link_dragged.start_attr != INVALID_INDEX)
    {
        // Render the link being dragged
        if (ImGui::IsMouseDown(0))
        {
            int pin_idx = editor.pins.id_map.GetInt(
                ImGuiID(editor.link_dragged.start_attr), INVALID_INDEX);
            assert(pin_idx != INVALID_INDEX);
            const PinData& pin = editor.pins.pool[pin_idx];
            const NodeData& node =
                editor_context_get().nodes.pool[pin.node_idx];
            const ImRect node_rect = get_node_rect(node);

            const ImVec2 start_pos = pin_position(
                node_rect, node.attribute_rects[pin.attribute_idx], pin.type);
            const ImVec2 end_pos = ImGui::GetIO().MousePos;

            const LinkBezierData link_renderable =
                get_link_renderable(start_pos, end_pos, pin.type);
            editor.grid_draw_list->AddBezierCurve(
                link_renderable.p0,
                link_renderable.p1,
                link_renderable.p2,
                link_renderable.p3,
                g.style.colors[ColorStyle_Link],
                LINK_THICKNESS,
                link_renderable.num_segments);
        }

        // Finish the new link

        if (ImGui::IsMouseReleased(0))
        {
            // The link was created if the mouse is released near a pin
            // Check that the pin isn't the same on that the link was started
            // on!
            if (g.pin_hovered != INVALID_INDEX &&
                editor.pins.pool[g.pin_hovered].id !=
                    editor.link_dragged.start_attr)
            {
                editor.link_dragged.end_attr =
                    editor.pins.pool[g.pin_hovered].id;

                g.link_created.start_attr = editor.link_dragged.start_attr;
                g.link_created.end_attr = editor.link_dragged.end_attr;
            }
            // else the link is dropped
            else
            {
                g.link_dropped = true;
            }
            // finally, reset the dragged link
            editor.link_dragged = LinkData();
        }
    }

    if (g.node_moved.index != INVALID_INDEX &&
        editor.link_dragged.start_attr == INVALID_INDEX)
    {
        // Don't move the node if we're dragging a link
        editor.nodes.pool[g.node_moved.index].origin = g.node_moved.position;
    }

    // set channel 0 before merging, or else UI rendering is broken
    editor.grid_draw_list->ChannelsSetCurrent(0);
    editor.grid_draw_list->ChannelsMerge();
    editor.grid_draw_list = nullptr;

    // apply panning if the mouse was dragged
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() &&
        ImGui::IsMouseDragging(2, 0))
    {
        editor.panning = editor.panning + ImGui::GetIO().MouseDelta;
    }

    // pop style
    ImGui::EndChild();      // end scrolling region
    ImGui::PopStyleColor(); // pop child window background color
    ImGui::PopStyleVar();   // pop window padding
    ImGui::PopStyleVar();   // pop frame padding
    ImGui::EndGroup();

    for (int idx = 0; idx < editor.nodes.pool.size(); idx++)
    {
        NodeData& node = editor.nodes.pool[idx];

        node.attribute_rects.clear();
    }
}

void BeginNode(int node_id)
{
    // Remember to call BeginNodeEditor before calling BeginNode
    assert(g.current_scope == Scope_Editor);
    g.current_scope = Scope_Node;
    assert(g.node_current.index == INVALID_INDEX);

    EditorContext& editor = editor_context_get();

    NodeData& node = editor.nodes.find_or_create_new(node_id);
    node.id = node_id;
    node.color_style.background = g.style.colors[ColorStyle_NodeBackground];
    node.color_style.background_hovered =
        g.style.colors[ColorStyle_NodeBackgroundHovered];
    node.color_style.background_selected =
        g.style.colors[ColorStyle_NodeBackgroundSelected];
    node.color_style.outline = g.style.colors[ColorStyle_NodeOutline];
    node.color_style.titlebar = g.style.colors[ColorStyle_TitleBar];
    node.color_style.titlebar_hovered =
        g.style.colors[ColorStyle_TitleBarHovered];
    node.color_style.titlebar_selected =
        g.style.colors[ColorStyle_TitleBarSelected];

    {
        const int idx = editor.nodes.id_map.GetInt(node_id, INVALID_INDEX);
        assert(idx != INVALID_INDEX);
        g.node_current.index = idx;
    }

    ImGui::SetCursorPos(get_node_content_origin(node) + editor.panning);

    ImGui::PushID(node_id);
    ImGui::BeginGroup();
}

void Name(const char* name)
{
    assert(g.current_scope == Scope_Node);

    NodeData& node = editor_context_get().nodes.pool[g.node_current.index];
    assert(strlen(name) < NODE_NAME_STR_LEN);
    memset(node.name, 0, NODE_NAME_STR_LEN);
    memcpy(node.name, name, strlen(name));
}

void EndNode()
{
    assert(g.current_scope == Scope_Node);
    g.current_scope = Scope_Editor;

    EditorContext& editor = editor_context_get();

    ImGui::EndGroup();
    ImGui::PopID();
    editor.nodes.pool[g.node_current.index].content_rect = get_item_rect();
    g.node_current.index = INVALID_INDEX;
}

void BeginInputAttribute(int id) { begin_attribute(id, AttributeType_Input); }

void BeginOutputAttribute(int id) { begin_attribute(id, AttributeType_Output); }

void EndAttribute()
{
    assert(g.current_scope == Scope_Attribute);
    g.current_scope = Scope_Node;

    ImGui::PopID();
    ImGui::EndGroup();

    if (ImGui::IsItemActive())
    {
        g.node_active.index = g.node_current.index;
        g.node_active.attribute = g.node_current.attribute.index;
    }

    NodeData& node_current =
        editor_context_get().nodes.pool[g.node_current.index];
    node_current.attribute_rects.push_back(get_item_rect());
}

void Link(int id, const int start_attr, const int end_attr)
{
    assert(g.current_scope == Scope_Editor);

    EditorContext& editor = editor_context_get();
    LinkData& link = editor.links.find_or_create_new(id);
    link.id = id;
    link.start_attr = start_attr;
    link.end_attr = end_attr;
    link.color_style.base = g.style.colors[ColorStyle_Link];
    link.color_style.hovered = g.style.colors[ColorStyle_LinkHovered];
    link.color_style.selected = g.style.colors[ColorStyle_LinkSelected];
}

void PushColorStyle(ColorStyle item, unsigned int color)
{
    // Remember to call Initialize() before using any other functions!
    assert(initialized);
    g.color_style_stack.push_back(
        ColorStyleElement(g.style.colors[item], item));
    g.style.colors[item] = color;
}

void PopColorStyle()
{
    assert(g.color_style_stack.size() > 0);
    const ColorStyleElement elem = g.color_style_stack.back();
    g.style.colors[elem.item] = elem.color;
    g.color_style_stack.pop_back();
}

void SetNodePos(int node_id, const ImVec2& screen_space_pos)
{
    // Remember to call Initialize() before using any other functions!
    assert(initialized);
    EditorContext& editor = editor_context_get();
    NodeData& node = editor.nodes.find_or_create_new(node_id);
    node.origin =
        screen_space_pos - editor_context_get().panning - g.grid_origin;
}

bool IsNodeHovered(int* const node_id)
{
    assert(g.current_scope == Scope_None);
    assert(node_id != NULL);

    const bool is_hovered = g.node_hovered != INVALID_INDEX;
    if (is_hovered)
    {
        *node_id = g.node_hovered;
        return true;
    }
    return false;
}

bool IsLinkHovered(int* const link_id)
{
    assert(g.current_scope == Scope_None);
    assert(link_id != NULL);

    const bool is_hovered = g.link_hovered != INVALID_INDEX;
    if (is_hovered)
    {
        const EditorContext& editor = editor_context_get();
        *link_id = editor.links.pool[g.link_hovered].id;
    }
    return is_hovered;
}

bool IsPinHovered(int* const attr)
{
    assert(g.current_scope == Scope_None);
    assert(attr != NULL);

    const bool is_hovered = g.pin_hovered != INVALID_INDEX;
    if (is_hovered)
    {
        const EditorContext& editor = editor_context_get();
        *attr = editor.pins.pool[g.pin_hovered].id;
    }
    return is_hovered;
}

bool IsNodeSelected(int* const node_id)
{
    assert(g.current_scope == Scope_None);
    assert(node_id != NULL);

    const bool is_selected = g.node_selected != INVALID_INDEX;
    if (is_selected)
    {
        const EditorContext& editor = editor_context_get();
        *node_id = editor.nodes.pool[g.node_selected].id;
        return true;
    }
    return false;
}

bool IsLinkSelected(int* const link_id)
{
    assert(g.current_scope == Scope_None);
    assert(link_id != NULL);

    const bool is_selected = g.link_selected != INVALID_INDEX;
    if (is_selected)
    {
        const EditorContext& editor = editor_context_get();
        *link_id = editor.links.pool[g.link_selected].id;
    }
    return is_selected;
}

bool IsAttributeActive()
{
    return g.node_active.attribute == g.node_current.attribute.index;
}

bool IsAnyAttributeActive(int* const attribute_id)
{
    assert((g.current_scope & (Scope_None | Scope_Editor)) != 0);
    if (g.node_active.attribute != INVALID_INDEX)
    {
        if (attribute_id != NULL)
        {
            *attribute_id = g.node_active.attribute;
        }
        return true;
    }
    return false;
}

bool IsLinkStarted(int* const started_at)
{
    assert(g.current_scope == Scope_None);
    assert(started_at != NULL);

    const EditorContext& editor = editor_context_get();
    const bool is_started = editor.link_dragged.start_attr != INVALID_INDEX;
    if (is_started)
    {
        *started_at = editor.link_dragged.start_attr;
    }
    return is_started;
}

bool IsLinkDropped()
{
    assert(g.current_scope == Scope_None);
    return g.link_dropped;
}

bool IsLinkCreated(int* const started_at, int* const ended_at)
{
    assert(g.current_scope == Scope_None);
    assert(started_at != NULL);
    assert(ended_at != NULL);

    if ((g.link_created.start_attr != INVALID_INDEX) &&
        (g.link_created.end_attr != INVALID_INDEX))
    {
        *started_at = g.link_created.start_attr;
        *ended_at = g.link_created.end_attr;
        return true;
    }

    return false;
}

namespace
{
void node_line_handler(EditorContext& editor, const char* line)
{
    int i;
    float x, y;
    if (sscanf(line, "[node.%i", &i) == 1)
    {
        NodeData& node = editor.nodes.find_or_create_new(i);
        node.id = i;
        // the next case won't work unless this assumption is true
        assert(
            editor.nodes.id_map.GetInt(i, INVALID_INDEX) ==
            editor.nodes.pool.size() - 1);
    }
    else if (sscanf(line, "origin=%f,%f", &x, &y) == 2)
    {
        editor.nodes.pool.back().origin = ImVec2(x, y);
    }
}

void editor_line_handler(EditorContext& editor, const char* line)
{
    sscanf(line, "panning=%f,%f", &editor.panning.x, &editor.panning.y);
}
} // namespace

const char* SaveCurrentEditorStateToMemory(size_t* data_size)
{
    return SaveEditorStateToMemory(&editor_context_get());
}

const char* SaveEditorStateToMemory(
    const EditorContext* editor_ptr,
    size_t* data_size)
{
    assert(editor_ptr != NULL);
    const EditorContext& editor = *editor_ptr;

    g.text_buffer.clear();
    // TODO: check to make sure that the estimate is the upper bound of element
    g.text_buffer.reserve(64 * editor.nodes.pool.size());

    g.text_buffer.appendf(
        "[editor]\npanning=%i,%i\n",
        (int)editor.panning.x,
        (int)editor.panning.y);

    for (int i = 0; i < editor.nodes.pool.size(); i++)
    {
        if (editor.nodes.in_use[i])
        {
            const NodeData& node = editor.nodes.pool[i];
            g.text_buffer.appendf("\n[node.%d]\n", node.id);
            g.text_buffer.appendf(
                "origin=%i,%i\n", (int)node.origin.x, (int)node.origin.y);
        }
    }

    if (data_size != NULL)
    {
        *data_size = g.text_buffer.size();
    }

    return g.text_buffer.c_str();
}

void LoadCurrentEditorStateFromMemory(const char* data, size_t data_size)
{
    LoadEditorStateFromMemory(&editor_context_get(), data, data_size);
}

void LoadEditorStateFromMemory(
    EditorContext* editor_ptr,
    const char* data,
    size_t data_size)
{
    if (data_size == 0u)
    {
        return;
    }

    EditorContext& editor =
        editor_ptr == NULL ? editor_context_get() : *editor_ptr;

    char* buf = (char*)ImGui::MemAlloc(data_size + 1);
    const char* buf_end = buf + data_size;
    memcpy(buf, data, data_size);
    buf[data_size] = 0;

    void (*line_handler)(EditorContext&, const char*);
    line_handler = NULL;
    char* line_end = NULL;
    for (char* line = buf; line < buf_end; line = line_end + 1)
    {
        while (*line == '\n' || *line == '\r')
        {
            line++;
        }
        line_end = line;
        while (line_end < buf_end && *line_end != '\n' && *line_end != '\r')
        {
            line_end++;
        }
        line_end[0] = 0;

        if (*line == ';' || *line == '\0')
        {
            continue;
        }

        if (line[0] == '[' && line_end[-1] == ']')
        {
            line_end[-1] = 0;
            if (strncmp(line + 1, "node", 4) == 0)
            {
                line_handler = node_line_handler;
            }
            else if (strcmp(line + 1, "editor") == 0)
            {
                line_handler = editor_line_handler;
            }
        }

        if (line_handler != NULL)
        {
            line_handler(editor, line);
        }
    }
    ImGui::MemFree(buf);
}

void SaveCurrentEditorStateToDisk(const char* file_name)
{
    SaveEditorStateToDisk(&editor_context_get(), file_name);
}

void SaveEditorStateToDisk(const EditorContext* editor, const char* file_name)
{
    size_t data_size = 0u;
    const char* data = SaveEditorStateToMemory(editor, &data_size);
    FILE* file = ImFileOpen(file_name, "wt");
    if (!file)
    {
        return;
    }

    fwrite(data, sizeof(char), data_size, file);
    fclose(file);
}

void LoadCurrentEditorStateFromDisk(const char* file_name)
{
    LoadEditorStateFromDisk(&editor_context_get(), file_name);
}

void LoadEditorStateFromDisk(EditorContext* editor, const char* file_name)
{
    size_t data_size = 0u;
    char* file_data = (char*)ImFileLoadToMemory(file_name, "rb", &data_size);

    if (!file_data)
    {
        return;
    }

    LoadEditorStateFromMemory(editor, file_data, data_size);
    ImGui::MemFree(file_data);
}
} // namespace imnodes
