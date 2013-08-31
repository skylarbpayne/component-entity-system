/**
    * The BehaviorSystem manages the updating of and facilitation of messages to behaviors.
    *
    * Author: Skylar Payne
    * Date: 7/28/2013
    * File: BehaviorSystem.h
**/

#pragma once

#include "IBehavior.h"
#include "ISystem.h"
#include "IListener.h"
#include "LuaBindings.h"

class BehaviorSystem : public ISystem, public IListener<CollisionMessage>
{
private:
    void OnMessage(CollisionMessage& msg) override;
public:
    BehaviorSystem();
    ~BehaviorSystem();

    void Update(sf::Time dt) override;
    bool ValidateEntity(unsigned int ID) override;
};
