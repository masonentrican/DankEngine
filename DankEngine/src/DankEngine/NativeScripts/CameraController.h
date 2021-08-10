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
            auto& transform = GetComponent<TransformComponent>().Transform;
            float speed = 5.0f;

            if (Input::IsKeyPressed(DANK_KEY_A))
                transform[3][0] -= speed * ts;
            if (Input::IsKeyPressed(DANK_KEY_D))
                transform[3][0] += speed * ts;
            if (Input::IsKeyPressed(DANK_KEY_W))
                transform[3][1] += speed * ts;
            if (Input::IsKeyPressed(DANK_KEY_S))
                transform[3][1] -= speed * ts;
        }

    };
}