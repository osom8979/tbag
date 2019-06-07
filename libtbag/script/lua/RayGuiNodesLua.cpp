/**
 * @file   RayGuiNodesLua.cpp
 * @brief  RayGuiNodesLua class implementation.
 * @author zer0
 * @date   2019-06-07
 */

#include <libtbag/script/lua/RayGuiNodesLua.hpp>
#include <libtbag/script/lua/RayLua.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/ray/RayGuiNodes.hpp>

#include <cassert>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::ray;

TBAG_CONSTEXPR static char const * const METATABLE_GUINODES = "GuiNodes";

struct GuiNodes
{
    RayGuiNodes * nodes;
};

static GuiNodes * luaE_pushguinodes(lua_State * L, GuiNodes const * src = nullptr)
{
    auto * result = (GuiNodes*)lua_newuserdata(L, sizeof(GuiNodes));
    assert(result != nullptr);
    if (src) {
        memcpy((void*)result, (void const *)src, sizeof(GuiNodes));
    } else {
        try {
            result->nodes = new RayGuiNodes();
        } catch (...) {
            result->nodes = nullptr;
        }
    }
    luaL_getmetatable(L, METATABLE_GUINODES);
    lua_setmetatable(L, -2);
    return result;
}

static GuiNodes * luaE_checkguinodes(lua_State * L, int num_arg)
{
    auto * result = (GuiNodes*)luaL_checkudata(L, num_arg, METATABLE_GUINODES);
    if (result == nullptr) {
        luaL_typerror(L, num_arg, METATABLE_GUINODES);
        return nullptr;
    }
    return result;
}

static GuiNodes * luaE_optguinodes(lua_State * L, int num_arg, GuiNodes * def)
{
    if (lua_isuserdata(L, num_arg)) {
        return luaE_checkguinodes(L, num_arg);
    }
    return def;
}

static int _GuiNodes(lua_State * L)
{
    luaE_pushguinodes(L);
    return 1;
}

static int _GuiNodes_gc(lua_State * L)
{
    auto * nodes = luaE_checkguinodes(L, 1);
    assert(nodes != nullptr);
    if (nodes->nodes) {
        delete nodes->nodes;
        nodes->nodes = nullptr;
    }
    return 0;
}

static int _GuiNodes_tostring(lua_State * L)
{
    lua_pushstring(L, METATABLE_GUINODES);
    return 1;
}

static luaL_Reg const __lua_reg_guinodes[] = {
        { "__gc", _GuiNodes_gc },
        { "__tostring", _GuiNodes_tostring },
        { nullptr, nullptr }
};

static void luaE_register_guinodes(lua_State * L)
{
    luaE_register_metatable(L, METATABLE_GUINODES, __lua_reg_guinodes);
}

static int _GuiNodesEditorContextSet(lua_State * L)
{
    auto * nodes = luaE_checkguinodes(L, 1);
    assert(nodes != nullptr);
    GuiNodesEditorContextSet(*(nodes->nodes));
    return 0;
}

static int _GuiNodesInitialize(lua_State * L)
{
    GuiNodesInitialize();
    return 0;
}

static int _GuiNodesShutdown(lua_State * L)
{
    GuiNodesShutdown();
    return 0;
}

static int _GuiNodesBeginNodeEditor(lua_State * L)
{
    GuiNodesBeginNodeEditor();
    return 0;
}

static int _GuiNodesEndNodeEditor(lua_State * L)
{
    GuiNodesEndNodeEditor();
    return 0;
}

static int _GuiNodesPushColorStyle(lua_State * L)
{
    GuiNodesPushColorStyle((GuiNodesColorStyle)luaL_checkinteger(L, 1),
                           luaL_checkinteger(L, 2));
    return 0;
}

static int _GuiNodesPopColorStyle(lua_State * L)
{
    GuiNodesPopColorStyle();
    return 0;
}

static int _GuiNodesBeginNode(lua_State * L)
{
    GuiNodesBeginNode(luaL_checkinteger(L, 1));
    return 0;
}

static int _GuiNodesEndNode(lua_State * L)
{
    GuiNodesEndNode();
    return 0;
}

