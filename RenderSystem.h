/** Render System Header
 *
 * @author      Caleb Geiger
 * Created:     8-2-2013
 * Source File:  RenderSystem.h
 */

#pragma once

#include "ISystem.h"
#include "WindowAccessor.h"
#include "IListener.h"
//#include <functional>

class IRenderComponent;

typedef std::vector<std::vector<IRenderComponent*> > renderQueue;

class RenderSystem : public ISystem, public WindowAccessor, IListener<EntityMovedMessage>
{
private:
    unsigned int _depth;
    renderQueue _RenderQueue;
    //std::function<bool> _sort;
    void OnMessage(EntityMovedMessage& msg) override;
    void insertRenderComponent(unsigned int id);
    void removeRenderComponent(unsigned int id);
public:
    RenderSystem(unsigned int depth = 0) : ISystem("Render"), _depth(depth), _RenderQueue(_depth) { }
    void Update(sf::Time dt) override;
    bool ValidateEntity(unsigned int ID) override;
};
