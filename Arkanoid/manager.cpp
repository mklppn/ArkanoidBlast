#include <algorithm>
#include <iostream>
#include "manager.h"
#include "interactions.h"

// prune all destroyed entities
void EntityManager::Prune()
{
	for (auto& [type, entities] : GroupedEntities)
	{
		// all the elements for which are destroyed are moved to the back AND an iterator to the first one is returned
        // then erase frome the first moved element to the end
		entities.erase(
			remove_if(begin(entities), end(entities), [](const auto& p) { return p->IsDestroyed(); }),
			end(entities)
		);
	}
}

// update all the entities
void EntityManager::Update()
{
	for (const auto& pair : GroupedEntities)
		for (auto& e : pair.second) e->Update();

	ForAll<Ball>([this](auto& ball)			 // ball interactions
	{
		ForAll<Brick>([&ball](auto& brick)   // ball/bricks interactions
		{
			Interactor::Collide(ball, brick);
		});
		ForAll<Paddle>([&ball](auto& paddle) // ball/paddle interaction
		{
			Interactor::Collide(ball, paddle);
		});
	});
}

template<typename... Types, typename F> void ForTypes(F&& f)
{
	(f.template operator() < Types > (), ...);  // Fold over types
}

void EntityManager::Draw(RenderWindow& window)
{
	//for (const auto& e : GetAll<Background>()) e->Draw(window);
	//for (const auto& e : GetAll<Ball>      ()) e->Draw(window);
	//for (const auto& e : GetAll<Paddle>    ()) e->Draw(window);
	//for (const auto& e : GetAll<Brick>     ()) e->Draw(window);
	ForTypes<Background, Ball, Paddle, Brick>([&]<typename T>()
	{
		for (const auto& e : GetAll<T>()) e->Draw(window);
	});
}
