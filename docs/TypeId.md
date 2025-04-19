# Understanding Object Ownership and Type Checking in Game Architecture

## Projectile Ownership Hierarchy

In our game architecture, we have the following ownership chain:
- **Projectiles** are owned by **Guns**
- **Guns** are owned by either **Players** or **Enemies**

This creates an ownership chain where:
```
Projectile->Owner = Gun
Gun->Owner = Character (Player or Enemy)
```

## The Collision Detection Challenge

When a collision occurs between a Hero and another object, we need to determine:

1. If the colliding object is a Projectile
2. If that Projectile was fired from an Enemy's Gun

This is critical for proper damage application and game mechanics.

## Previous Solution: Dynamic Casting

Our initial implementation used `dynamic_cast` to traverse this ownership chain:

```cpp
CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
{
    auto* enemyObj = dynamic_cast<EnemyBase*>(eventData.Other->Owner->Owner);
    auto* enemyProj = dynamic_cast<ProjectileBase*>(eventData.Other);
    
    if (enemyObj && enemyProj)
    {
        // Handle enemy projectile collision
    }
});
```

## Performance Issue

While functional, this approach has a significant drawback:
- **dynamic_cast** is computationally expensive, especially in collision-heavy games
- It relies on RTTI (Run-Time Type Information), which adds overhead

## New Solution: Custom Type ID System

To improve performance, we've implemented a custom type system using TypeID that:
- Assigns unique integer IDs to each class type
- Registers inheritance relationships between classes
- Provides type-checking that's much faster than dynamic_cast

This system allows us to perform the same type checking with significantly better performance while maintaining the safety of proper type verification.

___

# How to use typeid 

Firstly when a new class is created, we have to both expose it to the reflection and provide type inheritance hierarchy for the **type id**. Here's an example of `InitializeTypeRegistry` function inside `TypeRegistry.cpp`

```cpp
static void InitializeTypeRegistry()
        {
            RegisterType<Hero>();
            REGISTER_BASE_CLASS(Hero, GameObjectBase);
        }
```

Every game object inherited from `GameObjectBase` will have a unique type id and can be checked against it with these simple functions that works similar to unreal engine's cast system: 
```cpp
        template <typename T>
        void SetTypeInfo()
        {
            TypeID = TypeID::GetID<T>();
        }

        //Type checking without knowing derived types
        template <typename T>
        bool IsA() const
        {
            return TypeID::IsBaseOf(GetType(), TypeID::GetID<T>());
        }

        //Safe casting
        template <typename T>
        T* As()
        {
            return IsA<T>() ? static_cast<T*>(this) : nullptr;
        };

        template <typename T>
        const T* As() const
        {
            return IsA<T>() ? static_cast<const T*>(this) : nullptr;
        }
```

All the game objects already needs to be initialized with factory class' `CreateGameObjectDefault` or `CreateGameObjectAttached`. After the object is initialized, these functions will call `SetTypeInfo` to register unique type id automatically.

We are ready. Here's an example for **collision check if collided object is projectile and projectile's `owner->owner` is enemy** 

```cpp
CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        if (eventData.Other->IsA<ProjectileBase>())
                {
                    auto* projectile = eventData.Other->As<ProjectileBase>();
                    
                    if (projectile && projectile->Owner && projectile->Owner->Owner &&
                        projectile->Owner->Owner->IsA<EnemyBase>())
                    {
                        const auto enemy = static_cast<EnemyBase*>(projectile->Owner->Owner);
                    }
                }
    }
```

Note that unlike **clang-tidy**'s safety warning `static_cast` for upcasting is completely safe as long as we know the object's type exactly. We ensured type name is correct with `IsA` function.

