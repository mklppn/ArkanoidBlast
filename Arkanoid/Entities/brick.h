#ifndef BRICK_H
#define BRICK_H

#include "../constants.h"
#include "entity.h"

// class thatrepresent a brick
// A grid of bricks is displayed across the top of the screen
// This grid will be represented by an std::vector of brick objects
// When the ball hits a brick, the brick is weakened
// If a brick is hit enough times, it is destroyed and disappears
// When a brick is destroyed, it is deleted from the vector
class Brick: public Entity
{
	// How many times the brick can be hit before destroying it
	int _strength = Config::Brick::Strength;
    V2f _size     = Config::Brick::Size;

public:
	Brick(const Texture& t, const V2f& p) : Entity(t, p)
	{
        ScaleTexture();
	}

	void Weaken() noexcept;       // decrease the brick's strength
	void Update      () override; // compute the brick's new appearance
	void ScaleTexture() override; // set the texture of the entity   

	string ToString() const override;
};

#endif // BRICK_H