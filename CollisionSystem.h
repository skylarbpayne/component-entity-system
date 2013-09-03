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
#include "EntityBin.h"

class CollisionSystem : public ISystem, public IListener<EntityMovedMessage>
{
private:
    sf::Vector2f _MapSize;
    sf::Vector2f _BinSize;
    unsigned int _NumBinsX;
    unsigned int _NumBins;
    EntityBin* _Bins;
    std::list<unsigned int> _MovedEntities;
private:
    void OnMessage(EntityMovedMessage& msg) override;
    bool CheckCollision(Entity* e1, Entity* e2, sf::Vector2f& norm);
    bool CheckCollisions(unsigned int id, sf::Vector2f& norm);
    bool GetBins(Entity* e, std::set<unsigned int>& bins);
    void FillBins();
    void ClearBins();
public:
    CollisionSystem(sf::Vector2f mapSize, sf::Vector2f binSize);

    void Update(sf::Time dt) override;
    bool ValidateEntity(unsigned int ID) override;
};
