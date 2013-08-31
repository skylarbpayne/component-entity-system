/**
    * The MovementSystem updates PositionComponents
    *
    * Author: Skylar Payne
    * Date: 8/3/2013
    * File: MovementSystem.h
**/

#pragma once

#include "IListener.h"
#include "ISystem.h"

class MovementSystem : public ISystem, public IListener<MoveEntityMessage>, public IListener<PushEntityMessage>
{
public:
    MovementSystem() : ISystem("Movement") { }
    void Update(sf::Time dt) override;
    bool ValidateEntity(unsigned int ID) override;

    void OnMessage(MoveEntityMessage& msg) override;
    void OnMessage(PushEntityMessage& msg) override;
};
