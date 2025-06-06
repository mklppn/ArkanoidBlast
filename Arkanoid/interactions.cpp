#include "interactions.h"

using namespace sf::Keyboard;

// resolve potential collision between the ball and the paddle
void Interactor::Collide(Ball& b, const Paddle& p)
{
    if (!Entity::AreInteracting(p, b)) return; // the ball and paddle are not interacting : do nothing
    
    V2f v = b.GetVelocity();           // get the current velocity of the ball

    if ( b.IsGlued())
    {
        v = p.GetVelocity();
        if (isKeyPressed(Key::Space))
        {
            v.y = Config::Ball::Speed; // set the vertical speed to the initial value
            b.SetGlued(false);         // release the ball if the space key is pressed
        }
    }
  
    if (!b.IsGlued())
    {
        constexpr bool specular = false;
        constexpr bool slipVelocity = true;
        float dx = b.x() - p.x();
        if (specular)
        {
            v.y = -v.y;                           // make the ball bounce upwards
            v.x = dx < 0 ? -abs(v.x) : abs(v.x);  // direction depend on the location of the collision
        }
        else
        {
            constexpr float maxAngle = 1.438f;    // 85° 1.31; // max deflection angle (in radians) <=> 75° 
            float angle = maxAngle * 2 * dx / p.GetBBox().size.x; // maxAngle * [-1, 1] 
            float speed = v.length();
            v.x =  speed * sin(angle);
            v.y = -speed * cos(angle);        
        }

        if (slipVelocity)
        {
            constexpr float k = 0.3f;                           // constant to adjust the ball's velocity after the collision (typically in [0.3;1]
            constexpr float maxSpeed = 2 * Config::Ball::Speed; // maximum speed of the ball
            v.x = clamp(v.x + p.GetVelocity().x * k, -maxSpeed, maxSpeed);
        }
    }

    b.SetVelocity(v); // set the new velocity of the ball
}

// resolve potential collision between the ball and a brick
void Interactor::Collide(Ball& b, Brick& block)
{
    if (!Entity::AreInteracting(block, b)) return; // the ball and brick are not interacting : do nothing

    block.Weaken();     // update the brick's strength

    // compute the side of impact
    //      __________________________
    //      |                         |
    //    __|_                       _|__
    //   /  | \                     / |  \
    //  |   |dR|                   |dL|   |
    //  |   |  |                   |  |   |
    //   \__|_/                     \_|__/
    //      |_________________________|

    float dL = abs(b.right () - block.left  ());
    float dR = abs(b.left  () - block.right ());
    float dT = abs(b.bottom() - block.top   ());
    float dB = abs(b.top   () - block.bottom());

    float minDx = dL < dR ? dL : dR; // left overlap smaller than the right : the ball hit the left side
    float minDy = dT < dB ? dT : dB; // top  overlap smaller than the bottom: the ball hit the top  side

    V2f v = b.GetVelocity();         // get the current velocity of the ball
    if (minDx < minDy) v.x = dL < dR ? -abs(v.x) : abs(v.x); // the ball hit the left/right side => change its horizontal direction
    else               v.y = dT < dB ? -abs(v.y) : abs(v.y); // the ball hit the top/bottom side => change its vertical   direction
    b.SetVelocity(v); // set the new velocity of the ball
}
