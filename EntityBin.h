/**
    * An EntityBin holds a set of Entities that lie within it's bounds.
    *
    * Author: Skylar Payne
    * Date: 8/31/2013
    * File: EntityBin.h
**/

#include <SFML/Graphics.hpp>
#include <set>

class EntityBin
{
private:
    sf::FloatRect _GlobalBounds;
    std::set<unsigned int> _Entities;
public:
    sf::FloatRect const& GetGlobalBounds() const { return _GlobalBounds; }
    void SetGlobalBounds(sf::FloatRect bounds) { _GlobalBounds = bounds; }
    void AddEntity(unsigned int id) { _Entities.insert(id); }
    std::set<unsigned int>& GetEntities() { return _Entities; }
    void clear() { _Entities.clear(); }
};
