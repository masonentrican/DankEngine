#include "dankpch.h"
#include "Entity.h"

namespace Dank {

	Entity::Entity(entt::entity handle, Scene* scene) : _entityHandle(handle), _scene(scene) {}

}