/**
    * The CollisionSystem tests for collisions between entities with a Collider component
    *
    * Author: Skylar Payne
    * Date: 8/5/2013
    * File: CollisionSystem.h
**/

#pragma once

#include <SFML/Graphics.hpp>
#include "IListener.h"
#include "ISystem.h"

class CollisionSystem : public ISystem, public IListener<EntityMovedMessage>
{
private:
    std::list<unsigned int> _MovedEntities;
private:
    void OnMessage(EntityMovedMessage& msg) override;
    bool CheckCollision(Entity* e1, Entity* e2, sf::Vector2f& norm);
    bool CheckCollisions(unsigned int id, sf::Vector2f& norm);
public:
    CollisionSystem() : ISystem("Collision") { }

    void Update(sf::Time dt) override;
    bool ValidateEntity(unsigned int ID) override;
};
