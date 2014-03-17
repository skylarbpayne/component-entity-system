/** Sprite Component Header
 *
 * @author      Caleb Geiger
 * Created:     8-19-2013
 * Source File:  SpriteComponent.h
 */

#pragma once
#include "IRenderComponent.h"
#include "ResourceAccessor.h"

using namespace std;
using namespace sf;

class SpriteComponent : public IRenderComponent, public ResourceAccessor
{
friend class RenderSystem;
private:
    Sprite _XSprite;
public:
    SpriteComponent() : IRenderComponent("sprite") { }
    void SetPosition(const Vector2f &pos) override
    {
        _XSprite.setPosition(pos);
    }

    Drawable const& GetDrawable() const
    {
        return _XSprite;
    }
    void setSpriteColor(Color const& s){
        _XSprite.setColor(s);
    }
    void setSpriteTexture(const char* file){
        _XSprite.setTexture(*this->GetTexture(file));
    }
    Color const getSpriteColor(){
        return _XSprite.getColor();
    }
    Texture const* getSpriteTexture(){
        return _XSprite.getTexture();
    }

    void Load(lua_State *L) override
    {
        lua_pushstring(L, "texture");
        lua_gettable(L, -2);
        setSpriteTexture(lua_tostring(L, -1));
        lua_pop(L, 1);

        lua_getglobal(L, "x");
        lua_getglobal(L, "y");

        _XSprite.setPosition(lua_tonumber(L, -2), lua_tonumber(L, -1));
        lua_pop(L, 2);
    }
};
