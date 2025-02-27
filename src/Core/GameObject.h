#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

namespace ETG
{
    class GameObject
    {

        struct DrawProperties
        {
            sf::Vector2f Position{0, 0};
            sf::Vector2f Scale{0, 0};
            sf::Vector2f Origin{0, 0};
            float Rotation{};
            float Depth{};
        };

    protected:
        //Push back every GameObject to the SceneObj during initialization.  
        GameObject();

        virtual ~GameObject() = default;

        virtual void Initialize()
        {
        }

        virtual void Draw()
        {
        }

        virtual void Update()
        {
            //Calculate the final drawing properties. The Base position modified from the source and relative pos given in the UI will be summed to form the final value before being drawn
            ComputeDrawProperties();
        }

        std::string ObjectName{"Default"};
        sf::Texture Texture;

        //Base position of GameObjects
        //Inherited Objects such as Gun's position will be attached to hand pos in tick. After the object manipulations are completed, the relative offsets needs given in UI needs to be applied
        //and result will be stored in FinalPos, FinalRot etc. Final properties will be drawn.    
        sf::Vector2f Position{0, 0};
        sf::Vector2f Scale = {1, 1};
        float Rotation{};
        sf::Vector2f Origin{0.f, 0.f};
        float Depth{};

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

        

    public:

        //Owner //TODO: So tired to make this shit private, give friend bullshits and write getter setter
        GameObject* Owner = nullptr;
        
        std::string SetObjectNameToSelfClassName();

        // Only the drawing code (or renderer) is expected to use these values.
        [[nodiscard]] const DrawProperties& GetDrawProperties() const { return DrawProps; }
        virtual std::string& GetObjectName() { return ObjectName; }

        const GameObject* GetOwner() const { return Owner; }

        [[nodiscard]] const sf::Vector2f& GetPosition() const { return Position; }
        [[nodiscard]] const sf::Vector2f& GetScale() const { return Scale; }
        [[nodiscard]] const sf::Vector2f& GetOrigin() const { return Origin; }

        void SetPosition(const sf::Vector2f& Position) { this->Position = Position; }
        void SetScale(const sf::Vector2f& Scale) { this->Scale = Scale; }
        void SetOrigin(const sf::Vector2f& Origin) { this->Origin = Origin; }

        void ComputeDrawProperties();

        //If same named object constructed before, differentiate it with appending a number end of the name
        //ex: BaseProjectile BaseProjectile2 BaseProjectile3 
        void IncrementName();

        //Friend classes for Engine UI
        friend void ImGuiSetRelativeOrientation(GameObject* obj);
        friend void ImGuiSetAbsoluteOrientation(GameObject* obj);
    };
}