static int _GuiNodesName(lua_State * L)
{
    GuiNodesName(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiNodesBeginInputAttribute(lua_State * L)
{
    GuiNodesBeginInputAttribute(luaL_checkinteger(L, 1));
    return 0;
}

static int _GuiNodesBeginOutputAttribute(lua_State * L)
{
    GuiNodesBeginOutputAttribute(luaL_checkinteger(L, 1));
    return 0;
}

static int _GuiNodesEndAttribute(lua_State * L)
{
    GuiNodesEndAttribute();
    return 0;
}

static int _GuiNodesLink(lua_State * L)
{
    GuiNodesLink(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
}

static int _GuiNodesSetNodePos(lua_State * L)
{
    GuiNodesSetNodePos(luaL_checkinteger(L, 1), luaE_checkvector2(L, 2));
    return 0;
}

static int _GuiNodesIsNodeHovered(lua_State * L)
{
    int node_id = luaL_checkinteger(L, 1);
    auto code = GuiNodesIsNodeHovered(&node_id);
    lua_pushboolean(L, code);
    lua_pushinteger(L, node_id);
    return 2;
}

static int _GuiNodesIsLinkHovered(lua_State * L)
{
    int link_id = luaL_checkinteger(L, 1);
    auto const code = GuiNodesIsLinkHovered(&link_id);
    lua_pushboolean(L, code);
    lua_pushinteger(L, link_id);
    return 2;
}

static int _GuiNodesIsPinHovered(lua_State * L)
{
    int attribute_id = luaL_checkinteger(L, 1);
    auto const code = GuiNodesIsPinHovered(&attribute_id);
    lua_pushboolean(L, code);
    lua_pushinteger(L, attribute_id);
    return 2;
}

static int _GuiNodesIsNodeSelected(lua_State * L)
{
    int node_id = luaL_checkinteger(L, 1);
    auto const code = GuiNodesIsNodeSelected(&node_id);
    lua_pushboolean(L, code);
    lua_pushinteger(L, node_id);
    return 2;
}

static int _GuiNodesIsLinkSelected(lua_State * L)
{
    int link_id = luaL_checkinteger(L, 1);
    auto const code = GuiNodesIsLinkSelected(&link_id);
    lua_pushboolean(L, code);
    lua_pushinteger(L, link_id);
    return 2;
}

static int _GuiNodesIsAttributeActive(lua_State * L)
{
    lua_pushboolean(L, GuiNodesIsAttributeActive());
    return 1;
}

static int _GuiNodesIsAnyAttributeActive(lua_State * L)
{
    int const top = lua_gettop(L);
    if (top == 0) {
        lua_pushboolean(L, GuiNodesIsAnyAttributeActive(nullptr));
        return 1;
    } else {
        int attribute_id = luaL_checkinteger(L, 1);
        auto const code = GuiNodesIsAnyAttributeActive(&attribute_id);
        lua_pushboolean(L, code);
        lua_pushinteger(L, attribute_id);
        return 2;
    }
}

static int _GuiNodesIsLinkStarted(lua_State * L)
{
    int started_at_attr = luaL_checkinteger(L, 1);
    auto const code = GuiNodesIsLinkStarted(&started_at_attr);
    lua_pushboolean(L, code);
    lua_pushinteger(L, started_at_attr);
    return 2;
}

static int _GuiNodesIsLinkDropped(lua_State * L)
{
    lua_pushboolean(L, GuiNodesIsLinkDropped());
    return 1;
}

static int _GuiNodesIsLinkCreated(lua_State * L)
{
    int started_at_attr = luaL_checkinteger(L, 1);
    int ended_at_attr = luaL_checkinteger(L, 2);
    auto const code = GuiNodesIsLinkCreated(&started_at_attr, &ended_at_attr);
    lua_pushboolean(L, code);
    lua_pushinteger(L, started_at_attr);
    lua_pushinteger(L, ended_at_attr);
    return 3;
}

static int _GuiNodesSaveCurrentEditorStateToMemory(lua_State * L)
{
    size_t data_size = luaL_checkinteger(L, 1);
    char const * mem = GuiNodesSaveCurrentEditorStateToMemory(&data_size);
    std::vector<lua_Integer> buffer(data_size);
    for (size_t i = 0; i < data_size; ++i) {
        buffer[i] = mem[i];
    }
    luaE_pushinteger_array(L, buffer.data(), buffer.size());
    return 1;
}

static int _GuiNodesSaveEditorStateToMemory(lua_State * L)
{
    auto * nodes = luaE_checkguinodes(L, 1);
    size_t data_size = luaL_checkinteger(L, 2);
    char const * mem = GuiNodesSaveEditorStateToMemory(*(nodes->nodes), &data_size);
    std::vector<lua_Integer> buffer(data_size);
    for (size_t i = 0; i < data_size; ++i) {
        buffer[i] = mem[i];
    }
    luaE_pushinteger_array(L, buffer.data(), buffer.size());
    return 1;
}

static int _GuiNodesLoadCurrentEditorStateFromMemory(lua_State * L)
{
    auto const data = luaE_checkinteger_array(L, 1);
    std::vector<char> const buffer(data.begin(), data.end());
    GuiNodesLoadCurrentEditorStateFromMemory(buffer.data(), buffer.size());
    return 0;
}

static int _GuiNodesLoadEditorStateFromMemory(lua_State * L)
{
    auto * nodes = luaE_checkguinodes(L, 1);
    auto const data = luaE_checkinteger_array(L, 2);
    std::vector<char> const buffer(data.begin(), data.end());
    GuiNodesLoadEditorStateFromMemory(*(nodes->nodes), buffer.data(), buffer.size());
    return 0;
}

static int _GuiNodesSaveCurrentEditorStateToDisk(lua_State * L)
{
    GuiNodesSaveCurrentEditorStateToDisk(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiNodesSaveEditorStateToDisk(lua_State * L)
{
    auto * nodes = luaE_checkguinodes(L, 1);
    GuiNodesSaveEditorStateToDisk(*(nodes->nodes), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiNodesLoadCurrentEditorStateFromDisk(lua_State * L)
{
    GuiNodesLoadCurrentEditorStateFromDisk(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiNodesLoadEditorStateFromDisk(lua_State * L)
{
    auto * nodes = luaE_checkguinodes(L, 1);
    GuiNodesLoadEditorStateFromDisk(*(nodes->nodes), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiNodesShowDemoWindow(lua_State * L)
{
    GuiNodesShowDemoWindow();
    return 0;
}

#ifndef RAYGUINODES_REGISTER
#define RAYGUINODES_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_layguinodes[] = {
        { METATABLE_GUINODES, _GuiNodes },
        RAYGUINODES_REGISTER(GuiNodesEditorContextSet),
        RAYGUINODES_REGISTER(GuiNodesInitialize),
        RAYGUINODES_REGISTER(GuiNodesShutdown),
        RAYGUINODES_REGISTER(GuiNodesBeginNodeEditor),
        RAYGUINODES_REGISTER(GuiNodesEndNodeEditor),
        RAYGUINODES_REGISTER(GuiNodesPushColorStyle),
        RAYGUINODES_REGISTER(GuiNodesPopColorStyle),
        RAYGUINODES_REGISTER(GuiNodesBeginNode),
        RAYGUINODES_REGISTER(GuiNodesEndNode),
        RAYGUINODES_REGISTER(GuiNodesName),
        RAYGUINODES_REGISTER(GuiNodesBeginInputAttribute),
        RAYGUINODES_REGISTER(GuiNodesBeginOutputAttribute),
        RAYGUINODES_REGISTER(GuiNodesEndAttribute),
        RAYGUINODES_REGISTER(GuiNodesLink),
        RAYGUINODES_REGISTER(GuiNodesSetNodePos),
        RAYGUINODES_REGISTER(GuiNodesIsNodeHovered),
        RAYGUINODES_REGISTER(GuiNodesIsLinkHovered),
        RAYGUINODES_REGISTER(GuiNodesIsPinHovered),
        RAYGUINODES_REGISTER(GuiNodesIsNodeSelected),
        RAYGUINODES_REGISTER(GuiNodesIsLinkSelected),
        RAYGUINODES_REGISTER(GuiNodesIsAttributeActive),
        RAYGUINODES_REGISTER(GuiNodesIsAnyAttributeActive),
        RAYGUINODES_REGISTER(GuiNodesIsLinkStarted),
        RAYGUINODES_REGISTER(GuiNodesIsLinkDropped),
        RAYGUINODES_REGISTER(GuiNodesIsLinkCreated),
        RAYGUINODES_REGISTER(GuiNodesSaveCurrentEditorStateToMemory),
        RAYGUINODES_REGISTER(GuiNodesSaveEditorStateToMemory),
        RAYGUINODES_REGISTER(GuiNodesLoadCurrentEditorStateFromMemory),
        RAYGUINODES_REGISTER(GuiNodesLoadEditorStateFromMemory),
        RAYGUINODES_REGISTER(GuiNodesSaveCurrentEditorStateToDisk),
        RAYGUINODES_REGISTER(GuiNodesSaveEditorStateToDisk),
        RAYGUINODES_REGISTER(GuiNodesLoadCurrentEditorStateFromDisk),
        RAYGUINODES_REGISTER(GuiNodesLoadEditorStateFromDisk),
        RAYGUINODES_REGISTER(GuiNodesShowDemoWindow),
        { nullptr, nullptr }
};

#ifndef RAYGUINODES_INT_SYMBOL
#define RAYGUINODES_INT_SYMBOL(L, s)     \
    do {                            \
        lua_pushinteger(L, s);      \
        lua_setfield(L, -2, #s);    \
    } while (false) /* -- END -- */
#endif

static void luaL_push_rayguinodes_symbols(lua_State * L)
{
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_NodeBackground);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_NodeBackgroundHovered);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_NodeBackgroundSelected);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_NodeOutline);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_TitleBar);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_TitleBarHovered);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_TitleBarSelected);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_Link);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_LinkHovered);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_LinkSelected);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_Pin);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_PinHovered);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_PinOutline);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_GridBackground);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_GridLine);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesColorStyle_Count);

    RAYGUINODES_INT_SYMBOL(L, GuiNodesFlags_None);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesFlags_NodeOutline);
    RAYGUINODES_INT_SYMBOL(L, GuiNodesFlags_PinOutline);
}

bool luaE_open_rayguinodes(lua_State * L)
{
    luaL_register(L, lua_tbag_name(), __lua_layguinodes);
    {
        luaL_push_rayguinodes_symbols(L);
        luaE_register_guinodes(L);
    }
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

