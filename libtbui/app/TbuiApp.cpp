/**
 * @file   TbuiApp.cpp
 * @brief  TbuiApp class implementation.
 * @author zer0
 * @date   2018-10-30
 */

#include <libtbui/app/TbuiApp.hpp>
#include <libtbui/util/TbuiVersion.hpp>

#include <libtbui/3rd/imgui/imgui.h>
#include <libtbui/3rd/imgui/imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace libtbui {
namespace app     {

TbuiApp::TbuiApp(int argc, char ** argv, char ** envs, bool init, char const * name)
        : Base(argc, argv, envs, init, name)
{
    // EMPTY.
}

TbuiApp::~TbuiApp()
{
    // EMPTY.
}

bool TbuiApp::onCreate()
{
    return Base::onCreate();
}

void TbuiApp::onDestroy()
{
    Base::onDestroy();
}

TbuiApp::Version TbuiApp::getVersion()
{
    return libtbui::util::getVersion();
}

int TbuiApp::onMain(Commands const & commands)
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}

} // namespace app
} // namespace libtbui

