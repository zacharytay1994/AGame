#include "zEntity.h"
#include "Factory.h"

void Entity::Destroy()
{
	// remove from chunk
	_chunk->Remove(_id);
	// remove from factory
	Factory::Instance().RemoveEntity(this);
}
