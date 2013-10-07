/**
 * Particle System is in control of particle instantiation.
 *
 * @author      Caleb Geiger
 * Created:     8-27-2013
 * Source File:  ParticleSystem.h
 */
<<<<<<< HEAD

using namespace sf;

=======
>>>>>>> cb41974323673df6cd1ab695d499d5699cb83591
 #pragma once

#include "ISystem.h"
#include "ParticleComponent.h"

<<<<<<< HEAD
class ParticleSystem : public ISystem {
=======
using namespace sf;

class ParticleSystem : public ISystem : public WindowAccessor {
>>>>>>> cb41974323673df6cd1ab695d499d5699cb83591

    void resetParticle(ParticleComponent* pc, size_t index) ;

public:

    ParticleSystem() : ISystem("Particle") { }
<<<<<<< HEAD

    void Update(Time elapsed) override {
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
            }
        }
    }

    bool ValidateEntity(unsigned int ID) {
        Entity* e = this->GetEntity(ID);

        return e->HasComponent("Particle");
    }
=======
    void update(Time elapsed, RenderTarget& target, RenderStates states);
    bool ValidateEntity(unsigned int ID);
>>>>>>> cb41974323673df6cd1ab695d499d5699cb83591
};
