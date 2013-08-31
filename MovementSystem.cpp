/**
    * The MovementSystem updates PositionComponents
    *
    * Author: Skylar Payne
    * Date: 8/3/2013
    * File: MovementSystem.h
**/

#include "MovementSystem.h"

#include "Entity.h"
#include "MovementComponent.h"
#include "PositionComponent.h"

/**
 * @brief MovementSystem::Update Updates all entities with position and movement components
 * @param dt the time splice between frames
 */
void MovementSystem::Update(sf::Time dt)
{
    std::set<unsigned int>::iterator it;

    for(it = _EntitiesToUpdate.begin(); it != _EntitiesToUpdate.end(); ++it)
    {
        Entity* e = this->GetEntity(*it);

        MovementComponent* mc = e->GetComponent<MovementComponent>("Movement");
        sf::Vector2f const& vel = mc->GetVelocity();

        if(vel.x == 0 && vel.y == 0)
        {
            continue;
        }

        PositionComponent* pc = e->GetComponent<PositionComponent>("Position");
        sf::Vector2f const& pos = pc->GetPosition();

        pc->SetPosition(pos.x + vel.x, pos.y + vel.y);

        EntityMovedMessage msg;
        msg.ID = (*it);
        msg.oldPosition = pos;
        msg.newPosition = pc->GetPosition();
        Emit<EntityMovedMessage>(msg);
    }
}

/**
 * @brief MovementSystem::ValidateEntity validates an entity for updating
 * @param ID the entity to validate
 * @return true if validated, false otherwise
 */
bool MovementSystem::ValidateEntity(unsigned int ID)
{
    Entity* e = this->GetEntity(ID);

    return (e->HasComponent("Position") && e->HasComponent("Movement"));
}

/**
 * @brief MovementSystem::OnMessage Updates an entity's position
 * @param msg contains ID and newPosition
 */
void MovementSystem::OnMessage(MoveEntityMessage& msg)
{
    Entity* e = this->GetEntity(msg.ID);
    PositionComponent* pc = e->GetComponent<PositionComponent>("Position");

    EntityMovedMessage newMsg;
    newMsg.ID = msg.ID;
    newMsg.oldPosition = pc->GetPosition();
    newMsg.newPosition = msg.newPosition;

    pc->SetPosition(msg.newPosition.x, msg.newPosition.y);
    Emit<EntityMovedMessage>(newMsg);
}

/**
 * @brief MovementSystem::OnMessage Updates an entity's velocity
 * @param msg contains ID and newVelocity
 */
void MovementSystem::OnMessage(PushEntityMessage& msg)
{
    Entity* e = this->GetEntity(msg.ID);
    e->GetComponent<MovementComponent>("Movement")->SetVelocity(msg.newVelocity.x, msg.newVelocity.y);
}
