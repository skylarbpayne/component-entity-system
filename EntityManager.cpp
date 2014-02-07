/**
    * EntityManager handles the adding and removing of entities and any other higher level entity management. Has write access
    * to EntityAccessor. Implementation of class declared in EntityManager.h
    *
    * Author: Skylar Payne
    * Date: 7/16/2013
    * File: EntityManager.cpp
**/

#include "EntityManager.h"
#include "Entity.h"
#include "IListener.h"

std::vector<Entity*> EntityAccessor::s_Entities;

/**
 * @brief EntityManager::~EntityManager removes all entities.
 */
EntityManager::~EntityManager()
{
    for(unsigned int i = 0; i < EntityAccessor::s_Entities.size(); i++)
    {
        if(EntityAccessor::s_Entities[i])
        {
            delete EntityAccessor::s_Entities[i];
            EntityAccessor::s_Entities[i] = nullptr;
        }
    }

    EntityAccessor::s_Entities.clear();
}

/**
 * @brief EntityManager::AddEntity Adds an entity to the entity container
 * @param e the entity to add
 * @return true if entity was added, false otherwise
 */
bool EntityManager::AddEntity(Entity *e)
{
    if(e == nullptr)
    {
        return false;
    }

    if(_EntitySpaces.empty())
    {
        e->_ID = EntityAccessor::s_Entities.size();
        EntityAccessor::s_Entities.push_back(e);

        EntityMessage msg;
        msg.ID = e->_ID;
        msg.Destroyed = false;
        Emit<EntityMessage>(msg);

        return true;
    }

    std::list<unsigned int>::iterator it;
    for(it = _EntitySpaces.begin(); it != _EntitySpaces.end(); it++)
    {
        if(EntityAccessor::s_Entities[*it] == nullptr)
        {
            e->_ID = *it;
            EntityAccessor::s_Entities[*it] = e;

            EntityMessage msg;
            msg.ID = e->_ID;
            msg.Destroyed = false;
            Emit<EntityMessage>(msg);

            _EntitySpaces.erase(it);

            return true;
        }

        else
        {
            _EntitySpaces.erase(it);
        }
    }

    return false;
}

/**
 * @brief EntityManager::RemoveEntity removes an entity from the container
 * @param ID The ID of the entity to remove
 */
void EntityManager::RemoveEntity(unsigned int ID)
{
    if(ID > EntityAccessor::s_Entities.size())
    {
        return;
    }

    if(EntityAccessor::s_Entities[ID] != nullptr)
    {
        EntityMessage msg;
        msg.ID = ID;
        msg.Destroyed = true;
        Emit<EntityMessage>(msg);

        delete EntityAccessor::s_Entities[ID];
        EntityAccessor::s_Entities[ID] = nullptr;
        _EntitySpaces.push_back(ID);
    }
}

/**
 * @brief EntityManager::Update removes all entities at a time where nothing should be iterating through them.
 */
void EntityManager::Update()
{
    std::list<unsigned int>::iterator it;

    for(it = _EntitiesToRemove.begin(); it != _EntitiesToRemove.end(); it++)
    {
        RemoveEntity(*it);
    }
    _EntitiesToRemove.clear();
}

/**
 * @brief EntityManager::OnMessage calls AddEntity
 * @param msg contains the entity to add
 */
void EntityManager::OnMessage(AddEntityMessage& msg)
{
    this->AddEntity(msg.entity);
}

/**
 * @brief EntityManager::OnMessage puts the entity ID into a list to be destroyed next frame
 * @param msg holds the ID of the entity to destroy
 */
void EntityManager::OnMessage(DestroyEntityMessage& msg)
{
    _EntitiesToRemove.push_back(msg.ID);
}
