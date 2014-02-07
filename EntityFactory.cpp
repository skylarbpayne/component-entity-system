/**
    * The EntityFactory creates Entities through Lua config scripts
    *
    * Author: Skylar Payne
    * Date: 8/9/2013
    * File: EntityFactory.cpp
**/

#include "EntityFactory.h"
#include <lua.hpp>
#include "Entity.h"
#include "IListener.h"
#include "ScriptableBehavior.h"
#include "Logger.h"

/**
 * @brief EntityFactory::Register allows the use of lambda functions to register component types so that entities can be constructed
 * @param componentType the type of component (same string referenced in Lua!)
 * @param constructor the constructor of the component of componentType
 * @return true if the constructor is registered, false otherwise
 */
bool EntityFactory::Register(const char *componentType, std::function<IComponent*()> constructor)
{
    constructorMap::iterator it = _ConstructorMap.find(componentType);

    if(it != _ConstructorMap.end())
    {
        return true;
    }

    _ConstructorMap[componentType] = constructor;
    return true;
}

/**
 * @brief EntityFactory::Create creates an entity from a Lua script
 * @param entity the file from which to load the entity
 */
void EntityFactory::Create(const char* entity, float x, float y)
{
    lua_State* L = luaL_newstate();

    if(luaL_dofile(L, entity))
    {
        g_Logger << lua_error(L) << lua_tostring(L, -1) << "\n";
        g_Logger.flush();
        lua_pop(L, 1);
        return;
    }

    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    lua_setglobal(L, "y");
    lua_setglobal(L, "x");

    Entity* e = new Entity();
    IComponent* c = nullptr;

    lua_settop(L, 0);
    lua_getglobal(L, "Components");

    if(lua_isnil(L, -1) == 0)
    {
        lua_pushnil(L);

        while(lua_next(L, 1) != 0)
        {
            const char* temp = lua_tostring(L, -2);

            if(strcmp(temp, "Tag") == 0)
            {
                e->SetTag(lua_tostring(L, -1));
                lua_pop(L, 1);
                continue;
            }

            else if(_ConstructorMap.find(temp) == _ConstructorMap.end())
            {
                g_Logger << temp << " component type not found. Continuing\n";
                lua_pop(L, 1);
                continue;
            }

            c = _ConstructorMap[temp]();
            c->Load(L);
            lua_settop(L, 3);
            e->AttachComponent(c);

            lua_pop(L, 1);
        }
    }

    lua_settop(L, 0);
    lua_getglobal(L, "Behaviors");

    if(lua_isnil(L, -1) == 0)
    {
        lua_pushnil(L);
        ScriptableBehavior* b = nullptr;

        while(lua_next(L, 1) != 0)
        {
            b = new ScriptableBehavior(lua_tostring(L, -2), lua_tostring(L, -1));
            e->AttachBehavior(b);
            lua_pop(L, 1);
        }
    }

    lua_close(L);


    AddEntityMessage msg;
    msg.entity = e;
    Emit<AddEntityMessage>(msg);
}

/**
 * @brief EntityFactory::OnMessage calls the create function with data held in msg
 * @param msg position and file from which to load entity.
 */
void EntityFactory::OnMessage(CreateEntityMessage& msg)
{
    Create(msg.script, msg.position.x, msg.position.y);
}
