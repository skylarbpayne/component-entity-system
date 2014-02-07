/**
	* Implementation of the entity class declared in Entity.h
    *
	* Author: Skylar Payne
	* Date: 6/8/2013
	* File: Entity.cpp
**/

#include "Entity.h"
#include <cstring>

#include "Logger.h"

/**
 * @brief Entity::~Entity Deallocates all components and behaviors
 */
Entity::~Entity()
{
    componentMap::iterator cit;

    for(cit = _Components.begin(); cit != _Components.end(); cit++)
    {
        if(cit->second)
        {
            delete (cit->second);
            (cit->second) = nullptr;
        }
    }

    _Components.clear();

    behaviorMap::iterator bit;

    for(bit = _Behaviors.begin(); bit != _Behaviors.end(); bit++)
    {
        if(bit->second)
        {
            delete (bit->second);
            (bit->second) = nullptr;
        }
    }

    _Behaviors.clear();
}

/**
 * @brief Entity::FindComponent finds a specified component
 * @param type the type of component to look for
 * @return true if the component was found, false otherwise
 */
bool Entity::FindComponent(const char* type)
{
    return (_Components.find(type) != _Components.end());
}

/**
 * @brief Entity::FindBehavior finds a particular behavior
 * @param type the type of behavior to find
 * @return true if the behavior was found, false otherwise
 */
bool Entity::FindBehavior(const char* type)
{
    return (_Behaviors.find(type) != _Behaviors.end());
}

/**
    * @brief Entity::AttachComponent Attaches a component to the entity if a component of that type is not already attached
	* @param comp The component to attach
	* @return true if the new component was attached, false otherwise.
**/
bool Entity::AttachComponent(IComponent* comp)
{
    if(comp == nullptr)
	{
        g_Logger << __FILE__ << ": " << __LINE__ << "-Error: component pointer was nullptr\n";
		return false;
	}

    if(this->FindComponent(comp->GetType()))
	{
        g_Logger << __FILE__ << ": " << __LINE__ << "-Error: " << comp->GetType() << " already attached to Entity" << this->_ID << "\n";
		return false;
	}

    _Components[comp->GetType()] = comp;
    comp->_Parent = this;

    g_Logger << __FILE__ << ": " << __LINE__ << "-" << comp->GetType() << " was added to Entity " << this->_ID << "\n";
	return true;
}

/**
    * @brief Entity::RemoveComponent Removes a type component from an entity if that entity has that component
	* @param type The component type to remove.
**/
void Entity::RemoveComponent(const char* type)
{ 
    g_Logger << __FILE__ << ": " << __LINE__ << "-" << type << " was ";
    g_Logger << (_Components.erase(type) > 0 ? "removed from " : "not in ") << "Entity " << this->_ID << "\n";
}

/**
    * @brief Entity::HasComponent Finds a particular component
	* @param type The type of component to find
	* @return true if the component was found, false otherwise
**/
bool Entity::HasComponent(const char* type)
{
       return FindComponent(type);
}

/**
 * @brief Entity::AttachBehavior attaches a behavior to an entity
 * @param beh the behavior to attach
 * @return true if the behavior was attached, false otherwise
 */
bool Entity::AttachBehavior(IBehavior* beh)
{
    if(beh == nullptr)
    {
        g_Logger << __FILE__ << ": " << __LINE__ << "-Error: behavior pointer was nullptr\n";
        return false;
    }

    if(this->FindBehavior(beh->GetType()))
    {
        g_Logger << __FILE__ << ": " << __LINE__ << "-Error: " << beh->GetType() << " already attached to Entity" << this->_ID << "\n";
        return false;
    }

    _Behaviors[beh->GetType()] = beh;
    beh->_Parent = this;

    g_Logger << __FILE__ << ": " << __LINE__ << "-" << beh->GetType() << " was added to Entity " << this->_ID << "\n";
    return true;
}

/**
 * @brief Entity::RemoveBehavior removes a behavior
 * @param type the type of behavior to remove
 */
void Entity::RemoveBehavior(const char* type)
{
    g_Logger << __FILE__ << ": " << __LINE__ << "-" << type << " was ";
    g_Logger << (_Behaviors.erase(type) > 0 ? "removed from " : "not in ") << "Entity " << this->_ID << "\n";
}

/**
 * @brief Entity::GetBehavior Gets a specified behavior from the entity
 * @param type the type of behavior
 * @return the behavior, if found, nullptr otherwise.
 */
IBehavior* Entity::GetBehavior(const char *type)
{
    if(this->FindBehavior(type))
    {
       return _Behaviors[type];
    }

    return nullptr;
}
