#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "glm/gtx/quaternion.hpp"

namespace Dank
{
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; //TODO: Should probably be in scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position) : Position(position) {}

		glm::mat4 GetTransform() const
		{
			// Use quats instead
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position) * rotation;
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};


	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		
		// Function pointers
		ScriptableEntity*(*Instantiate)();			// Instantiate is a function pointer of type ScriptableEntity pointer
		void (*Destroy)(NativeScriptComponent*);	// Destroy is a void function pointer of itself (NativeScriptComponent) - emulates 'this'

		template<typename T>
		void Bind()
		{			
			Instantiate = []() { return static_cast<ScriptableEntity*>(new T()); }; // Down cast to ScriptableEntity cause compiler mad
			Destroy = [](NativeScriptComponent* nativeScriptComponent)
			{
				delete nativeScriptComponent->Instance;
				nativeScriptComponent->Instance = nullptr;
			};
		}

	};
}
