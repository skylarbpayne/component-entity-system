/**
    * The CollisionSystem tests for collisions between entities with a Collider component
    *
    * Author: Skylar Payne
    * Date: 8/5/2013
    * File: CollisionSystem.h
**/

#include "CollisionSystem.h"
#include "PositionComponent.h"
#include "ColliderComponent.h"
#include "Entity.h"

/**
 * @brief CollisionSystem::OnMessage Checks for collisions when an entity moves
 * @param msg holds entity ID
 */
void CollisionSystem::OnMessage(EntityMovedMessage& msg)
{
    if(this->GetEntity(msg.ID)->HasComponent("Collider"))
    {
        _MovedEntities.insert(msg.ID);
    }
}

/**
 * @brief CollisionSystem::CheckCollision checks for a collision between two entities
 * @param e1 the first entity
 * @param e2 the second entity
 * @param norm the normal of collision, if they collided
 * @return true if the two entities are colliding, false otherwise
 */
bool CollisionSystem::CheckCollision(Entity* e1, Entity* e2, sf::Vector2f& norm)
{
    sf::Vector2f const& p1 = e1->GetComponent<PositionComponent>("Position")->GetPosition();
    sf::Vector2f const& p2 = e2->GetComponent<PositionComponent>("Position")->GetPosition();

    ColliderComponent* c1 = e1->GetComponent<ColliderComponent>("Collider");
    ColliderComponent* c2 = e2->GetComponent<ColliderComponent>("Collider");

    //Reset the normal vector to avoid any miscalculations later on
    norm.x = 0.f;
    norm.y = 0.f;

    //Find the respective x and y distances... Used later to determine level of penetration
    float distx, disty, absdistx, absdisty = 0.f;

    distx = (p2.x + c2->_Offset.x + c2->_Dimensions.x / 2) - (p1.x + c1->_Offset.x + c1->_Dimensions.x / 2);
    disty = (p2.y + c2->_Offset.y + c2->_Dimensions.y / 2) - (p1.y + c1->_Offset.y + c1->_Dimensions.y / 2);

    //Make sure both distances are positive... Direction doesn't matter
    (distx < 0) ? absdistx = distx * -1: absdistx = distx;
    (disty < 0) ? absdisty = disty * -1 : absdisty = disty;

    //Use these combined half dimensions along with distances to determine level of penetration
    float halfWidths = (c1->_Dimensions.x + c2->_Dimensions.x) / 2;
    float halfHeights = (c1->_Dimensions.y + c2->_Dimensions.y) / 2;

    //If both distances are greater than or equal to the half dimensions, no collision, return false.
    if(!((absdistx < halfWidths) && (absdisty < halfHeights)))
        return false;

    //Otherwise, there was a collision, let's find where it happened!
    //overlap is determined by the distance in half dimensions and distances
    float xMag = halfWidths - absdistx;
    float yMag = halfHeights - absdisty;

    //Least significant overlap determines surface of collision...
    if(xMag < yMag)
        norm.x = (distx > 0) ? xMag : -xMag;
    else
        norm.y = (disty > 0) ? yMag : -yMag;

    return true;
}

bool CollisionSystem::CheckCollisions(unsigned int id, Vector2f &norm)
{
    Entity* e1 = this->GetEntity(id);
    Entity* e2 = nullptr;

    std::set<unsigned int>::iterator eit;

    for(eit = _EntitiesToUpdate.begin(); eit != _EntitiesToUpdate.end(); eit++)
    {
        if(id == *eit)
        {
            continue;
        }

        e2 = this->GetEntity(*eit);

        if(CheckCollision(e1, e2, norm))
        {
            CollisionMessage cmsg;
            cmsg.ID1 = id;
            cmsg.ID2 = *eit;
            cmsg.norm.x = norm.x;
            cmsg.norm.y = norm.y;
            Emit<CollisionMessage>(cmsg);
            return true;
        }
    }
    return false;
}

/**
 * @brief CollisionSystem::Update checks for collisions between entities that have moved and all other entities
 * @param dt frame time
 */
void CollisionSystem::Update(sf::Time dt)
{
    PositionComponent* p1 = nullptr;
    sf::Vector2f norm;
    std::set<unsigned int> movedEntities = _MovedEntities;
    _MovedEntities.clear();
    std::set<unsigned int>::iterator mit;

    for(mit = movedEntities.begin(); mit != movedEntities.end(); mit++)
    {
        p1 = this->GetEntity(*mit)->GetComponent<PositionComponent>("Position");

        if(CheckCollisions(*mit, norm))
        {
            MoveEntityMessage msg;
            msg.ID = *mit;
            msg.newPosition.x = p1->GetPosition().x - norm.x;
            msg.newPosition.y = p1->GetPosition().y - norm.y;
            Emit<MoveEntityMessage>(msg);
        }
    }
}

/**
 * @brief CollisionSystem::ValidateEntity validates an entity to update
 * @param ID the entity to validate
 */
bool CollisionSystem::ValidateEntity(unsigned int ID)
{
    Entity* e = this->GetEntity(ID);

    bool rtn = e->HasComponent("Collider") && e->HasComponent("Position");

    if(rtn)
    {
        EntityMovedMessage msg;
        msg.ID = ID;
        sf::Vector2f const& pos = e->GetComponent<PositionComponent>("Position")->GetPosition();
        msg.oldPosition = pos;
        msg.newPosition = pos;
        Emit<EntityMovedMessage>(msg);
    }

    return rtn;
}
