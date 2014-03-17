/** Rectangle Component Header
 *
 * @author      Caleb Geiger
 * Created:     8-19-2013
 * Source File:  RectangleComponent.h
 */

#pragma once
#include "IRenderComponent.h"

using namespace std;
using namespace sf;


class RectangleComponent : public IRenderComponent
{
friend class RenderSystem;
private:
    RectangleShape _Rectangle;
public:
    RectangleComponent() : IRenderComponent("rectangle") {
        _Rectangle.setSize(Vector2f(100, 50));
        _Rectangle.setFillColor(sf::Color::Red);
    }

    void SetPosition(const Vector2f &pos) override
    {
        _Rectangle.setPosition(pos);
    }

    RectangleShape const& GetDrawable() const
    {
        return _Rectangle;
    }
    void setSize(float x, float y){
        _Rectangle.setSize(Vector2f(x, y));
    }
    void setColor(Color const& c){
        _Rectangle.setFillColor(c);
    }
    Vector2f const& getSize(){
        return _Rectangle.getSize();
    }
    Color const& getColor(){
        return _Rectangle.getFillColor();
    }

    void Load(lua_State *L) override
    {
        sf::Vector2f dim;
        sf::Vector2f pos;
        sf::Color color;

        lua_pushstring(L, "w");
        lua_gettable(L, -2);
        dim.x = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "h");
        lua_gettable(L, -2);
        dim.y = lua_tonumber(L, -1);
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

        _Rectangle.setSize(dim);
        _Rectangle.setFillColor(color);

        lua_getglobal(L, "x");
        lua_getglobal(L, "y");

        _Rectangle.setPosition(lua_tonumber(L, -2), lua_tonumber(L, -1));
    }
};
