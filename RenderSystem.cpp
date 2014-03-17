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
    for(int i = _RenderQueue.size() - 1; i >= 0; i--)
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
void RenderSystem::insertRenderComponent(unsigned int id, const char* type)
{
    Entity* e = this->GetEntity(id);
    IRenderComponent* rc = e->GetComponent<IRenderComponent>(type);

    if(rc->GetPriorityZ() >= _RenderQueue.size())
    {
        g_Logger << __FILE__ << ": " << __LINE__ << "-" << "Priority Z of " << rc->GetPriorityZ() << " for Entity ";
        g_Logger << id << "is not in depth buffer\n";
        return;
    }

    _RenderQueue[rc->GetPriorityZ()].push_back(rc);
}

void RenderSystem::removeRenderComponent(unsigned int id, const char* type)
{
    //Will remove entity later. Have to think through how to do this efficiently.
    Entity* e = this->GetEntity(id);
    IRenderComponent* rc = e->GetComponent<IRenderComponent>(type);

    if(rc->GetPriorityZ() < _RenderQueue.size())
    {
        //Find and remove
        for(auto it = _RenderQueue[rc->GetPriorityZ()].begin(); it != _RenderQueue[rc->GetPriorityZ()].end(); it++)
        {
            if((*it) == rc)
            {
                _RenderQueue[rc->GetPriorityZ()].erase(it);
                return;
            }
        }
    }
}

bool RenderSystem::ValidateEntity(unsigned int ID)
{
    Entity* e = this->GetEntity(ID);
    bool validated = false;
    for(const char* type : _renderTypes) {
        if(e->HasComponent(type)) {
            insertRenderComponent(ID, type);
            validated = true;
        }
    }
    return validated;
}

void RenderSystem::OnMessage(EntityMovedMessage& msg)
{
    if(_EntitiesToUpdate.find(msg.ID) != _EntitiesToUpdate.end())
    {
        Entity* e = this->GetEntity(msg.ID);
        for(const char* type : _renderTypes) {
            IRenderComponent* rc = e->GetComponent<IRenderComponent>(type);
            if(rc)
                rc->SetPosition(msg.newPosition);
        }
    }
}
