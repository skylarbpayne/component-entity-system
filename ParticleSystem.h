/**
    * The ParticleSystem class handles all the updating
    *
    * Author: Skylar Payne
    * Date: 03/16/2014
    * File: ParticleSystem.h
**/

#pragma once

#include "ISystem.h"

class ParticleSystem : public ISystem
{
private:
public:
    ParticleSystem() : ISystem("particle") { }
    void Update(sf::Time dt) override;
    bool ValidateEntity(unsigned int ID) override;
};
