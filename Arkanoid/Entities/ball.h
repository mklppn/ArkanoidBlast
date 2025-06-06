#ifndef BALL_H
#define BALL_H

#include "../constants.h"
#include "entity.h"

// class that represent the bouncing ball, represented by its, position, velocity
// and smallest radius (ball may not be a circle). Assume that the min box of its texture is (Oxy) aligned. 
class Ball : public MovingEntity
{
    float _r     = Config::Ball::Radius; // radius of the ball
    bool  _glued = true;                 // true if the ball is glued to the paddle

public:
    Ball(const Texture& t, const V2f& p, const V2f& v) : MovingEntity(t, p, v)
    {
        ScaleTexture();
    }

    inline bool IsGlued() const      { return _glued; }
    inline void SetGlued(bool glued) { _glued = glued; }

    void Update      () override; // compute the ball's new position
    void ScaleTexture() override; // set the texture of the entity   

    string ToString() const override;
};

#endif // BALL_H
