#include "ball.h"

// set the texture of the entity   
void Ball::ScaleTexture()
{
    V2u   texSize = _sprite->getTexture().getSize();
    float s       = 2*_r / min(texSize.x, texSize.y);
    _sprite->setScale({ s, s });
}

void Ball::Update()
{
  // compute the ball's new position: move the position of the ball
  _sprite->move(_velocity);

  // We check if the ball has moved off the left hand side of the window
  // If so, we change sign of the x-component of the velocity
  // This will make it move at the same speed, but to the right
  // The ball will appear to bounce back into the window
  // And similarly for the right hand side of the screen
  const V2f& p = _sprite->getPosition();
  if      (p.x > Config::Game::win.x) _velocity.x = -_velocity.x;
  else if (p.x < 0                  ) _velocity.x = -_velocity.x;

  // We can also do this for the top and bottom of the screen
  if      (p.y > Config::Game::win.y) Destroy();
  else if (p.y < 0                  ) _velocity.y = -_velocity.y;
}

string Ball::ToString() const
{
    const V2f& p = _sprite->getPosition();
    const V2f& v = _velocity;
    string str = "p=(" + to_string(p.x) + ", " + to_string(p.y) + ")"
              + " v=(" + to_string(v.x) + ", " + to_string(v.y) + ")";

    if (_velocity.x != 0 || _velocity.y != 0) str += ", " + to_string(_velocity.angle().asDegrees()) + "°";
 
    str += ")";

    return str;
}