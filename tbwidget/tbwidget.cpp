/**
 * @file   tbwidget.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-10-17
 */

#include <libtbag/predef.hpp>
#include <iostream>

static char const * const WINDOW_TITLE = "TBAG WIDGET with SFML";

static int const WINDOW_WIDTH  = 200;
static int const WINDOW_HEIGHT = 200;

#if defined(USE_SFML)
#include <SFML/Graphics.hpp>
int runSfmlSample(int argc, char ** argv)
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
#else
int runSfmlSample(int argc, char ** argv)
{
    return 1;
}
#endif // defined(USE_SFML)

inline static TBAG_CONSTEXPR bool isUseSfml() TBAG_NOEXCEPT
{
#if defined(USE_SFML)
    return true;
#else
    return false;
#endif // defined(USE_SFML)
}

int main(int argc, char ** argv)
{
    if (isUseSfml()) {
        return runSfmlSample(argc, argv);
    } else {
        std::cout << "Not found SFML!\n";
        return 0;
    }
}

