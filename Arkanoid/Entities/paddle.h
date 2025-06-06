#ifndef PADDLE_H
#define PADDLE_H

#include "../constants.h"
#include "entity.h"

// class that represent a paddle that moves across the bottom of the screen
// under the user's control. When the ball hits the paddle, it bounces and
// change its direction
class Paddle : public MovingEntity
{
    V2f _size = Config::Paddle::Size;

    void ComputeVelocity();       // compute velocities from user inputs

public:
    Paddle(const Texture& t, const V2f& p) : MovingEntity(t, p, { 0.0f, 0.0f })
    {
        ScaleTexture();
    } 

    void Update()       override; // compute paddle velocity and move it
    void ScaleTexture() override; // set the texture of the entity   

    string ToString() const override;
};

#endif // PADDLE_H
