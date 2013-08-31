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


class RenderSystem : public ISystem, public WindowAccessor, IListener<EntityMovedMessage>
{
private:
    void OnMessage(EntityMovedMessage& msg) override;
public:
    RenderSystem() : ISystem("Render") { }
    void Update(sf::Time dt) override;
    bool ValidateEntity(unsigned int ID) override;
};
