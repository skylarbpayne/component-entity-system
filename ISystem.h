/**
	* Interface for all systems. Systems act on a particular set of components.
    *
	* Author: Skylar Payne
	* Date: 6/8/2013
	* File: ISystem.h
**/

#pragma once

#include <set>
#include "EntityAccessor.h"
#include "IListener.h"

class ISystem : public EntityAccessor, public IListener<EntityMessage>
{
protected:
	bool _Active;
    const char* _Type;
    std::set<unsigned int> _EntitiesToUpdate;
private:
    void OnMessage(EntityMessage& msg) override final;
public:
    ISystem(const char* type) : _Active(true), _Type(type) { }
    virtual ~ISystem() {}
	void Activate() { _Active = true; }
	void Deactivate() { _Active = false; }
	bool isActive() const { return _Active; }
    const char* GetType() const { return _Type; }
    virtual void Update(sf::Time dt) = 0;
    virtual bool ValidateEntity(unsigned int ID) = 0;
};

/**
 * @brief ISystem::OnMessage When a new entity is added or destroyed, this will determine whether to add/remove entity from list
 * @param msg the message data
 */
inline void ISystem::OnMessage(EntityMessage &msg)
{
    if(msg.Destroyed)
    {
        _EntitiesToUpdate.erase(msg.ID);
    }

    else if(ValidateEntity(msg.ID))
    {
        _EntitiesToUpdate.insert(msg.ID);
    }
}
