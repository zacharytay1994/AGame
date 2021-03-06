/******************************************************************************/
/*!
\File Name		: zEntity.cpp
\Project Name	: AGame
\Authors 		:
				Primary - Zachary Tay (100%)
				Secondary -
\brief		Entity for ECS

All content � 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/

#include "zEntity.h"
#include "Factory.h"

/*______________________________________________________
	* Brief:	Destroy this instance.
	*
	* Access:	public
	*
	* Use e.g:	Entity& e1 = Factory::CreateEntity<Com_Position>();
	*			e1.Destroy();
________________________________________________________*/
void Entity::Destroy()
{
	// remove from chunk
	_chunk->Remove(_id);
	// remove from factory
	Factory::Instance().RemoveEntity(this);
}

void Entity::FreeData()
{
	// remove from chunk
	_chunk->Remove(_id);
}
