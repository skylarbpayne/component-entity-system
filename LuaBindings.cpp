/**
    * LuaBindings contains function declarations regarding lua binding.
    *
    * Author: Skylar Payne
    * Date: 8/1/2013
    * File: LuaBindings.h
**/

#include "LuaBindings.h"
#include "IListener.h"
#include "WindowAccessor.h"
#include "EntityAccessor.h"
#include "PositionComponent.h"
#include "MovementComponent.h"
#include "Entity.h"

#include "Logger.h"

/**
 * @brief The LuaAccessor struct allows Lua to gain access to window access data.
 */
struct LuaAccessor : public WindowAccessor, public EntityAccessor
{
    sf::RenderWindow& GetWindow() const { return *WindowAccessor::GetWindow(); }
    Entity* GetEntity(unsigned int ID) const { return EntityAccessor::GetEntity(ID); }
};

extern "C"
{

/**
 * @brief exit Lets lua emit an exit message
 * @param L the calling lua thread
 * @return no return value if successful, error message if failed
 */
static int Exit(lua_State* L)
{
    if(lua_gettop(L) != 1)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    ExitMessage msg;
    msg.ExitStatus = lua_tonumber(L, 1);
    Emit<ExitMessage>(msg);
    return 0;
}

/**
 * @brief CreateEntity lets lua emit a CreateEntityMessage
 * @param L the calling lua thread
 * @return no return value if successful, error message if failed
 */
static int CreateEntity(lua_State* L)
{
    if(lua_gettop(L) != 3)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    CreateEntityMessage msg;
    msg.script = lua_tostring(L, 1);
    msg.position.x = lua_tonumber(L, 2);
    msg.position.y = lua_tonumber(L, 3);
    Emit<CreateEntityMessage>(msg);
    return 0;
}

/**
 * @brief DestroyEntity lets lua emit a DestroyEntityMessage
 * @param L the calling lua thread
 * @return no return value if successful, error message if failed
 */
static int DestroyEntity(lua_State* L)
{
    if(lua_gettop(L) != 1)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    DestroyEntityMessage msg;
    msg.ID = lua_tonumber(L, 1);
    Emit<DestroyEntityMessage>(msg);
    return 0;
}

/**
 * @brief Wait pauses the program for a certain number of milliseconds
 * @param L the calling lua thread
 * @return no return value
 */
static int Wait(lua_State* L)
{
    if(lua_gettop(L) != 1)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    sf::sleep(sf::milliseconds(lua_tonumber(L, -1)));
    return 0;
}

/**
 * @brief GetWindowSize gets the dimensions of the window
 * @param L the calling lua thread
 * @return the width and height of the window, otherwise an error message
 */
static int GetWindowSize(lua_State* L)
{
    if(lua_gettop(L) != 0)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    LuaAccessor la;

    lua_pushnumber(L, la.GetWindow().getSize().x);
    lua_pushnumber(L, la.GetWindow().getSize().y);
    return 2;
}

/**
 * @brief GetKeyState gets the key state of a certain key
 * @param L the calling lua thread
 * @return true if key was pressed, false if not pressed, or error message if failed
 */
static int isKeyPressed(lua_State* L)
{
    if(lua_gettop(L) != 1)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(lua_tointeger(L, 1));
    lua_pushboolean(L, sf::Keyboard::isKeyPressed(key));
    return 1;
}

/**
 * @brief GetCursorPosition gets the position of the on screen cursor
 * @param L the calling lua thread
 * @return 2 return values: x position and y position of cursor or error message if failed
 */
static int GetCursorPosition(lua_State* L)
{
    if(lua_gettop(L) != 0)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    LuaAccessor la;
    lua_pushnumber(L, sf::Mouse::getPosition(la.GetWindow()).x);
    lua_pushnumber(L, sf::Mouse::getPosition(la.GetWindow()).y);
    return 2;
}

/**
 * @brief GetEntityTag gets the tag of a specified entity
 * @param L the calling lua thread
 * @return the tag, if the entity exists, an error string otherwise
 */
static int GetEntityTag(lua_State* L)
{
    if(lua_gettop(L) != 1)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    LuaAccessor la;
    unsigned int id = (unsigned int)lua_tonumber(L, 1);
    Entity* e = la.GetEntity(id);

    if(e)
    {
        lua_pushstring(L, e->GetTag());
        return 1;
    }

    lua_pushstring(L, "Entity does not exist");
    return 1;
}

/**
 * @brief GetEntityPosition gets the position of a specified entity
 * @param L the calling lua thread
 * @return the entity position if available, an error string otherwise.
 */
static int GetEntityPosition(lua_State* L)
{
    if(lua_gettop(L) != 1)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    LuaAccessor la;
    unsigned int id = (unsigned int)lua_tonumber(L, 1);
    Entity* e = la.GetEntity(id);

    if(e)
    {
        PositionComponent* p = e->GetComponent<PositionComponent>("Position");

        if(p)
        {
            lua_pushnumber(L, p->GetPosition().x);
            lua_pushnumber(L, p->GetPosition().y);
            return 2;
        }

        else
        {
            lua_pushstring(L, "No position component");
            return 1;
        }
    }

    lua_pushstring(L, "Entity does not exist");
    return 1;
}

/**
 * @brief GetEntityVelocity gets a specified entity's velocity
 * @param L the calling lua thread
 * @return the velocity; 0, 0 if the entity does not have a movement component.
 */
static int GetEntityVelocity(lua_State *L)
{
    if(lua_gettop(L) != 1)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    LuaAccessor la;
    unsigned int id = (unsigned int)lua_tonumber(L, 1);
    Entity* e = la.GetEntity(id);

    if(e)
    {
        MovementComponent* m = e->GetComponent<MovementComponent>("Movement");

        if(m)
        {
            lua_pushnumber(L, m->GetVelocity().x);
            lua_pushnumber(L, m->GetVelocity().y);
        }

        else
        {
            lua_pushnumber(L, 0);
            lua_pushnumber(L, 0);
        }

        return 2;
    }

    lua_pushstring(L, "Entity does not exist");
    return 1;
}

/**
 * @brief MoveEntity Lets lua emit a MoveEntity message
 * @param L the calling lua thread
 * @return no return value if successful, error message if failed
 */
static int MoveEntity(lua_State* L)
{
    if(lua_gettop(L) != 3)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    MoveEntityMessage msg;
    msg.ID = lua_tonumber(L, 1);
    msg.newPosition.x = lua_tonumber(L, 2);
    msg.newPosition.y = lua_tonumber(L, 3);
    Emit<MoveEntityMessage>(msg);
    return 0;
}

/**
 * @brief PushEntity Lets lua emit a PushEntity message
 * @param L the calling lua thread
 * @return no return value if successful, error message if failed
 */
static int PushEntity(lua_State* L)
{
    if(lua_gettop(L) != 3)
    {
        lua_pushstring(L, "Error: incorrect number of parameters");
        return 1;
    }

    PushEntityMessage msg;
    msg.ID = lua_tonumber(L, 1);
    msg.newVelocity.x = lua_tonumber(L, 2);
    msg.newVelocity.y = lua_tonumber(L, 3);
    Emit<PushEntityMessage>(msg);
    return 0;
}
}

