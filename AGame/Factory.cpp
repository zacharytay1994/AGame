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
    ResourceManager::Instance().GetResource(sprite._render_pack._texture, sprite._render_pack._mesh, data._texturename, data._row, data._col, data._frames);
    sprite._x_scale = data._scalex;
    sprite._y_scale = data._scaley;
    sprite._row = data._row;
    sprite._col = data._col;
    sprite._frames = data._frames;
    sprite._frame_interval = data._interval;
    sprite._render_pack._layer = data._layer;

    e.Get<Com_Position>() = { x,y };

    return id;
}

eid Factory::FF_Tilemap(const std::string& texture, const std::string& bottom, const std::string& top)
{
    eid id = CreateEntity<Com_Tilemap, Com_Position>();
    Entity& e = Factory::Instance()[id];
    Com_Tilemap& tilemap = e.Get<Com_Tilemap>();
    ResourceManager::Instance().GetResource(tilemap._render_pack._texture, tilemap._render_pack._mesh, texture, 4, 4, 16);
    ResourceManager::Instance().ReadTilemapTxt(top, tilemap);
    ResourceManager::Instance().ReadFloorMapTxt(bottom, tilemap);
    tilemap._scale_x = 50.0f;
    tilemap._scale_y = 50.0f;
    tilemap._initialized = true;
    return id;
}


eid Factory::FF_SpriteTile(const SpriteData& data, const eid& tilemap, const int& x, const int& y)
{
    eid id = FF_Sprite(data, 0.0f, 0.0f);
    Factory::Instance()[id].AddComponent<Com_TilePosition, Com_TilemapRef, Com_Direction>();
    Entity& e = Factory::Instance()[id];
    e.Get<Com_TilePosition>() = { x,y,x,y };
    e.Get<Com_TilemapRef>()._tilemap = &Factory::Instance()[tilemap].Get<Com_Tilemap>();
    return id;
}

eid Factory::FF_CreateGUISurface(const SpriteData& data, const float& x, const float& y, const float& width, const float& height, int layer)
{
    eid id = FF_Sprite(data, 0.0f, 0.0f);
    Entity& e = Factory::Instance()[id].AddComponent<Com_GUISurface>();
    Com_GUISurface& surface = e.Get<Com_GUISurface>();;
    Com_Sprite& sprite = e.Get<Com_Sprite>();
    surface._position = { x,y };
    surface._dimensions = { width, height };
    surface._ph_dimensions = { width * (float)AEGetWindowWidth() / 2.0f, height * (float)AEGetWindowHeight() / 2.0f };
    surface._layer = layer;
    sprite._x_scale = AEGetWindowWidth() * width;
    sprite._y_scale = AEGetWindowHeight() * height;
    sprite._render_pack._layer = layer;
    return id;
}

eid Factory::FF_CreateGUISurfaceText(const SpriteData& data, const float& x, const float& y, const float& width, const float& height, const std::string& text, const std::string& font, int layer)
{
    eid id = FF_CreateGUISurface(data, x, y, width, height, layer);
    Entity& e = Factory::Instance()[id].AddComponent<Com_Text>();
    Com_Text& com_text = e.Get<Com_Text>();
    com_text._data._text = text;
    com_text._data._font = ResourceManager::Instance().GetFont(font);
    com_text._data._layer = layer + 1;
    return id;
}

eid Factory::FF_CreateGUIClickableSurface(const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*), int layer)
{
    eid id = FF_CreateGUISurface(data, x, y, width, height, layer);
    Entity& e = Factory::Instance()[id].AddComponent<Com_GUIOnClick, Com_GUIMouseCheck>();
    e.Get<Com_GUIOnClick>()._click_event = onclick;
    return id;
}

eid Factory::FF_CreateGUIClickableSurfaceText(const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*), const std::string& text, const std::string& font, int layer)
{
    eid id = FF_CreateGUIClickableSurface(data, x, y, width, height, onclick, layer);
    Entity& e = Factory::Instance()[id].AddComponent<Com_Text>();
    Com_Text& com_text = e.Get<Com_Text>();
    com_text._data._text = text;
    com_text._data._font = ResourceManager::Instance().GetFont(font);
    com_text._data._layer = layer + 1;
    return id;
}

