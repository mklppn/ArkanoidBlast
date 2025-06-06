#include "entity.h"

// build an entity with a texture and center position
Entity::Entity(const Texture& t, const V2f& c, bool center)
{
    _sprite = make_unique<Sprite>(t); // initial sprite creation
    _sprite->setPosition(c);          // origin location in c
    if (center)                       // origin is the center of the sprite (left upper side otherwise)
        _sprite->setOrigin(_sprite->getGlobalBounds().size / 2.0f);
}

// set the texture of the entity   
void Entity::SetTexture(const Texture& t, const V2f& p)
{
    _sprite = make_unique<Sprite>(t);
    _sprite->setPosition(p);
    ScaleTexture();
}

// interaction with other entities, based on the distance between boxes
bool Entity::AreInteracting(const Entity& e1, const Entity& e2, double dTol)
{
    auto b1 = e1.GetBBox();
    auto b2 = e2.GetBBox();
    //return b1.findIntersection(b2) != std::nullopt;

    const V2f& p1 = b1.position, s1 = b1.size;
    const V2f& p2 = b2.position, s2 = b2.size;
    double xy1[4] = { p1.x, p1.y, p1.x + s1.x, p1.y + s1.y }; // xmin, ymin, xmax, ymax
    double xy2[4] = { p2.x, p2.y, p2.x + s2.x, p2.y + s2.y };

    // 1  2  3
    // 4  b1 6
    // 7  8  9
    double d2 = 0, d;
    if      (xy1[2] < xy2[0]) { d = xy1[2] - xy2[0], d2 += d * d; } // zones 3, 6, 9
    else if (xy2[2] < xy1[0]) { d = xy2[2] - xy1[0], d2 += d * d; } // zones 1, 4, 7

    if      (xy1[3] < xy2[1]) { d = xy1[3] - xy2[1], d2 += d * d; } // zones 1, 2, 3
    else if (xy2[3] < xy1[1]) { d = xy2[3] - xy1[1], d2 += d * d; } // zones 7, 8, 9

    bool res2 = d2 <= dTol * dTol;
    return d2 <= dTol * dTol; // if the distance is less than the tolerance, they are interacting
}
