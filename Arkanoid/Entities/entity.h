#ifndef ENTITY_H
#define ENTITY_H

#include "../constants.h"

using namespace sf;
using namespace std;

// abstract class that represent any graphical entity
class Entity
{
private:
    bool               _destroyed{ false };    // entity's status

protected:
    unique_ptr<Sprite> _sprite;

    Entity()                         = delete; // explicitly delete the default constructor to avoid the error
    Entity(const Entity&)            = delete; // explicitly delete the copy constructor to avoid the error
    Entity& operator=(const Entity&) = delete; // explicitly delete the copy assignment operator to avoid the error

public:
    Entity(const Texture& t, const V2f& p, bool center = true); // build an entity with a texture and the position of the initial center
    virtual ~Entity() { }                                       // virtual destructor

    virtual void   Update  ()       = 0;                        // update the entity (compute position, appearance, and so on)
    virtual string ToString() const = 0;                        // build a debug string
    virtual void   ScaleTexture() {};                           // scale the texture of the entity

    void SetTexture(const Texture& t, const V2f& p); // set the texture of the entity  

    // inline methods
    inline FloatRect GetBBox()     const noexcept { return _sprite->getGlobalBounds(); };
    inline float     x()           const noexcept { return _sprite->getPosition().x;}; // center of the box
    inline float     y()           const noexcept { return _sprite->getPosition().y;}; // center of the box
    inline void      Destroy()           noexcept { _destroyed = true; }
    inline bool      IsDestroyed() const noexcept { return _destroyed; }
    inline void      Draw(RenderWindow& window) const { window.draw(*_sprite); };

    // edges of the sprite
    inline float left  ()    const noexcept { auto b = GetBBox(); return b.position.x           ; }
    inline float right ()    const noexcept { auto b = GetBBox(); return b.position.x + b.size.x; }
    inline float top   ()    const noexcept { auto b = GetBBox(); return b.position.y           ; }
    inline float bottom()    const noexcept { auto b = GetBBox(); return b.position.y + b.size.y; }
    inline V2f   GetCenter() const noexcept { auto b = GetBBox(); return b.position + b.size / 2.0f; }

    // interaction with other entities, based on the distance between boxes
    static bool AreInteracting(const Entity& e1, const Entity& e2, double dTol = 0);
};

// abstract class that represent moveable graphical entities
class MovingEntity : public Entity
{
protected:
    V2f _velocity{ 0.0, 0.0 }; // displacement between screen updates

public:
    MovingEntity(const Texture& t, const V2f& p, const V2f& v) : Entity(t, p), _velocity(v) {}

    void       SetVelocity(const V2f& v) { _velocity = v; }
    const V2f& GetVelocity() const       { return _velocity; }
};

#endif // ENTITY_H
