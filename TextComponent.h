/** Text Component Header
 *
 * @author      Caleb Geiger
 * Created:     8-19-2013
 * Source File:  TextComponent.h
 */

#pragma once
#include "IRenderComponent.h"
#include "ResourceAccessor.h"

using namespace std;
using namespace sf;


class TextComponent : public IRenderComponent, public ResourceAccessor
{
friend class RenderSystem;
private:
    Text _XText;
public:
    TextComponent(unsigned int z = 0, sf::Vector2f offset = sf::Vector2f(0.f, 0.f)) : IRenderComponent("text", z, offset) {
        _XText.setString("Text");
    }

    void SetPosition(const Vector2f &pos) override
    {
        _XText.setPosition(pos);
    }

    Text const& GetDrawable() const
    {
        return _XText;
    }
    void setText(string const& s){
        _XText.setString(s);
    }
    void setFont(const char* file){
        _XText.setFont(*this->GetFont(file));
    }
    void setSize(unsigned int i){
        _XText.setCharacterSize(i);
    }
    void setStyle(Uint32 sT){
        _XText.setStyle(sT);
    }
    void setColor(Color const& c){
        _XText.setColor(c);
    }
    String const& getText(){
        return _XText.getString();
    }
    Font const* getFont(){
       return _XText.getFont();
    }
    unsigned int getSize(){
        return _XText.getCharacterSize();
    }
    Uint32 getStyle(){
        return _XText.getStyle();
    }
    Color const& getColor(){
        return _XText.getColor();
    }

    void rLoad(lua_State *L) override
    {
        lua_pushstring(L, "font");
        lua_gettable(L, -2);
        setFont(lua_tostring(L, -1));
        lua_pop(L, 1);

        lua_pushstring(L, "size");
        lua_gettable(L, -2);
        setSize(lua_tonumber(L, -1));
        lua_pop(L, 1);

        lua_pushstring(L, "text");
        lua_gettable(L, -2);
        _XText.setString(lua_tostring(L, -1));
        lua_pop(L, 1);

        lua_getglobal(L, "x");
        lua_getglobal(L, "y");

        _XText.setPosition(lua_tonumber(L, -2) + _offset.x, lua_tonumber(L, -1) + _offset.y);
        lua_pop(L, 2);
    }
};
