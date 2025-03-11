#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <boost/describe.hpp>
#include <boost/mp11/algorithm.hpp>

#include "GameClass.h"
#include "../Utils/Interface/IAnimationComponent.h"

namespace ETG
{
    class GameObjectBase : public GameClass
    {
    public:
        struct DrawProperties
        {
            sf::Vector2f Position{0, 0};
            sf::Vector2f Scale{0, 0};
            sf::Vector2f Origin{0, 0};
            float Rotation{};
            float Depth{};
            sf::Texture* Texture = nullptr;
        };
    protected:
        //Push back every GameObject to the SceneObj during initialization.  
        GameObjectBase();
        virtual ~GameObjectBase();
        virtual void Initialize();
        virtual void Draw();
        virtual void Update();

        //Base position of GameObjects
        //Inherited Objects such as Gun's position will be attached to hand pos in tick. After the object manipulations are completed, the relative offsets needs given in UI needs to be applied
        //and result will be stored in FinalPos, FinalRot etc. Final properties will be drawn.    
        sf::Vector2f Position{0, 0};
        sf::Vector2f Scale = {1, 1};
        float Rotation{};
        sf::Vector2f Origin{0.f, 0.f};
        float Depth{};

        //This will be true for drawable game objects. False for components

        //NOTE: Pointer to animation component interface. This is my first time using interface logic
        IAnimationComponent* AnimInterface = nullptr;

    private:
        //Relative Offsets for GameObjects.
        sf::Vector2f RelativePos{0.f, 0.f};
        sf::Vector2f RelativeScale = {1, 1};
        float RelativeRotation{};
        sf::Vector2f RelativeOrigin{0.f, 0.f};

        //Previous Relative Offsets
        sf::Vector2f PrevRelativePos{0.f, 0.f};
        sf::Vector2f PrevRelativeScale{0.f, 0.f};
        sf::Vector2f PrevRelativeRot{0.f, 0.f};

        //Contains the final drawing properties. 
        DrawProperties DrawProps;

        //The typename without any increment
        std::string TypeName{};

        void ComputeDrawProperties();
        void VisualizeOrigin() const;
        void IncrementName();

    public:
        //Owner //TODO: So tired to make this shit private, give friend bullshits and write getter setter
        GameObjectBase* Owner = nullptr;
        bool DrawBound = false;
        bool DrawOriginPoint = true;
        bool IsGameObjectUISpecified = false;
        std::string ObjectName{"Default"};
        std::shared_ptr<sf::Texture> Texture;

        // Only the drawing code (or renderer) is expected to use these values.
        [[nodiscard]] const DrawProperties& GetDrawProperties() const { return DrawProps; }
        virtual std::string& GetObjectName() { return ObjectName; }

        [[nodiscard]] const GameObjectBase* GetOwner() const { return Owner; }

        [[nodiscard]] const sf::Vector2f& GetPosition() const { return Position; }
        [[nodiscard]] const sf::Vector2f& GetScale() const { return Scale; }
        [[nodiscard]] const sf::Vector2f& GetOrigin() const { return Origin; }

        [[nodiscard]] const sf::Vector2f& GetRelativePosition() const { return RelativePos; }
        [[nodiscard]] const sf::Vector2f& GetRelativeScale() const { return RelativeScale; }
        [[nodiscard]] const sf::Vector2f& GetRelativeOrigin() const { return RelativeOrigin; }

        sf::Vector2f& SetRelativePosition(const sf::Vector2f& Pos) { return RelativePos = Pos; }
        sf::Vector2f& SetRelativeScale(const sf::Vector2f& Scale) { return RelativeScale = Scale; }
        sf::Vector2f& SetRelativeOrigin(const sf::Vector2f& Origin) { return RelativeOrigin = Origin; }

        [[nodiscard]] const std::string& GetTypeName() const { return TypeName; }

        void SetPosition(const sf::Vector2f& Position) { this->Position = Position; }
        void SetScale(const sf::Vector2f& Scale) { this->Scale = Scale; }
        void SetOrigin(const sf::Vector2f& Origin) { this->Origin = Origin; }

        // Animation component management
        void SetAnimationInterface(IAnimationComponent* animComp) { AnimInterface = animComp; }
        IAnimationComponent* GetAnimationInterface() { return AnimInterface; } //Never used yet

        // Bounds methods
        [[nodiscard]] sf::FloatRect GetBounds() const;
        void DrawBounds(sf::Color color = sf::Color::Red) const;

        //If same named object constructed before, differentiate it with appending a number end of the name
        //ex: BaseProjectile BaseProjectile2 BaseProjectile3 
        std::string SetObjectNameToSelfClassName();

        virtual void PopulateSpecificWidgets();


        //Friend classes for Engine UI
        friend void ImGuiSetRelativeOrientation(GameObjectBase* obj);
        friend void ImGuiSetAbsoluteOrientation(GameObjectBase* obj);

        BOOST_DESCRIBE_CLASS(GameObjectBase, (GameClass),
                             (Owner, ObjectName,Texture, DrawOriginPoint, DrawBound),
                             (Origin, Depth),
                             ())
    };
}
