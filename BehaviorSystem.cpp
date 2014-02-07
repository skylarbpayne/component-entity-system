/**
    * Definition of the BehaviorSystem class as defined in BehaviorSystem.h
    *
    * Author: Skylar Payne
    * Date: 7/28/2013
    * File: BehaviorSystem.cpp
**/

#include "BehaviorSystem.h"
#include "Entity.h"

#include <lua.hpp>
#include "LuaBindings.h"
#include "ScriptableBehavior.h"

/**
 * @brief BehaviorSystem::BehaviorSystem sets up a lua thread for scriptable behaviors to use.
 */
BehaviorSystem::BehaviorSystem() : ISystem("Behavior")
{
    ScriptableBehavior::s_L = luaL_newstate();
    SetBindings(ScriptableBehavior::s_L);
}

/**
 * @brief BehaviorSystem::~BehaviorSystem closes the lua thread for scriptable behaviors
 */
BehaviorSystem::~BehaviorSystem()
{
    lua_close(ScriptableBehavior::s_L);
}

/**
 * @brief BehaviorSystem::Update updates all behaviors of all entities
 * @param dt time since last frame
 */
void BehaviorSystem::Update(sf::Time dt)
{
    behaviorMap::iterator bit;

    for(unsigned int i = 0; i < this->GetEntities().size(); i++)
    {
        if(!this->GetEntities()[i])
        {
            continue;
        }

        for(bit = this->GetEntities()[i]->_Behaviors.begin(); bit != this->GetEntities()[i]->_Behaviors.end(); bit++)
        {
            (bit->second)->Update();
        }
    }
}

/**
 * @brief BehaviorSystem::OnMessage called when a CollisionMessage is emitted--Calls OnCollide method of appropriate behaviors
 * @param msg the collision data
 */
void BehaviorSystem::OnMessage(CollisionMessage& msg)
{
    Entity* e1 = this->GetEntity(msg.ID1);
    Entity* e2 = this->GetEntity(msg.ID2);

    behaviorMap::iterator it;

    for(it = e1->_Behaviors.begin(); it != e1->_Behaviors.end(); it++)
    {
        (it->second)->OnCollide(msg.ID2, msg.norm);
    }

    for(it = e2->_Behaviors.begin(); it != e2->_Behaviors.end(); it++)
    {
        (it->second)->OnCollide(msg.ID1, msg.norm);
    }
}

/**
 * @brief BehaviorSystem::ValidateEntity checks to see if any behaviors are attached to the entity
 * @param ID the entity to check
 * @return true if the entity has any behaviors, false otherwise
 */
bool BehaviorSystem::ValidateEntity(unsigned int ID)
{
    Entity* e = this->GetEntity(ID);
    return e->_Behaviors.size() > 0;
}
