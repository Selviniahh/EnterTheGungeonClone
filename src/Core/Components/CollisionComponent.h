#pragma once
#include "../ComponentBase.h"
#include "../Events/EventDelegate.h"

namespace ETG
{
    class GameObjectBase;
    class CollisionEventData;

    class CollisionComponent : public ComponentBase
    {
    public:
        CollisionComponent();
        ~CollisionComponent() override;

        void Initialize() override;
        void Update() override;

        //Radius to expand collision box beyond the texture boundaries
        float CollisionRadius = 0.0f;

        //Whether to show collision bounds for debugging
        bool ShowCollisionBounds = true;

        //Color for collision visualiztion
        sf::Color CollisionVisualizationColor = sf::Color::Yellow;

        //Get the current collision bounds (including radius)
        sf::FloatRect GetCollisionBounds() const;

        //Events for collision
        EventDelegate<CollisionEventData> OnCollisionEnter;
        EventDelegate<CollisionEventData> OnCollisionStay;
        EventDelegate<CollisionEventData> OnCollisionExit;

        //Draw debug visualization
        void DrawDebug(sf::RenderWindow& window);

        //Get collision registry (all activecollision components)
        static std::vector<CollisionComponent*>& GetRegistry();

        void SetCollisionEnabled(bool enabled);
        bool IsCollisionEnabled() const { return CollisionEnabled; }
        void PopulateSpecificWidgets() override;

    private:
        //Cache the owner's bounds + radius
        sf::FloatRect ExpandedBounds;

        //Track which objects we are currently colldiing with
        std::unordered_map<CollisionComponent*, bool> CurrentCollisions;

        //Registry of all active collision components. To see the owner of any element look at: otherComp->ComponentBase->GameObjectBase->Owner 
        static std::vector<CollisionComponent*> AllCollisionRegistries;

        //Whether this collision component is active
        bool CollisionEnabled{};

        //Update internal bounds cache
        void UpdateBounds();

        //Check colision with another component
        bool CheckCollision(const CollisionComponent* other) const;
    private:
        BOOST_DESCRIBE_CLASS(CollisionComponent, (ComponentBase), (CollisionRadius, ShowCollisionBounds, CollisionEnabled, CollisionVisualizationColor), (), ()) //TODO: Define color in template specialization GameEngineUI
    };

    struct CollisionEventData
    {
        GameObjectBase* Self = nullptr; //The object that owns this collision component
        GameObjectBase* Other = nullptr; //The object that collided with this one 
        CollisionComponent* OtherComp = nullptr; //The collision component of the other object

        CollisionEventData(GameObjectBase* self, GameObjectBase* other, CollisionComponent* otherComp) : Self(self), Other(other), OtherComp(otherComp) {}
    };
}