/**
 * @brief SetBindings loads standard lua libraries and binds the functions and data defined above
 * @param L the lua state to which to bind
 */
void SetBindings(lua_State *L)
{
    luaL_openlibs(L);

    lua_pushcfunction(L, Exit);
    lua_setglobal(L, "exit");

    lua_pushcfunction(L, CreateEntity);
    lua_setglobal(L, "create_entity");

    lua_pushcfunction(L, DestroyEntity);
    lua_setglobal(L, "destroy_entity");

    lua_pushcfunction(L, Wait);
    lua_setglobal(L, "wait");

    lua_pushcfunction(L, GetWindowSize);
    lua_setglobal(L, "get_window_size");

    lua_pushcfunction(L, GetCursorPosition);
    lua_setglobal(L, "get_cursor_position");

    lua_pushcfunction(L, isKeyPressed);
    lua_setglobal(L, "is_key_pressed");

    lua_pushcfunction(L, GetEntityTag);
    lua_setglobal(L, "get_entity_tag");

    lua_pushcfunction(L, GetEntityPosition);
    lua_setglobal(L, "get_entity_position");

    lua_pushcfunction(L, GetEntityVelocity);
    lua_setglobal(L, "get_entity_velocity");

    lua_pushcfunction(L, MoveEntity);
    lua_setglobal(L, "move_entity");

    lua_pushcfunction(L, PushEntity);
    lua_setglobal(L, "push_entity");

    lua_newtable(L);
    lua_pushliteral(L, "W");
    lua_pushnumber(L, sf::Keyboard::W);
    lua_settable(L, -3);

    lua_pushliteral(L, "S");
    lua_pushnumber(L, sf::Keyboard::S);
    lua_settable(L, -3);

    lua_pushliteral(L, "A");
    lua_pushnumber(L, sf::Keyboard::A);
    lua_settable(L, -3);

    lua_pushliteral(L, "D");
    lua_pushnumber(L, sf::Keyboard::D);
    lua_settable(L, -3);

    lua_pushliteral(L, "Up");
    lua_pushnumber(L, sf::Keyboard::Up);
    lua_settable(L, -3);

    lua_pushliteral(L, "Down");
    lua_pushnumber(L, sf::Keyboard::Down);
    lua_settable(L, -3);

    lua_pushliteral(L, "Left");
    lua_pushnumber(L, sf::Keyboard::Left);
    lua_settable(L, -3);

    lua_pushliteral(L, "Right");
    lua_pushnumber(L, sf::Keyboard::Right);
    lua_settable(L, -3);
    lua_setglobal(L, "Keys");
}
