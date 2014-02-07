/** Render System Implementation
 *
 * @author      Caleb Geiger
 * Created:     8-2-2013
 * Source File:  RenderSystem.cpp
 */
#include "Entity.h"
#include "IRenderComponent.h"
#include "RenderSystem.h"
#include "Logger.h"
#include <algorithm>

void RenderSystem::Update(sf::Time dt)
{
    //sort queue
    //iterate through queue and render.
    for(unsigned int i = 0; i < _RenderQueue.size(); i++)
    {
        if(_sort)
        {
            std::sort (_RenderQueue[i].begin(), _RenderQueue[i].end(), _sort);
        }

        for(unsigned int j = 0; j < _RenderQueue[i].size(); j++)
        {
            this->GetWindow()->draw(_RenderQueue[i][j]->GetDrawable());
        }
    }
}
void RenderSystem::insertRenderComponent(unsigned int id)
{
    Entity* e = this->GetEntity(id);
    IRenderComponent* rc = e->GetComponent<IRenderComponent>("Render");

    if(rc->GetPriorityZ() >= _RenderQueue.size())
    {
        g_Logger << __FILE__ << ": " << __LINE__ << "-" << "Priority Z of " << rc->GetPriorityZ() << " for Entity ";
        g_Logger << id << "is not in depth buffer\n";
        return;
    }

    _RenderQueue[rc->GetPriorityZ()].push_back(rc);
}

void RenderSystem::removeRenderComponent(unsigned int id)
{
    //Will remove entity later. Have to think through how to do this efficiently.
}

bool RenderSystem::ValidateEntity(unsigned int ID)
{
    Entity* e = this->GetEntity(ID);
    if(e->HasComponent("Render"))
    {
        insertRenderComponent(ID);
        return true;
    }
    return false;
}

void RenderSystem::OnMessage(EntityMovedMessage& msg)
{
    if(_EntitiesToUpdate.find(msg.ID) != _EntitiesToUpdate.end())
    {
        Entity* e = this->GetEntity(msg.ID);
        IRenderComponent* rc = e->GetComponent<IRenderComponent>("Render");
        rc->SetPosition(msg.newPosition);
    }
}
