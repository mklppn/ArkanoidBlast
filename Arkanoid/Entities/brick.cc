#include "brick.h"

// Use different colours, depending on the strength of the brick
// Format: RGBA
constexpr Color brick_colours[] = {
    {0, 0, 0, 0},       // dummy
    {0, 255, 0, 80},    // Dull green
    {0, 255, 0, 170},   // Medium green
    {0, 255, 0, 255}    // Bright green
};

void Brick::Weaken() noexcept
{
    --_strength;
    if (_strength <= 0) Destroy();
}

// set the texture of the entity   
void Brick::ScaleTexture()
{
    V2u texSize = _sprite->getTexture().getSize();
    _sprite->setScale({ _size.x / texSize.x, _size.y / texSize.y });
}

void Brick::Update()
{  
  _sprite->setColor(brick_colours[_strength]); // set the colour depending on its strength
}

string Brick::ToString() const
{
    const V2f& p = _sprite->getPosition();
    return "p=(" + to_string(p.x) + ", " + to_string(p.y) + ")";
}