/**
	* Wrapper class for collections of components and behaviors that constitute anything.
    *
	* Author: Skylar Payne
	* Date: 6/8/2013
	* File: Entity.h
**/

#pragma once

#include "HashMap.h"
#include "IComponent.h"
#include "IBehavior.h"

typedef hash_map<const char*, IComponent*> componentMap;
typedef hash_map<const char*, IBehavior*> behaviorMap;

class Entity
{
friend class EntityManager;
friend class BehaviorSystem;
private:
    unsigned int _ID;
    std::string _Tag;
    componentMap _Components;
    behaviorMap _Behaviors;
private:
    bool FindComponent(const char* type);
    bool FindBehavior(const char* type);
public:
    Entity(const char* tag = "") : _Tag(tag) { }
    ~Entity();

    unsigned int GetID() const { return _ID; }
    const char* GetTag() const { return _Tag.c_str(); }
    void SetTag(const char* tag) { _Tag = tag; }

    bool AttachComponent(IComponent* comp);
    void RemoveComponent(const char* type);
    bool HasComponent(const char* type);

    template<class T>
    T* GetComponent(const char* type);

    bool AttachBehavior(IBehavior* beh);
    void RemoveBehavior(const char* type);
    bool HasBehavior(const char* type);
    IBehavior* GetBehavior(const char* type);
};

/**
 * @brief Entity::GetComponent templated method to get a Component
 * @param type the type of component to get
 * @return the component, if found, nullptr otherwise.
 */
template<class T>
T* Entity::GetComponent(const char* type)
{
    if(this->FindComponent(type))
    {
        return static_cast<T*>(_Components[type]);
    }

    return nullptr;
}
