#include "CollisionComponent.h"
#include "../../Core/GameObjectBase.h"
#include "../../Managers/Globals.h"
#include <imgui.h>

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
                    // New collision
                    CollisionEventData eventData(Owner, otherComp->Owner, otherComp);
                    OnCollisionEnter.Broadcast(eventData);
                }
                else
                {
                    // Continuing collision
                    CollisionEventData eventData(Owner, otherComp->Owner, otherComp);
                    OnCollisionStay.Broadcast(eventData);
                }
            }
            else if (wasColliding)
            {
                // Collision ended
                CollisionEventData eventData(Owner, otherComp->Owner, otherComp);
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
                    CollisionEventData eventData(Owner, otherComp->Owner, otherComp);
                    OnCollisionExit.Broadcast(eventData);
                }
            }
        }

        // Update current collisions
        CurrentCollisions = std::move(stillColliding);
    }

    sf::FloatRect CollisionComponent::GetCollisionBounds() const
    {
        return ExpandedBounds;
    }

    void CollisionComponent::UpdateBounds()
    {
        if (!Owner) return;

        // Get basic bounds from owner
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

        return ExpandedBounds.intersects(other->GetCollisionBounds());
    }

    void CollisionComponent::DrawDebug(sf::RenderWindow& window)
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
        }
    }

    std::vector<CollisionComponent*>& CollisionComponent::GetRegistry()
    {
        return AllCollisionRegistries;
    }

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
                    const CollisionEventData eventData(Owner, otherComp->Owner, otherComp);
                    OnCollisionExit.Broadcast(eventData);
                }
            }

            CurrentCollisions.clear();
        }
    }

    void CollisionComponent::PopulateSpecificWidgets()
    {
        ComponentBase::PopulateSpecificWidgets();

        ImGui::Checkbox("Collision Enabled", &CollisionEnabled);
        ImGui::Checkbox("Show Collision Bounds", &ShowCollisionBounds);
        ImGui::SliderFloat("Collision Radius", &CollisionRadius, 0.0f, 100.0f);

        // Color picker for visualization
        float color[4] = {
            CollisionVisualizationColor.r / 255.0f,
            CollisionVisualizationColor.g / 255.0f,
            CollisionVisualizationColor.b / 255.0f,
            CollisionVisualizationColor.a / 255.0f
        };

        if (ImGui::ColorEdit4("Collision Color", color))
        {
            CollisionVisualizationColor = sf::Color(
                static_cast<sf::Uint8>(color[0] * 255),
                static_cast<sf::Uint8>(color[1] * 255),
                static_cast<sf::Uint8>(color[2] * 255),
                static_cast<sf::Uint8>(color[3] * 255)
            );
        }

        ImGui::Separator();

        // Show active collisions in ImGui
        ImGui::Text("Active Collisions: %zu", CurrentCollisions.size());
        if (!CurrentCollisions.empty())
        {
            ImGui::BeginChild("Collisions", ImVec2(0, 100), true);
            for (auto& [otherComp, _] : CurrentCollisions)
            {
                if (otherComp && otherComp->Owner)
                {
                    ImGui::Text("- %s", otherComp->Owner->GetObjectName().c_str());
                }
            }
            ImGui::EndChild();
        }
    }
}
