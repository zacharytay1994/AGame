#include "Factory.h"
#include "ResourceManager.h"
#include "CSHeaderDef.h"

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

void Factory::FF_CreateBasicSprite(const std::string& texturename, const std::string& meshname)
{
    _entities.emplace_back();
    Entity& e = _entities.back();
    e.Initialize<Com_Position,Com_Sprite,Com_ArrowKeys>();
    ResourceManager::Instance().GetResource(e.Get<Com_Sprite>()._texture, e.Get<Com_Sprite>()._mesh, texturename, meshname);
    e.Get<Com_Sprite>()._x_scale = 50.0f;
    e.Get<Com_Sprite>()._y_scale = 50.0f;
}
