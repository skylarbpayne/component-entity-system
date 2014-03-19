/**
    * TestScene is the hub to test all code changes.
    *
    * Author: Skylar Payne
    * Date: 7/31/2013
    * File: TestScene.h
**/

#include "TestScene.h"

#include <SFML/Graphics.hpp>
#include "IListener.h"

#include <lua.hpp>
#include "LuaBindings.h"
#include "Logger.h"

#include "PositionComponent.h"
#include "MovementComponent.h"
#include "MovementSystem.h"
#include "CircleComponent.h"
#include "RectangleComponent.h"
#include "SpriteComponent.h"
#include "TextComponent.h"
#include "ParticleComponent.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "ScriptableBehavior.h"
#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "BehaviorSystem.h"
#include "ParticleSystem.h"

bool TestScene::Load()
{
    RenderSystem* rs = new RenderSystem();
    sm.Add(rs);
    MovementSystem* ms = new MovementSystem();
    sm.Add(ms);
    CollisionSystem* cs = new CollisionSystem(sf::Vector2f(this->GetWindow()->getSize()), sf::Vector2f(200, 200));
    sm.Add(cs);
    BehaviorSystem* bs = new BehaviorSystem();
    sm.Add(bs);
    ParticleSystem* ps = new ParticleSystem();
    sm.Add(ps);

    ef.Register("Position", []() { return new PositionComponent(); });
    ef.Register("Movement", []() { return new MovementComponent(); });
    ef.Register("Collider", []() { return new ColliderComponent(); });
    ef.Register("Circle", []() { return new CircleComponent(); });
    ef.Register("Rectangle", []() { return new RectangleComponent(); });
    ef.Register("Sprite", []() { return new SpriteComponent(); });
    ef.Register("Text", []() { return new TextComponent(); });
    //This will be a tad trickier to get lua integration with. Because of the memory allocation involved.
    //ef.Register("Particle", []() { return new ParticleComponent();})

    rm.AddFont("Lorena.ttf");
    rm.AddTexture("spaceship.png");

    ef.Create("entity2.lua", 50, 50);
    //ef.Create("entity.lua", 400, 400);

    Entity* e = new Entity("particleman");
    IComponent* ic = new PositionComponent(300, 200);
    e->AttachComponent(ic);
    ic = new MovementComponent();
    e->AttachComponent(ic);
    ic = new ColliderComponent(0, 0, 20, 20);
    e->AttachComponent(ic);
    ic = new CircleComponent();
    e->AttachComponent(ic);
    ic = new ParticleComponent(10000, 0, sf::Vector2f(50.f, 50.f));
    e->AttachComponent(ic);
    ScriptableBehavior* sb = new ScriptableBehavior("move", "move.lua");
    e->AttachBehavior(sb);

    AddEntityMessage msg;
    msg.entity = e;
    Emit<AddEntityMessage>(msg);

    EntityMovedMessage mmsg;
    mmsg.ID = e->GetID();
    mmsg.newPosition = mmsg.oldPosition = sf::Vector2f(300, 200);
    Emit<EntityMovedMessage>(mmsg);

    return true;
}

void TestScene::Update()
{
    sf::Event event;
    while(this->GetWindow()->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            ExitMessage msg;
            msg.ExitStatus = 0;
            Emit<ExitMessage>(msg);
        }

        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Q)
            {
                lua_State* L = luaL_newstate();
                SetBindings(L);
                if(luaL_dofile(L, "luatest.lua"))
                {
                    g_Logger << lua_tostring(L, -1) << "\n";
                    lua_pop(L, 1);
                }
                lua_close(L);
            }
        }
    }

    this->GetWindow()->clear();
    em.Update();
    sm.Update();
    this->GetWindow()->display();
}

void TestScene::Unload()
{
    rm.Unload();
}
