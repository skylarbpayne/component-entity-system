/**
    * Defines implementation of ParticleSystem class
    *
    * Author: Skylar Payne
    * Date: 03/16/2014
    * File: ParticleSystem.cpp
**/

#include "ParticleSystem.h"
#include "ParticleComponent.h"
#include "Entity.h"

void ParticleSystem::Update(sf::Time dt)
{
    for(unsigned int id : _EntitiesToUpdate) {
        Entity* e = this->GetEntity(id);
        ParticleComponent* pc = e->GetComponent<ParticleComponent>("particle");

        for(std::size_t i = 0; i < pc->_particleInfo.size(); ++i) {
            ParticleInfo& p = pc->_particleInfo[i];
            p.life -= dt;
            if(p.life <= sf::Time::Zero)
                pc->resetParticle(i);

            pc->_particles._vertices[i].position += p.velocity * dt.asSeconds();
            float ratio = p.life.asSeconds() / pc->_lifeTime.asSeconds();
            pc->_particles._vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
        }
    }
}

bool ParticleSystem::ValidateEntity(unsigned int ID)
{
    Entity* e = this->GetEntity(ID);
    return e->HasComponent("Particle");
}
