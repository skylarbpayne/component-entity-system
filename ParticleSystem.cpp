/**
 * The implementation of Particles.
 *
 * @author      Caleb Geiger
 * Created:     10-6-2013
 * Source File: ParticleSystem.cpp
 */

#pragma once
#include "ParticleSystem.h"

void ParticleSystem::resetParticle(ParticleComponent* pc, size_t index) {
        float angle = ((rand() % (pc->_MaxAngle - pc->_MinAngle)) + pc->_MinAngle )* 3.14f / 180.f;
        float speed = (rand() % (pc->_MaxVelocity - pc->_MinVelocity)) + pc->_MinVelocity;
        pc->_Particles[index].velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);
        pc->_Particles[index].lifetime = milliseconds((rand() % 2000) + 1000);

        pc->_Vertices[index].position = _Center;
}

void ParticleSystem::update(Time elapsed, RenderTarget& target, RenderStates states) {
        set<unsigned int>::iterator it;

        for(it = _EntitiesToUpdate.begin(); it != _EntitiesToUpdate.end(); ++it) {
            Entity* e = this->GetEntity(*it);
            ParticleComponent* pc = e->GetComponent<ParticleComponent>("Particle");

            for (size_t i = 0; i < pc->_Particles.size(); ++i) {
                // update the particle lifetime
                Particle& p = pc->_Particles[i];
                p.lifetime -= elapsed;

                // if the particle is dead, respawn it
                if (p.lifetime <= Time::Zero)
                    resetParticle(pc, i);

                // update the position of the corresponding vertex
                pc->_Vertices[i].position += p.velocity * elapsed.asSeconds();

                // update the alpha (transparency) of the particle according to its lifetime
                float ratio = p.lifetime.asSeconds() / _Lifetime.asSeconds();
                pc->_Vertices[i].color.a = static_cast<Uint8>(ratio * 255);

                states.transform *= getTransform();
                states.texture = nullptr;
                target.draw(pc->_Vertices, states);
            }
        }
}

bool ParticleSystem::ValidateEntity(unsigned int ID) {
        Entity* e = this->GetEntity(ID);

        return e->HasComponent("Particle");
}
