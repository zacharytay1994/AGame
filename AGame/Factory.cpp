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

int Factory::FF_Sprite(const std::string& texturename, const int& row, const int& col, const int& frames, const float& interval, const float& scalex, const float& scaley)
{
    _entities.emplace_back();
    Entity& e = _entities.back();
    e.Initialize<Com_Position,Com_Sprite>();
    // gets texture and mesh resources from resource manager
    ResourceManager::Instance().GetResource(e.Get<Com_Sprite>()._texture, e.Get<Com_Sprite>()._mesh, texturename, row, col, frames);
    e.Get<Com_Sprite>()._x_scale = scalex;
    e.Get<Com_Sprite>()._y_scale = scaley;
    e.Get<Com_Sprite>()._row = row;
    e.Get<Com_Sprite>()._col = col;
    e.Get<Com_Sprite>()._frames = frames;
    e.Get<Com_Sprite>()._frame_interval = interval;

    ++_unique_ids;
    return (int)_unique_ids-1;
}

int Factory::FF_Tilemap(const std::string& texture, const std::string& bottom, const std::string& top)
{
    Entity& e = CreateEntity<Com_Tilemap>();
    ResourceManager::Instance().GetResource(e.Get<Com_Tilemap>()._texture, e.Get<Com_Tilemap>()._mesh, texture, 4, 4, 16);
    ResourceManager::Instance().ReadTilemapTxt(top, e.Get<Com_Tilemap>());
    ResourceManager::Instance().ReadFloorMapTxt(bottom, e.Get<Com_Tilemap>());
    Com_Tilemap& tilemap = e.Get<Com_Tilemap>();
    e.Get<Com_Tilemap>()._scale_x = 50.0f;
    e.Get<Com_Tilemap>()._scale_y = 50.0f;
    e.Get<Com_Tilemap>()._initialized = true;
    ++_unique_ids;
    return _unique_ids - 1;
}

int Factory::FF_SpriteRandomPosition(const std::string& texturename, const int& row, const int& col, const int& frames, const float& interval, const float& scalex, const float& scaley,
    const float& x, const float& y)
{
    _entities.emplace_back();
    Entity& e = _entities.back();
    e.Initialize<Com_Position, Com_Sprite, Com_Example_Velocity>();
    // gets texture and mesh resources from resource manager
    ResourceManager::Instance().GetResource(e.Get<Com_Sprite>()._texture, e.Get<Com_Sprite>()._mesh, texturename, row, col, frames);
    e.Get<Com_Sprite>()._x_scale = scalex;
    e.Get<Com_Sprite>()._y_scale = scaley;
    e.Get<Com_Sprite>()._row = row;
    e.Get<Com_Sprite>()._col = col;
    e.Get<Com_Sprite>()._frames = frames;
    e.Get<Com_Sprite>()._frame_interval = interval;

    e.Get<Com_Position>().x = x;
    e.Get<Com_Position>().y = y;

    e.Get<Com_Example_Velocity>().x = 1.0f;
    e.Get<Com_Example_Velocity>().y = 1.0f;

    ++_unique_ids;
    return (int)_unique_ids - 1;
    
    /*int e = FF_Sprite(texturename, row, col, frames, interval, scalex, scaley);
    GetEntity(e).AddComponent<Com_Example_Velocity>();
    e.Get<Com_Position>().x = x;
    e.Get<Com_Position>().y = y;
    return e;
    */
}
