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
    eid id = FF_Sprite(data, static_cast<float>(x), static_cast<float>(y));
    //for the projectile not the entity calling it 
    Factory::Instance()[id].AddComponent<Com_WeaponAttack,Com_Velocity,Com_Boundary, Com_objecttype, Com_CollisionData, Com_BoundingBox>();
    Entity& e = Factory::Instance()[id];
    e.Get<Com_objecttype>().objtype = Com_objecttype::bullett;
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

eid Factory::FF_Createproj2(const SpriteData& data, const int& x, const int& y, const int& vel_x, const int& vel_y, eid const& tilemap)
{
    eid id = FF_SpriteTile(data, tilemap, x, y);
    //for the projectile not the entity calling it 
    Entity& e = Factory::Instance()[id].AddComponent<Com_Projectile, Com_type, Com_GridColData>();
    Com_Projectile& proj = e.Get<Com_Projectile>();
    e.Get<Com_type>().type = 2;
    proj.grid_vel_x = vel_x;
    proj.grid_vel_y = vel_y;
    return id;
}


eid Factory::FF_CreateEnemy(const SpriteData& data, const eid& tilemap, const int& x, const int& y) {
    eid id = FF_Sprite(data, 0.0f, 0.0f);
    Factory::Instance()[id].AddComponent<Com_TilePosition, Com_TilemapRef, Com_Direction, Com_EnemySpawn, Com_Wave, Com_YLayering, Com_EnemyStateOne, Com_FindPath,Com_type,Com_GridColData>();
    Entity& e = Factory::Instance()[id];
    e.Get<Com_type>().type = 1;
    e.Get<Com_TilePosition>() = { x,y,x,y };
    e.Get<Com_TilemapRef>()._tilemap = &Factory::Instance()[tilemap].Get<Com_Tilemap>();
    return id;
}



eid Factory::FF_CreateParticle(const SpriteData& data, const int& x, const int& y,const float& velx, const float& vely) {
    float min{ 200.0f };
    float max{ 200.0f };
    float lifetimemin{ 0.0f };
    float lifetimemax(10.0f);
    eid id = FF_Sprite(data, (float)x, (float)y);
    Factory::Instance()[id].AddComponent<Com_Velocity, Com_Particle,Com_GameTimer,Com_Boundary, Com_BoundingBox>();
    Entity& e = Factory::Instance()[id];
    e.Get<Com_Velocity>().x = velx;
    e.Get<Com_Velocity>().y = vely;
    //e.Get<Com_Particle>().lifetime = lifetimemin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (lifetimemax - (lifetimemin))));
    return id;
}

eid Factory::FF_CreateBomb(const SpriteData& data, const int& x, const int& y)
{
    eid id = FF_Sprite(data, (float)x, (float)y);
    //for the projectile not the entity calling it 
    Factory::Instance()[id].AddComponent<Com_WeaponAttack, Com_Velocity, Com_Boundary, Com_ParticleEmitter,Com_GameTimer>();
    Entity& e = Factory::Instance()[id];
    //setting of velocity which is not initialized 
    return id;
}

//edits 
eid Factory::FF_CreateGUIChildClickableTextboxSurface(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*))
{
    eid id = FF_CreateGUIChildSurface(parent, data, x, y, width, height);
    Entity& e = Factory::Instance()[id].AddComponent<Com_GUIOnClick, Com_GUIMouseCheck,Com_GUItextboxinput>();
    e.Get<Com_GUIOnClick>()._click_event = onclick;
    return id;
}


eid Factory::FF_CreateGUIChildClickableSurfaceTextBox(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*), const std::string& text, const std::string& font)
{
    eid id = FF_CreateGUIChildClickableTextboxSurface(parent, data, x, y, width, height, onclick);
    Entity& e = Factory::Instance()[id].AddComponent<Com_Text>();
    Com_Text& com_text = e.Get<Com_Text>();
    com_text._data._text = text;
    com_text._data._font = ResourceManager::Instance().GetFont(font);
    com_text._data._layer = Factory::Instance()[parent].Get<Com_GUISurface>()._layer + 2;
    return id;
}


eid Factory::FF_WriteTileMap(const std::string& data, const size_t column, const size_t row)
{
    //eid id = CreateEntity<Com_Tilemap, Com_Position>();
    //Entity& e = Factory::Instance()[id];
    //Com_Tilemap& tilemap = e.Get<Com_Tilemap>();
    //ResourceManager::Instance().GetResource(tilemap._render_pack._texture, tilemap._render_pack._mesh, texture, 4, 4, 16);
    //ResourceManager::Instance().ReadTilemapTxt(top, tilemap);
    //ResourceManager::Instance().ReadFloorMapTxt(bottom, tilemap);
    //tilemap._scale_x = 50.0f;
    //tilemap._scale_y = 50.0f;
    //tilemap._initialized = true;
    //return id;
}