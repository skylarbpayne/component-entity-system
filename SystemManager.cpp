/**
	* SystemManager manages all component systems. Allows the addition and removal of systems. Updates all systems,
    * and facilitates pausing all systems.
    *
	* Author: Skylar Payne
	* Date: 6/23/2013
	* File: SystemManager.cpp
**/

#include "SystemManager.h"
#include "ISystem.h"
#include <cstring>

#include "Logger.h"

/**
 * @brief SystemManager::~SystemManager deallocates all systems
 */
SystemManager::~SystemManager()
{
    std::list<ISystem*>::iterator it;

    for(it = _Systems.begin(); it != _Systems.end(); it++)
    {
        if(*it)
        {
            delete (*it);
            (*it) = nullptr;
        }
    }
}

/**
 * @brief SystemManager::Find finds a specified system
 * @param type the type of system to look for
 * @param loc the location of the system, if found
 * @return true if the system was found, false otherwise
 */
bool SystemManager::Find(const char *type, std::list<ISystem*>::iterator &loc)
{
    for(loc = _Systems.begin(); loc != _Systems.end(); loc++)
    {
        if(strcmp((*loc)->GetType(), type) == 0)
        {
            return true;
        }
    }

    return false;
}

/**
    * @brief SystemManager::Add Adds a system to manager if a system of that type has not already been added
	* @param sys The system to add
	* @return true if the system was successfully added, false otherwise
**/
bool SystemManager::Add(ISystem* sys)
{
    if(sys == nullptr)
	{
        g_Logger << __FILE__ << ": " << __LINE__ << "-Error: sys pointer was nullptr\n";
		return false;
	}

	std::list<ISystem*>::iterator it;
    if(this->Find(sys->GetType(), it))
	{
        g_Logger << __FILE__ << ": " << __LINE__ << "-Error: " << sys->GetType() << " already exists in SystemManager\n";
		return false;
	}

	_Systems.push_back(sys);
    g_Logger << __FILE__ << ": " << __LINE__ << "-" << sys->GetType() << " was added to SystemManager\n";
    g_Logger.flush();
	return true;
}

/**
    * @brief SystemManager::Remove Removes a system from the manager
	* @param type The type of system to remove
**/
void SystemManager::Remove(const char* type)
{
	std::list<ISystem*>::iterator it;
    if(this->Find(type, it))
	{
		_Systems.erase(it);
        g_Logger << __FILE__ << ": " << __LINE__ << "-" << type << " was removed from the SystemManager\n";
	}
}

/**
    * @brief SystemManager::Has Checks to see if a system exists within the manager
	* @param type The type to check for
	* @return true if the system was found, false otherwise
**/
bool SystemManager::Has(const char* type)
{
    std::list<ISystem*>::iterator loc;
    return this->Find(type, loc);
}

/**
    * @brief SystemManager::Activate Activates the manager (Useful for unpausing)
**/
void SystemManager::Activate()
{
	_Active = true;
}

/**
 * @brief SystemManager::Deactivate Deactivates the manager (Useful for pausing)
 */
void SystemManager::Deactivate()
{
	_Active = false;
}

/**
    * @brief SystemManager::isActive Checks whether the manager is active
	* @return true if active, false otherwise
**/
bool SystemManager::isActive() const
{
	return _Active;
}

/**
    * @brief SystemManager::Update Calls the update function of all active systems
**/
void SystemManager::Update()
{
	std::list<ISystem*>::iterator it;
	for(it = _Systems.begin(); it != _Systems.end(); it++)
	{
		if( (*it) && (*it)->isActive())
		{
            (*it)->Update(_SystemClock.getElapsedTime());
		}
	}
    _SystemClock.restart();
}
