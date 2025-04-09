#pragma once
#include "../GameObjectBase.h"
#include <vector>
#include <memory>

namespace ETG
{
    // Forward declaration
    class BulletMan;

    class Scene : public GameObjectBase
    {
    public:
        Scene();
        ~Scene() override = default;
        void Initialize() override;
        void Update() override;
        void Draw() override;
        void PopulateSpecificWidgets() override;

        // Method to spawn BulletMan (implementation in .cpp)
        void SpawnBulletMan(float x, float y);

    private:
        // Coordinates for spawning enemies
        float spawnX = 0.0f;
        float spawnY = 0.0f;
        
        // Container for all spawned enemies
        std::vector<std::unique_ptr<BulletMan>> enemies;
        
        BOOST_DESCRIBE_CLASS(Scene,(GameObjectBase),
            (spawnX, spawnY),
            (),
            ())
    };
}