eid Factory::FF_CreateGUIChildSurface(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height)
{
    Com_GUISurface& parent_surface = Factory::Instance()[parent].Get<Com_GUISurface>();
    eid id = FF_CreateGUISurface(data, x, y, parent_surface._dimensions.x*width, parent_surface._dimensions.y*height, parent_surface._layer+1);
    // set parent surface
    Factory::Instance()[id].Get<Com_GUISurface>()._parent_surface = &parent_surface;
    Factory::Instance()[id].Get<Com_GUISurface>()._parent_position = &Factory::Instance()[parent].Get<Com_Position>();
    Factory::Instance()[id].Get<Com_Sprite>()._render_pack._layer = Factory::Instance()[parent].Get<Com_Sprite>()._render_pack._layer+1;
    return id;
}

eid Factory::FF_CreateGUIChildSurfaceText(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height, const std::string& text, const std::string& font)
{
    eid id = FF_CreateGUIChildSurface(parent, data, x, y, width, height);
    Entity& e = Factory::Instance()[id].AddComponent<Com_Text>();
    Com_Text& com_text = e.Get<Com_Text>();
    com_text._data._text = text;
    com_text._data._font = ResourceManager::Instance().GetFont(font);
    com_text._data._layer = Factory::Instance()[parent].Get<Com_GUISurface>()._layer + 2;
    return id;
}

eid Factory::FF_CreateGUIChildClickableSurface(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*))
{
    eid id = FF_CreateGUIChildSurface(parent, data, x, y, width, height);
    Entity& e = Factory::Instance()[id].AddComponent<Com_GUIOnClick, Com_GUIMouseCheck>();
    e.Get<Com_GUIOnClick>()._click_event = onclick;
    return id;
}

eid Factory::FF_CreateGUIChildClickableSurfaceText(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*), const std::string& text, const std::string& font)
{
    eid id = FF_CreateGUIChildClickableSurface(parent, data, x, y, width, height, onclick);
    Entity& e = Factory::Instance()[id].AddComponent<Com_Text>();
    Com_Text& com_text = e.Get<Com_Text>();
    com_text._data._text = text;
    com_text._data._font = ResourceManager::Instance().GetFont(font);
    com_text._data._layer = Factory::Instance()[parent].Get<Com_GUISurface>()._layer + 2;
    return id;
}

eid Factory::FF_CreateGUISettings()
{
    return -1;
}

eid Factory::FF_Createproj(const SpriteData& data, const int& x, const int& y, const Com_Direction& direction)
{
    eid id = FF_Sprite(data, x, y);
    //for the projectile not the entity calling it 
    Factory::Instance()[id].AddComponent<Com_WeaponAttack,Com_Velocity>();
    Entity& e = Factory::Instance()[id];
    //setting of velocity which is not initialized 
    //Com_Direction& direction = e.Get<Com_Direction>();
    Com_Velocity& velocity = e.Get<Com_Velocity>();
	//updates the velocity 
	if (direction.currdir == direction.left) {
        velocity.x = -100.0f;
        velocity.y = 0.0f;
	}
	if (direction.currdir == direction.right) {
        velocity.x = 100.0f;
        velocity.y = 0.0f;
	}
	if (direction.currdir == direction.up) {
        velocity.x = 0.0f;
        velocity.y = 100.0f;
	}
	if (direction.currdir == direction.down) {
        velocity.x = 0.0f;
        velocity.y = -100.0f;
	}
    return id;
}


eid Factory::FF_CreateEnemy(const SpriteData& data, const eid& tilemap, const int& x, const int& y) {
    eid id = FF_Sprite(data, 0.0f, 0.0f);
    Factory::Instance()[id].AddComponent<Com_TilePosition, Com_TilemapRef, Com_Direction,Com_EnemySpawn>();
    Entity& e = Factory::Instance()[id];
    e.Get<Com_TilePosition>() = { x,y,x,y };
    e.Get<Com_TilemapRef>()._tilemap = &Factory::Instance()[tilemap].Get<Com_Tilemap>();
    return id;
}

//eid Factory::FF_SpriteRandomPosition(const SpriteData& data, const float& x, const float& y, const float& velX, const float& velY)
//{
//    eid id = CreateEntity<Com_Position, Com_Sprite, Com_Velocity>();
//    Entity& e = Factory::Instance()[id];
//    Com_Sprite& sprite = e.Get<Com_Sprite>();
//    // gets texture and mesh resources from resource manager
//    ResourceManager::Instance().GetResource(sprite._texture, sprite._mesh, data.texturename, data.row, data.col, data.frames);
//    sprite._x_scale = data.scalex;
//    sprite._y_scale = data.scaley;
//    sprite._row = data.row;
//    sprite._col = data.col;
//    sprite._frames = data.frames;
//    sprite._frame_interval = data.interval;
//
//    e.Get<Com_Position>() = { x,y };
//    e.Get<Com_Velocity>() = { velX,velY };
//
//    return id;
//}

