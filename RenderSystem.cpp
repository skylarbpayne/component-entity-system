/** Render System Implementation
 *
 * @author      Caleb Geiger
 * Created:     8-2-2013
 * Source File:  RenderSystem.cpp
 */
#include "Entity.h"
#include "IRenderComponent.h"
#include "RenderSystem.h"

void RenderSystem::Update(sf::Time dt)
{
    std::set<unsigned int>::iterator it;

    for(it = _EntitiesToUpdate.begin(); it != _EntitiesToUpdate.end(); ++it)
    {
        Entity* e = this->GetEntity(*it);

        IRenderComponent* rc = e->GetComponent<IRenderComponent>("Render");
        this->GetWindow()->draw(rc->GetDrawable());

    }
}

bool RenderSystem::ValidateEntity(unsigned int ID)
{
    Entity* e = this->GetEntity(ID);

    return e->HasComponent("Render");
}

void RenderSystem::OnMessage(EntityMovedMessage& msg)
{
    Entity* e = this->GetEntity(msg.ID);
    IRenderComponent* rc = e->GetComponent<IRenderComponent>("Render");
    rc->SetPosition(msg.newPosition);
}
