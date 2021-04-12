/******************************************************************************/
/*!
\file		zEntity.cpp
\author 	Zachary Tay
\par    	email: z.tay\@digipen.edu
\date   	April 12, 2021
\brief		Entity for ECS

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
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
