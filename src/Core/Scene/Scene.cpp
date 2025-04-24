#include "Scene.h"
#include "../../Core/Factory.h"
#include "../../Enemy/BulletMan/BulletMan.h"
#include <imgui.h>

namespace ETG
{
    Scene::Scene()
    {
        IsGameObjectUISpecified = true;
    }

    void Scene::Initialize()
    {
        GameObjectBase::Initialize();
        IsGameObjectUISpecified = true;

    }

    void Scene::Update()
    {
        GameObjectBase::Update();
    }

    void Scene::Draw()
    {
    }

    void Scene::SpawnBulletMan(float x, float y)
    {
        // Create a new BulletMan at the specified position
        auto newEnemy = ETG::CreateGameObjectDefault<BulletMan>(sf::Vector2f{x, y});

        // Check if creation was successful
        if (newEnemy)
        {
            // Add to enemies vector
            enemies.push_back(std::move(newEnemy));

            // Log confirmation
        }
    }

    void Scene::PopulateSpecificWidgets()
    {
        GameObjectBase::PopulateSpecificWidgets();

        // Spawn button
        if (ImGui::Button("Spawn BulletMan"))
        {
            SpawnBulletMan(spawnX, spawnY);
        }

        // Display count of active enemies
        ImGui::Text("Active enemies: %zu", enemies.size());
    }
}
