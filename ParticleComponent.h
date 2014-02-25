/**
    * Defines both the Particle component and graphical representation Particles class.
    *
    * Author: Skylar Payne
    * Date: 02/21/2014
    * File: ParticleComponent.h
**/

#pragma once

#include "IComponent.h"
#include "SFML/Graphics.hpp"

class Particles : public sf::Drawable, public sf::Transformable
{
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)
    {
        states.transform *= getTransform();

        states.texture = nullptr;

        target.draw(m_vertices, states);
    }
};
