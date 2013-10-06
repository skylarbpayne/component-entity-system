/**
 * Particle System is in control of particle instantiation.
 *
 * @author      Caleb Geiger
 * Created:     8-27-2013
 * Source File:  ParticleSystem.h
 */
 #pragma once

#include "ISystem.h"
#include "ParticleComponent.h"

using namespace sf;

class ParticleSystem : public ISystem : public WindowAccessor {

    void resetParticle(ParticleComponent* pc, size_t index) ;

public:

    ParticleSystem() : ISystem("Particle") { }
    void update(Time elapsed, RenderTarget& target, RenderStates states);
    bool ValidateEntity(unsigned int ID);
};
