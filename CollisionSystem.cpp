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

#include "Logger.h"

/**
 * @brief CollisionSystem::CollisionSystem allocates entity bins to use for spatial partitions
 * @param mapSize the dimensions of the map (can be screen!)
 * @param binSize the dimensions of each bin (keep it uniform to keep it simple).
 */
CollisionSystem::CollisionSystem(Vector2f mapSize, Vector2f binSize) : ISystem("Collision")
{
    _MapSize.x = mapSize.x;
    _MapSize.y = mapSize.y;
    _BinSize.x = binSize.x;
    _BinSize.y = binSize.y;
    _NumBinsX = mapSize.x / binSize.x;
    _NumBins = _NumBinsX * (mapSize.y / binSize.y);

    _Bins = new EntityBin[_NumBins];

    for(unsigned int i = 0; i < _NumBins; i++)
    {
        _Bins[i].SetGlobalBounds(sf::FloatRect((i % _NumBinsX) * _BinSize.x, (i / _NumBinsX) * _BinSize.y, _BinSize.x, _BinSize.y));
    }
}

/**
 * @brief CollisionSystem::OnMessage Checks for collisions when an entity moves
 * @param msg holds entity ID
 */
void CollisionSystem::OnMessage(EntityMovedMessage& msg)
{
    if(this->GetEntity(msg.ID)->HasComponent("Collider"))
    {
        _MovedEntities.push_front(msg.ID);
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

/**
 * @brief CollisionSystem::CheckCollisions checks for collisions between an entity and all other entities in it's vicinity
 * @param id the ID of an entity
 * @param norm the collision normal, if there is a collision
 * @return true if there is a collision, false otherwise
 */
bool CollisionSystem::CheckCollisions(unsigned int id, Vector2f &norm)
{
    Entity* e1 = this->GetEntity(id);
    Entity* e2 = nullptr;

    std::set<unsigned int> bins;
    if(!GetBins(e1, bins))
    {
        return false;
    }

    std::set<unsigned int>::iterator bit;
    std::set<unsigned int>::iterator eit;

    for(bit = bins.begin(); bit != bins.end(); bit++)
    {
        g_Logger << *bit << "\n";
        g_Logger.flush();

        for(eit = _Bins[*bit].GetEntities().begin(); eit != _Bins[*bit].GetEntities().end(); eit++)
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
    }
    return false;
}

/**
 * @brief CollisionSystem::GetBins gets all bins in which an entity belongs
 * @param e the entity
 * @param bins the bins the entity belongs to, if return value is true
 * @return true if entity is completely in defined game world, false otherwise
 */
bool CollisionSystem::GetBins(Entity *e, std::set<unsigned int>& bins)
{
    bins.clear();
    ColliderComponent* cc = e->GetComponent<ColliderComponent>("Collider");
    PositionComponent* pc = e->GetComponent<PositionComponent>("Position");
    sf::Vector2f pos = pc->GetPosition() + cc->GetOffset();
    sf::Vector2f dim = cc->GetDimensions();

    unsigned int bin[4];
    bin[0] = (pos.y / _BinSize.y) * _NumBinsX + (pos.x / _BinSize.x);
    bin[1] = ((pos.y + dim.y) / _BinSize.y) * _NumBinsX + (pos.x / _BinSize.x);
    bin[2] = (pos.y / _BinSize.y) * _NumBinsX + ((pos.x + dim.x) / _BinSize.x);
    bin[3] = ((pos.y + dim.y) / _BinSize.y) * _NumBinsX + ((pos.x + dim.x) / _BinSize.x);

    for(unsigned int i = 0; i < 4; i++)
    {
        if(bin[i] >= _NumBins)
        {
            return false;
        }

        bins.insert(bin[i]);
    }
    return true;
}

/**
 * @brief CollisionSystem::FillBins puts all entities in their proper bins
 */
void CollisionSystem::FillBins()
{   
    std::set<unsigned int>::iterator it;
    std::set<unsigned int> bins;
    Entity* e = nullptr;
    for(it = _EntitiesToUpdate.begin(); it != _EntitiesToUpdate.end(); it++)
    {
        e = this->GetEntity(*it);

        if(GetBins(e, bins))
        {
            std::set<unsigned int>::iterator t;
            for(t = bins.begin(); t != bins.end(); t++)
            {
                _Bins[*t].AddEntity(*it);
            }
        }
    }
}

/**
 * @brief CollisionSystem::ClearBins clears all entities from all bins
 */
void CollisionSystem::ClearBins()
{
    for(unsigned int i = 0; i < _NumBins; i++)
    {
        _Bins[i].clear();
    }
}

/**
 * @brief CollisionSystem::Update checks for collisions between entities that have moved and all other entities
 * @param dt frame time
 */
void CollisionSystem::Update(sf::Time dt)
{
    PositionComponent* p1 = nullptr;
    sf::Vector2f norm;
    std::list<unsigned int> movedEntities = _MovedEntities;
    _MovedEntities.clear();
    std::list<unsigned int>::iterator mit;

    FillBins();

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

    ClearBins();
}

/**
 * @brief CollisionSystem::ValidateEntity validates an entity to update
 * @param ID the entity to validate
 */
bool CollisionSystem::ValidateEntity(unsigned int ID)
{
    Entity* e = this->GetEntity(ID);

    sf::Vector2f norm;
    if(CheckCollisions(ID, norm))
    {
        DestroyEntityMessage dmsg;
        dmsg.ID = ID;
        Emit<DestroyEntityMessage>(dmsg);
        return false;
    }

    return e->HasComponent("Collider") && e->HasComponent("Position");
}
