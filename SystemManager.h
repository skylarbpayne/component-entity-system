/**
	* SystemManager manages all component systems. Allows the addition and removal of systems. Updates all systems,
    * and facilitates pausing all systems.
    *
	* Author: Skylar Payne
	* Date: 6/22/2013
	* File: SystemManager.h
**/

#pragma once

#include <list>
#include <SFML/System.hpp>

class ISystem;

class SystemManager
{
private:
	bool _Active;
    sf::Clock _SystemClock;
	std::list<ISystem*> _Systems;
private:
    bool Find(const char* type, std::list<ISystem*>::iterator& loc);
public:
    SystemManager() : _Active(true) { }
    ~SystemManager();

	bool Add(ISystem* sys);
    void Remove(const char* type);
    bool Has(const char* type);
	void Activate();
	void Deactivate();
	bool isActive() const;
	void Update();
};
