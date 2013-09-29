/**
 * Particle Component is in control of particle instantiation, updating, rendering, and destruction.
 *
 * @author      Caleb Geiger
 * Created:     8-27-2013
 * Source File:  ParticleComponent.h
 */

#pragma once

class ParticleSystem;

using namespace std;
using namespace sf;

class ParticleComponent : public IComponent, public Drawable, public Transformable {
    friend class ParticleSystem;
private:
    struct Particle{
    Vector2f velocity;
    Time lifetime;
    };

    vector<Particle> _Particles;
    VertexArray _Vertices;
    Time _Lifetime;
    Vector2f _Center;
    unsigned int _MaxVelocity = 100;
    unsigned int _MinVelocity = 50;
    unsigned int _MaxAngle = 360;
    unsigned int _MinAngle = 0;
    float _Radius = 0;

    virtual void draw(RenderTarget& target, RenderStates states) const {
        states.transform *= getTransform();
        states.texture = nullptr;
        target.draw(_Vertices, states);
    }

public:

    ParticleComponent(unsigned int numParticles, unsigned int lifetime = 3) :
        IComponent("Particle"),
        _Particles(numParticles),
        _Vertices(Points, numParticles),
        _Lifetime(seconds(lifetime)),
        _Center(0, 0) { }

    void setCenter(Vector2f position) {
        _Center = position;
    }

    void setMaxVel(unsigned int maxVel) {
        if(maxVel >= _MinVelocity) {
            _MaxVelocity = maxVel;
        }
    }

    void setMinVel(unsigned int minVel) {
        if(minVel <= _MaxVelocity) {
            _MinVelocity = minVel;
        }
    }

    void setMaxAngle(unsigned int maxAng) {
        if(maxAng >= _MinAngle) {
            _MaxAngle = maxAng;
        }
    }

    void setMinAngle(unsigned int minAng) {
        if(minAng <= _MaxAngle) {
            _MinAngle = minAng;
        }
    }

    void setParticleColor(unsigned int r, unsigned int g, unsigned int b) {
        r = static_cast<Uint8>(r);
        g = static_cast<Uint8>(g);
        b = static_cast<Uint8>(b);
        size_t numParticles = _Vertices.getVertexCount();
        for(size_t i = 0; i < numParticles; ++i) {
            _Vertices[i].color.r = r;
            _Vertices[i].color.g = g;
            _Vertices[i].color.b = b;
        }
    }
};
