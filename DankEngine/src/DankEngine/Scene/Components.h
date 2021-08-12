#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

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
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
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