/**
 * @file   RayGuiNodes.cpp
 * @brief  RayGuiNodes class implementation.
 * @author zer0
 * @date   2019-06-02
 */

#include <libtbag/ray/RayGuiNodes.hpp>
#include <libtbag/ray/RayBypass.hpp>

#include <imgui.h>
#include <ImNodes.h>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

struct MyNode
{
    ImNodes::NodeInfo node_info{};

    explicit MyNode(const char* title, const std::vector<ImNodes::SlotInfo>& inputs, const std::vector<ImNodes::SlotInfo>& outputs)
    {
        node_info.title = title;
        node_info.userdata = this;
        this->inputs = inputs;
        this->outputs = outputs;
    }

    MyNode(const MyNode& other) = delete;

    std::vector<ImNodes::SlotInfo> inputs;
    std::vector<ImNodes::SlotInfo> outputs;
    std::vector<ImNodes::Connection> connections;
};

std::vector<MyNode*> available_nodes{
        new MyNode("Node 0",
                   {
                           // Inputs
                           {
                                   "Input 0",
                           },
                           {
                                   "Input longer",
                           }
                   },
                   {
                           // Outputs
                           {
                                   "Output 0",
                           },
                           {
                                   "Output longer",
                           }
                   }),
        new MyNode("Node 1",
                   {
                           // Inputs
                           {
                                   "Input 0",
                           }
                   },
                   {
                           // Outputs
                           {
                                   "Output longer",
                           }
                   })
};
std::vector<MyNode*> nodes;

void DeleteConnection(ImNodes::Connection* connection, std::vector<ImNodes::Connection>& connections)
{
    for (auto it = connections.begin(); it != connections.end(); ++it)
    {
        if (*connection == *it)
        {
            connections.erase(it);
            break;
        }
    }
}

void GuiShowDemoNodes(bool * p_open)
{
    static ImNodes::CanvasState canvas{};
    if (ImGui::Begin("ImNodes"))
    {
        // We probably need to keep some state, like positions of nodes/slots for rendering connections.
        ImNodes::BeginCanvas(&canvas);
        for (auto it = nodes.begin(); it != nodes.end();)
        {
            MyNode* node = *it;
            if (ImNodes::BeginNode(&node->node_info,
                                   &node->inputs[0], node->inputs.size(),
                                   &node->outputs[0], node->outputs.size(),
                                   &node->connections[0], node->connections.size()))
            {
                // Custom widgets can be rendered in the middle of node
                ImGui::TextUnformatted("node content");

                // Store new connections
                ImNodes::Connection new_connection;
                if (ImNodes::GetNewConnection(&new_connection))
                {
                    ((MyNode*) new_connection.input_node->userdata)->connections.push_back(new_connection);
                    ((MyNode*) new_connection.output_node->userdata)->connections.push_back(new_connection);
                }

                // Remove deleted connections
                if (ImNodes::Connection* connection = ImNodes::GetDeleteConnection())
                {
                    DeleteConnection(connection, ((MyNode*) connection->input_node->userdata)->connections);
                    DeleteConnection(connection, ((MyNode*) connection->output_node->userdata)->connections);
                }

                ImNodes::EndNode();
            }

            if (node->node_info.selected && ImGui::IsKeyPressedMap(ImGuiKey_Delete))
            {
                for (auto& connection : node->connections)
                {
                    DeleteConnection(&connection, ((MyNode*) connection.input_node->userdata)->connections);
                    DeleteConnection(&connection, ((MyNode*) connection.output_node->userdata)->connections);
                }
                delete node;
                it = nodes.erase(it);
            }
            else
                ++it;
        }

        const ImGuiIO& io = ImGui::GetIO();
        if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
        {
            ImGui::FocusWindow(ImGui::GetCurrentWindow());
            ImGui::OpenPopup("NodesContextMenu");
        }

        if (ImGui::BeginPopup("NodesContextMenu"))
        {
            for (const auto& node_template : available_nodes)
            {
                if (ImGui::MenuItem(node_template->node_info.title))
                {
                    nodes.push_back(new MyNode(node_template->node_info.title, node_template->inputs,
                                               node_template->outputs));
                }
            }
            if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered())
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImNodes::EndCanvas();
    }
    ImGui::End();
}


} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

