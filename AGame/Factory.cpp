#include "Factory.h"
#include "ResourceManager.h"
#include "CSHeaderDef.h"
#include <assert.h>

Factory& Factory::Instance()
{
    static Factory instance;
    return instance;
}

void Factory::RemoveEntity(Entity* entity)
{
    *entity = _entities.back();
    _entities.erase(_entities.end() - 1);
}

Entity& Factory::GetEntity(const int& id)
{
    assert(id < _unique_ids);
    return _entities[id];
}

void Factory::Free()
{
    _unique_ids = 0;
    _entities = std::vector<Entity>();
}

eid Factory::FF_Sprite(const SpriteData& data, const float& x, const float& y)
{
    eid id = CreateEntity<Com_Position, Com_Sprite>();
    Entity& e = Factory::Instance()[id];
    Com_Sprite& sprite = e.Get<Com_Sprite>();
    // gets texture and mesh resources from resource manager
    ResourceManager::Instance().GetResource(sprite._texture, sprite._mesh, data.texturename, data.row, data.col, data.frames);
    sprite._x_scale = data.scalex;
    sprite._y_scale = data.scaley;
    sprite._row = data.row;
    sprite._col = data.col;
    sprite._frames = data.frames;
    sprite._frame_interval = data.interval;

    e.Get<Com_Position>() = { x,y };

    return id;
}

eid Factory::FF_Tilemap(const std::string& texture, const std::string& bottom, const std::string& top)
{
    eid id = CreateEntity<Com_Tilemap, Com_Position, Com_ArrowKeys>();
    Entity& e = Factory::Instance()[id];
    Com_Tilemap& tilemap = e.Get<Com_Tilemap>();
    ResourceManager::Instance().GetResource(tilemap._texture, tilemap._mesh, texture, 4, 4, 16);
    ResourceManager::Instance().ReadTilemapTxt(top, tilemap);
    ResourceManager::Instance().ReadFloorMapTxt(bottom, tilemap);
    tilemap._scale_x = 50.0f;
    tilemap._scale_y = 50.0f;
    tilemap._initialized = true;
    return id;
}

eid Factory::FF_SpriteTile(const SpriteData& data, const eid& tilemap, const int& x, const int& y)
{
    eid id = CreateEntity<Com_Position, Com_Sprite, Com_TilePosition, Com_TilemapRef, Com_ArrowKeysTilemap>();
    Entity& e = Factory::Instance()[id];
    // gets texture and mesh resources from resource manager
    Com_Sprite& sprite = e.Get<Com_Sprite>();
    ResourceManager::Instance().GetResource(sprite._texture, sprite._mesh, data.texturename, data.row, data.col, data.frames);
    sprite._x_scale = data.scalex;
    sprite._y_scale = data.scaley;
    sprite._row = data.row;
    sprite._col = data.col;
    sprite._frames = data.frames;
    sprite._frame_interval = data.interval;

    e.Get<Com_TilePosition>() = { x,y,x,y };
    Com_Tilemap& temp = Factory::Instance().GetEntity(tilemap).Get<Com_Tilemap>();
    Factory::Instance().GetEntity(id).Get<Com_TilemapRef>()._tilemap = &Factory::Instance().GetEntity(tilemap).Get<Com_Tilemap>();
    return id;
}

//int Factory::FF_SpriteRandomPosition(const std::string& texturename, const int& row, const int& col, const int& frames, const float& interval, const float& scalex, const float& scaley,
//    const float& x, const float& y, const float& velX, const float& velY)
//{
//    eid id = CreateEntity<Com_Position, Com_Sprite>();
//    Entity& e = Factory::Instance()[id];
//    e.Initialize<Com_Position, Com_Sprite, Com_Velocity>();
//    // gets texture and mesh resources from resource manager
//    ResourceManager::Instance().GetResource(e.Get<Com_Sprite>()._texture, e.Get<Com_Sprite>()._mesh, texturename, row, col, frames);
//    e.Get<Com_Sprite>()._x_scale = scalex;
//    e.Get<Com_Sprite>()._y_scale = scaley;
//    e.Get<Com_Sprite>()._row = row;
//    e.Get<Com_Sprite>()._col = col;
//    e.Get<Com_Sprite>()._frames = frames;
//    e.Get<Com_Sprite>()._frame_interval = interval;
//
//    e.Get<Com_Position>().x = x;
//    e.Get<Com_Position>().y = y;
//
//    e.Get<Com_Velocity>().x = velX;
//    e.Get<Com_Velocity>().y = velY;
//
//    
//    return id;
//    
//    /*int e = FF_Sprite(texturename, row, col, frames, interval, scalex, scaley);
//    GetEntity(e).AddComponent<Com_Example_Velocity>();
//    e.Get<Com_Position>().x = x;
//    e.Get<Com_Position>().y = y;
//    return e;
//    */
//}
