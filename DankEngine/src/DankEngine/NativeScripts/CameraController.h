#pragma once
#include "dankpch.h"
#include "Dank.h"

#include "DankEngine/Core/Input.h"
#include "DankEngine/Scene/ScriptableEntity.h"
#include "DankEngine/Scene/Components.h"

namespace Dank
{
    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
        }

        void OnDestroy()
        {
        }

        void OnUpdate(Timestep ts)
        {
            auto& position = GetComponent<TransformComponent>().Position;
            float speed = 5.0f;

            if (Input::IsKeyPressed(DANK_KEY_A))
                position.x -= speed * ts;
            if (Input::IsKeyPressed(DANK_KEY_D))
                position.x += speed * ts;
            if (Input::IsKeyPressed(DANK_KEY_W))
                position.y += speed * ts;
            if (Input::IsKeyPressed(DANK_KEY_S))
                position.y -= speed * ts;
        }

    };
}