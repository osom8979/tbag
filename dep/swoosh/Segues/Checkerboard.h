﻿#pragma once
#include <Swoosh/Segue.h>
#include <Swoosh/Ease.h>
#include <Swoosh/EmbedGLSL.h>
#include <Swoosh/Shaders.h>

using namespace swoosh;

template<int cols, int rows>
class CheckerboardCustom : public Segue {
private:
  glsl::Checkerboard shader;

public:
  virtual void onDraw(sf::RenderTexture& surface) {
    double elapsed = getElapsed().asMilliseconds();
    double duration = getDuration().asMilliseconds();
    double alpha = ease::linear(elapsed, duration, 1.0);

    this->drawLastActivity(surface);

    surface.display(); // flip and ready the buffer

    sf::Texture temp(surface.getTexture()); // Make a copy of the source texture

    sf::Sprite sprite(temp);

    surface.clear(sf::Color::Transparent);
    this->drawNextActivity(surface);

    surface.display(); // flip and ready the buffer

    sf::Texture temp2(surface.getTexture()); // Make a copy of the source texture

    shader.setAlpha((float)alpha);
    shader.setTexture1(&temp);
    shader.setTexture2(&temp2);
    shader.apply(surface);
  }

  CheckerboardCustom(sf::Time duration, Activity* last, Activity* next) : Segue(duration, last, next),
    shader(cols, rows) {

    shader.setSmoothness(0.005f);
  }

  virtual ~CheckerboardCustom() { ; }
};

using Checkerboard = CheckerboardCustom<10, 10>;
