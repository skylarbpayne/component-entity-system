

#include "TestScene.h"
/**
    * TestScene is the hub to test all code changes.
    *
    * Author: Skylar Payne
    * Date: 7/31/2013
    * File: TestScene.h
**/

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
#include "Entity.h"
#include "RenderSystem.h"
#include "ScriptableBehavior.h"
#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "BehaviorSystem.h"
#include "ParticleSystem.h"
#include "ParticleComponent.h"

bool TestScene::Load()
{
    RenderSystem* rs = new RenderSystem();
    sm.Add(rs);
    MovementSystem* ms = new MovementSystem();
    sm.Add(ms);
    CollisionSystem* cs = new CollisionSystem(sf::Vector2f(this->GetWindow()->getSize()), sf::Vector2f(200, 200));
    sm.Add(cs);
    ParticleSystem* ps = new ParticleSystem();
    sm.Add(ps);
    BehaviorSystem* bs = new BehaviorSystem();
    sm.Add(bs);

    ef.Register("Position", []() { return new PositionComponent(); });
    ef.Register("Movement", []() { return new MovementComponent(); });
    ef.Register("Collider", []() { return new ColliderComponent(); });
    ef.Register("Circle", []() { return new CircleComponent(); });
    ef.Register("Rectangle", []() { return new RectangleComponent(); });
    ef.Register("Sprite", []() { return new SpriteComponent(); });
    ef.Register("Text", []() { return new TextComponent(); });

    rm.AddFont("Lorena.ttf");
    rm.AddTexture("spaceship.png");

    ef.Create("entity2.lua", 50, 50);
    ef.Create("entity.lua", 400, 400);

    Entity* e = new Entity("Particles Boi");
    e->AttachComponent(new PositionComponent(100, 100));
    e->AttachComponent(new ParticleComponent(1000));
    AddEntityMessage msg;
    msg.entity = e;
    Emit<AddEntityMessage>(msg);
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
