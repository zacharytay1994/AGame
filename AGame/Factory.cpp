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
    _entities.back().Initialize<Position,Sprite>();
    ResourceManager::Instance().GetResource(_entities.back().Get<Sprite>()._texture, _entities.back().Get<Sprite>()._mesh, texturename, meshname);
}
