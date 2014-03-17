/**
    * Defines both the Particle component and graphical representation Particles class.
    *
    * Author: Skylar Payne
    * Date: 02/21/2014
    * File: ParticleComponent.h
**/

#pragma once

#include "IRenderComponent.h"
#include "SFML/Graphics.hpp"

class Particles : public sf::Drawable, public sf::Transformable
{
    friend class ParticleComponent;
    friend class ParticleSystem;
private:
    sf::VertexArray _vertices;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = nullptr;

        target.draw(_vertices, states);
    }

public:
    Particles(unsigned int count) : _vertices(sf::Points, count) { }
};

struct ParticleInfo
{
    sf::Vector2f velocity;
    sf::Time life;
};

class ParticleComponent : public IRenderComponent
{
    friend class ParticleSystem;
private:
    std::vector<ParticleInfo> _particleInfo;
    Particles _particles;
    sf::Time _lifeTime;
public:
    ParticleComponent(unsigned int count) : IRenderComponent("particle"), _particleInfo(count),
        _particles(count), _lifeTime(sf::seconds(3)) { }

    void resetParticle(unsigned int index) {
        float angle = (rand() % 360) * 3.14f / 180.f;
        float speed = (rand() % 50) + 50.0f;
        _particleInfo[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        _particleInfo[index].life = sf::milliseconds((rand() % 2000) + 1000);
        _particles._vertices[index].position = _particles.getPosition();
    }

    void Load(lua_State *L) { }
    sf::Drawable const& GetDrawable() const { return _particles; }
    void SetPosition(sf::Vector2f const& pos) {
        _particles.setPosition(sf::Vector2f(pos.x + 10, pos.y + 10));
    }
};
