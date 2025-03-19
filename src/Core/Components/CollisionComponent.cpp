#include <imgui.h>
#include "CollisionComponent.h"
#include "../../Core/GameObjectBase.h"
#include "../../Managers/Globals.h"
#include "../../Managers/SpriteBatch.h"

namespace ETG
{
    std::vector<CollisionComponent*> CollisionComponent::AllCollisionRegistries;

    CollisionComponent::CollisionComponent()
    {
        AllCollisionRegistries.push_back(this);
    }

    CollisionComponent::~CollisionComponent()
    {
        const auto it = std::ranges::find(AllCollisionRegistries, this);
        if (it != AllCollisionRegistries.end())
        {
            AllCollisionRegistries.erase(it);
        }
    }

    void CollisionComponent::Initialize()
    {
        ComponentBase::Initialize();
        UpdateBounds();
    }

    void CollisionComponent::Update()
    {
        if (!CollisionEnabled || !Owner) return;

        // Update our bounds based on the owner's position and texture
        UpdateBounds();

        // Track which components we're still colliding with
        std::unordered_map<CollisionComponent*, bool> stillColliding;

        // Check for collisions with all other collision components
        for (auto* otherComp : AllCollisionRegistries)
        {
            //Skip the unappropriated ones
            if (otherComp == this || !otherComp->IsCollisionEnabled() || !otherComp->Owner)
                continue;

            const bool wasColliding = CurrentCollisions.contains(otherComp);
            const bool isColliding = CheckCollision(otherComp);

            if (isColliding)
            {
                stillColliding[otherComp] = true;

                // Handle collision events
                if (!wasColliding)
                {
                    CollisionEventData eventData(Owner, otherComp->Owner, otherComp, CalculateImpactPoint(otherComp));
                    OnCollisionEnter.Broadcast(eventData);
                }
                else
                {
                    // Last tick collided, now still colliding
                    CollisionEventData eventData(Owner, otherComp->Owner, otherComp, CalculateImpactPoint(otherComp));
                    OnCollisionStay.Broadcast(eventData);
                }
            }
            else if (wasColliding)
            {
                // Collision ended
                CollisionEventData eventData(Owner, otherComp->Owner, otherComp, CalculateImpactPoint(otherComp));
                OnCollisionExit.Broadcast(eventData);
            }
        }

        // Find collisions that ended
        for (auto& [otherComp, _] : CurrentCollisions)
        {
            if (!stillColliding.contains(otherComp))
            {
                // This collision has ended
                if (otherComp && otherComp->Owner)
                {
                    CollisionEventData eventData(Owner, otherComp->Owner, otherComp, CalculateImpactPoint(otherComp));
                    OnCollisionExit.Broadcast(eventData);
                }
            }
        }

        // Update current collisions. If stillColliding is empty, it will also remove the previous element from CurrentCollisions
        CurrentCollisions = std::move(stillColliding);
    }

    void CollisionComponent::UpdateBounds()
    {
        if (!Owner) return;

        // Get basic bounds from owner that not expanded yet. 
        const sf::FloatRect baseBounds = Owner->GetBounds();

        // Expand by radius
        ExpandedBounds = sf::FloatRect(
            baseBounds.left - CollisionRadius,
            baseBounds.top - CollisionRadius,
            baseBounds.width + (2 * CollisionRadius),
            baseBounds.height + (2 * CollisionRadius)
        );
    }

    bool CollisionComponent::CheckCollision(const CollisionComponent* other) const
    {
        if (!other) throw std::runtime_error("The object: " + other->GetOwner()->ObjectName + " not found");
        //Thankfully at least I am not have to implement intersection this time myself. 
        return ExpandedBounds.intersects(other->GetCollisionBounds());
    }

