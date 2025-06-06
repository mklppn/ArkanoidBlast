#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../constants.h"
#include "entity.h"

// class that represent the background
class Background : public Entity
{
public:
    Background(const Texture& t, const V2f& p) : Entity(t, p, false) { ScaleTexture(); }

    void   Update() override {}
    string ToString() const override { return ""; }

    void   ScaleTexture() override
    {
        V2u texSize = _sprite->getTexture().getSize(); // Original image size
        V2f winSize = Config::Game::win;               // Current window size

        float scaleX = winSize.x / texSize.x;
        float scaleY = winSize.y / texSize.y;
        _sprite->setScale({ scaleX, scaleY });
    }
};

#endif // BACKGROUND_H
