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

#include "Logger.h"

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
    ParticleComponent(unsigned int count = 0, unsigned int z = 0, sf::Vector2f offset = sf::Vector2f(0.f, 0.f)) :
        IRenderComponent("Particle", z, offset), _particleInfo(count), _particles(count), _lifeTime(sf::seconds(3)) { }

    void resizeParticles(unsigned int count) {
        _particles._vertices.resize(count);
        _particleInfo.resize(count);
    }

    void resetParticle(unsigned int index) {
        float angle = (rand() % 360) * 3.14f / 180.f;
        float speed = (rand() % 50) + 50.0f;
        _particleInfo[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        _particleInfo[index].life = sf::milliseconds((rand() % 2000) + 1000);
        _particles._vertices[index].position = _particles.getPosition();
    }

    void rLoad(lua_State *L) override
    {

        lua_pushstring(L, "count");
        lua_gettable(L, -2);
        this->resizeParticles(lua_tonumber(L, -1));
        lua_pop(L, 1);

        lua_pushstring(L, "lifetime");
        lua_gettable(L, -2);
        _lifeTime = sf::seconds(lua_tonumber(L, -1));
        lua_pop(L, 1);

        lua_getglobal(L, "x");
        lua_getglobal(L, "y");

        _particles.setPosition(lua_tonumber(L, -2) + _offset.x, lua_tonumber(L, -1) + _offset.y);
        lua_pop(L, 2);
    }

    sf::Drawable const& GetDrawable() const { return _particles; }
    void SetPosition(sf::Vector2f const& pos) {
        _particles.setPosition(sf::Vector2f(pos.x + 10, pos.y + 10));
    }
};