    sf::Vector2f CollisionComponent::CalculateImpactPoint(const CollisionComponent* other) const
    {
        sf::FloatRect intersection;
        sf::FloatRect otherObjBounds = other->GetCollisionBounds();

        if (ExpandedBounds.intersects(otherObjBounds, intersection))
        {
            return {
                intersection.left + intersection.width / 2.0f, //x
                intersection.top + intersection.height / 2.0f //y
            };
        }

        return {0, 0};
    }

    void CollisionComponent::Visualize(sf::RenderWindow& window)
    {
        if (!ShowCollisionBounds || !CollisionEnabled || !Owner) return;

        // Draw the original bounds and expanded bounds
        sf::FloatRect baseBounds = Owner->GetBounds();

        // Original bounds in white
        sf::RectangleShape baseRect;
        baseRect.setPosition(baseBounds.left, baseBounds.top);
        baseRect.setSize(sf::Vector2f(baseBounds.width, baseBounds.height));
        baseRect.setFillColor(sf::Color::Transparent);
        baseRect.setOutlineColor(sf::Color::White);
        baseRect.setOutlineThickness(1.0f);
        window.draw(baseRect);

        // Expanded bounds in configured color
        sf::RectangleShape expandedRect;
        expandedRect.setPosition(ExpandedBounds.left, ExpandedBounds.top);
        expandedRect.setSize(sf::Vector2f(ExpandedBounds.width, ExpandedBounds.height));
        expandedRect.setFillColor(sf::Color::Transparent);
        expandedRect.setOutlineColor(CollisionVisualizationColor);
        expandedRect.setOutlineThickness(1.0f);
        window.draw(expandedRect);

        // Visualize current collisions
        for (auto& [otherComp, _] : CurrentCollisions)
        {
            if (otherComp && otherComp->Owner)
            {
                if (DrawCollisionLineBetweenCenters)
                {
                    DrawCollisionLineBetweenCenter(window, otherComp);
                }

                if (DrawImpactPoint)
                {
                    sf::CircleShape circle;
                    circle.setOrigin(5.0f, 5.0f);
                    circle.setRadius(5);
                    circle.setPosition(CalculateImpactPoint(otherComp));
                    circle.setFillColor(sf::Color::Green);
                    if (circle.getPosition() != sf::Vector2f{0, 0})
                        Globals::Window->draw(circle);
                }
            }
        }
    }

    void CollisionComponent::DrawCollisionLineBetweenCenter(sf::RenderWindow& window, const CollisionComponent* otherComp) const
    {
        // Draw a line connecting the centers
        sf::Vector2f selfCenter(
            ExpandedBounds.left + ExpandedBounds.width / 2,
            ExpandedBounds.top + ExpandedBounds.height / 2
        );

        sf::FloatRect otherBounds = otherComp->GetCollisionBounds();
        sf::Vector2f otherCenter(
            otherBounds.left + otherBounds.width / 2,
            otherBounds.top + otherBounds.height / 2
        );

        sf::Vertex line[] = {
            sf::Vertex(selfCenter, sf::Color::Red),
            sf::Vertex(otherCenter, sf::Color::Red)
        };

        window.draw(line, 2, sf::Lines);
    }

    std::vector<CollisionComponent*>& CollisionComponent::GetRegistry()
    {
        return AllCollisionRegistries;
    }

    //TODO: I am not sure if I should remove this function. For now let's put it bottom of this class to ignore easier 
    void CollisionComponent::SetCollisionEnabled(const bool enabled)
    {
        if (CollisionEnabled == enabled) return;

        CollisionEnabled = enabled;

        // Clear current collisions if disabling
        if (!enabled)
        {
            // Notify exit events for all current collisions
            for (auto& [otherComp, _] : CurrentCollisions)
            {
                if (otherComp && otherComp->Owner)
                {
                    const CollisionEventData eventData(Owner, otherComp->Owner, otherComp, CalculateImpactPoint(otherComp));
                    OnCollisionExit.Broadcast(eventData);
                }
            }

            CurrentCollisions.clear();
        }
    }
}
