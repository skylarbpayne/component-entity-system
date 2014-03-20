/** Circle Component Header
 *
 * @author      Caleb Geiger
 * Created:     8-2-2013
 * Source File:  CircleComponent.h
 */

#pragma once
#include "IRenderComponent.h"

using namespace std;
using namespace sf;


class CircleComponent : public IRenderComponent
{
friend class RenderSystem;
private:
    CircleShape _Circle;
public:
    CircleComponent(unsigned int z = 0, sf::Vector2f offset = sf::Vector2f(0.f, 0.f)) : IRenderComponent("circle", z, offset) {
        _Circle.setRadius(10);
        _Circle.setFillColor(Color::Red);
    }

    void SetPosition(const Vector2f &pos) override
    {
        _Circle.setPosition(pos);
    }

    Drawable const& GetDrawable() const override
    {
        return _Circle;
    }
    void setRadius(float radius){
        _Circle.setRadius(radius);
    }
    void setColor(Color const& c){
        _Circle.setFillColor(c);
    }
    float getRadius(){
        return _Circle.getRadius();
    }
    Color const& getColor(){
        return _Circle.getFillColor();
    }

    void rLoad(lua_State *L) override
    {
        sf::Color color;

        lua_pushstring(L, "radius");
        lua_gettable(L, -2);
        _Circle.setRadius(lua_tonumber(L, -1));
        lua_pop(L, 1);

        lua_pushstring(L, "r");
        lua_gettable(L, -2);
        color.r = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "g");
        lua_gettable(L, -2);
        color.g = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "b");
        lua_gettable(L, -2);
        color.b = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "a");
        lua_gettable(L, -2);
        color.a = lua_tonumber(L, -1);
        lua_pop(L, 1);

        _Circle.setFillColor(color);

        lua_getglobal(L, "x");
        lua_getglobal(L, "y");

        _Circle.setPosition(lua_tonumber(L, -2) + _offset.x, lua_tonumber(L, -1) + _offset.y);
        lua_pop(L, 2);
    }

};
