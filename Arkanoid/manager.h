#ifndef MANAGER_H
#define MANAGER_H

#include <typeinfo>
#include <vector>
#include <map>

#include "constants.h"
#include "Entities/background.h"
#include "Entities/ball.h"
#include "Entities/brick.h"
#include "Entities/paddle.h"

using namespace std;

// class that manages the entities in the game

using Entities = vector<unique_ptr<class Entity>>;

class EntityManager
{
    map<size_t, Entities> GroupedEntities; // stored entities of the game (bricks, background, ball, paddle...)

public:
    void Clear () { GroupedEntities.clear(); } // destroy all entities
    void Prune ();                             // scan all entities and clean up the destroyed ones    
    void Update();                             // update all the entities
    void Draw(RenderWindow& window);           // draw entities

    // entity factory using a variadic template to pass any number of arguments
    template <typename T, typename... Args>
    void create(Args&&... args)
    {
        // check that T derives from Entity
        static_assert(is_base_of<Entity, T>::value, R"("T" type parameter in create() must be derived from "entity")");

        auto ptr{ make_unique<T>(forward<Args>(args)...) }; // create a unique_ptr to the entity (forward arguments to the ctor)
        auto hash = typeid(T).hash_code();                   // get the hash code for the entity object's type

        GroupedEntities[hash].emplace_back(std::move(ptr));  // insert entity into the map
    }
    
    // retrieve all the objects of a given type
    template<typename T> Entities& GetAll()
    {
        return GroupedEntities[typeid(T).hash_code()];
    }

    // get first object of a given type
    template<typename T> T* GetFirst()
    {
        const Entities& entities = GetAll<T>();
        return entities.size() > 0 ? (T*)entities[0].get() : nullptr;
    }

    // apply a function to all entities of a given type
    template<typename T, typename Func> void ForAll(const Func& func)
    {
        for (const auto& ptr : GetAll<T>()) func(*dynamic_cast<T*>(ptr.get()));
    }
};

#endif // MANAGER_H
