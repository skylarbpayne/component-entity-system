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
public:
    IRenderComponent(const char* type, unsigned int z = 0) : IComponent(type), _priorityZ(z) {
    }
    virtual sf::Drawable const& GetDrawable() const = 0;
    virtual void SetPosition(sf::Vector2f const& pos) = 0;
    unsigned int GetPriorityZ() const { return _priorityZ; }
    virtual ~IRenderComponent( ) {    }
};
