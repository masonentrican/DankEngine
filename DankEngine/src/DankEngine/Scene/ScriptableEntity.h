#pragma once

#include "Entity.h"

namespace Dank
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return _entity.GetComponent<T>();
		}

	private:
		Entity _entity;
		friend class Scene;
	};
}

