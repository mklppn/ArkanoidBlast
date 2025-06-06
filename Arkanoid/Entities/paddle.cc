#include "paddle.h"

using namespace sf::Keyboard;

// set the texture of the entity   
void Paddle::ScaleTexture()
{
    V2u texSize = _sprite->getTexture().getSize();
    _sprite->setScale({ _size.x / texSize.x, _size.y / texSize.y });
}

// Compute the paddle's new position
void Paddle::Update()
{
    ComputeVelocity();        // compute the velocity based on user inputs
    _sprite->move(_velocity); // Move the position of the paddle
}

void Paddle::ComputeVelocity()
{
    // - left/right arrow key pressed => move to the left/right => -/+ velocity
    // - do not move off the screen
    _velocity.x = 0;
    if      (isKeyPressed(Key::Left ) && left()  > 0                  ) _velocity.x = -Config::Paddle::Speed;
    else if (isKeyPressed(Key::Right) && right() < Config::Game::win.x) _velocity.x =  Config::Paddle::Speed;
}

string Paddle::ToString() const
{
    const V2f& p = _sprite->getPosition();
    const V2f& v = _velocity;
    return "p=(" + to_string(p.x) + ", " + to_string(p.y) + ")"
        + " v=(" + to_string(v.x) + ", " + to_string(v.y) + ")";
}