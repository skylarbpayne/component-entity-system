/**
    * The EntityFactory creates Entities through Lua config scripts
    *
    * Author: Skylar Payne
    * Date: 8/9/2013
    * File: EntityFactory.h
**/

#pragma once

#include <functional>
#include "HashTable.h"
#include "IComponent.h"
#include "IListener.h"

typedef std::unordered_map<const char*, std::function<IComponent*()>, eqstr, eqstr> constructorMap;

class EntityFactory : public IListener<CreateEntityMessage>
{
private:
    constructorMap _ConstructorMap;
private:
    void OnMessage(CreateEntityMessage& msg) override;
public:
    virtual ~EntityFactory() { }

    bool Register(const char* componentType, std::function<IComponent* ()> constructor);
    void Create(const char* entity, float x = 0, float y = 0);
};
