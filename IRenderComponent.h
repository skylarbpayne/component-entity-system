/** IRender Component Header
 *
 * @author      Caleb Geiger
 * Created:     8-19-2013
 * Source File:  IRenderComponent.h
 */

#pragma once
#include "IComponent.h"
#include <SFML/Graphics.hpp>
#include <lua.hpp>

class IRenderComponent : public IComponent{
    friend class RenderSystem;
protected:
    unsigned int _priorityZ;
    sf::Vector2f _offset;

public:
    IRenderComponent(const char* type, unsigned int z = 0, sf::Vector2f offset = sf::Vector2f(0.f, 0.f)) :
        IComponent(type), _priorityZ(z), _offset(offset) { }
    virtual sf::Drawable const& GetDrawable() const = 0;
    virtual void SetPosition(sf::Vector2f const& pos) = 0;
    unsigned int GetPriorityZ() const { return _priorityZ; }
    sf::Vector2f GetOffset() const { return _offset; }
    virtual ~IRenderComponent( ) { }

    void Load(lua_State *L) final
    {
        lua_pushstring(L, "z");
        lua_gettable(L, -2);
        _priorityZ = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "offsetx");
        lua_gettable(L, -2);
        _offset.x = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "offsety");
        lua_gettable(L, -2);
        _offset.y = lua_tonumber(L, -1);
        lua_pop(L, 1);

        rLoad(L);
    }

    virtual void rLoad(lua_State* L) = 0;
};
