#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include "Entities/ball.h"
#include "Entities/brick.h"
#include "Entities/paddle.h"

class Interactor
{
public:
    static void Collide(Ball& b, const Paddle& p); // solve potential collision between a ball and a paddle
    static void Collide(Ball& b, Brick& p);        // solve potential collision between a ball and a brick
};

#endif //INTERACTIONS_H
