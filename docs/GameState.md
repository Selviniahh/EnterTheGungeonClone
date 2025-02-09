GameState Overview

The **GameState** class serves as a centralized manager to provide access to frequently used instances across different classes like InputManager, DebugTexts, and Enemy.
Setting the Hero Instance

In the Hero class constructor, the hero pointer is registered in GameState like this:

```c++
GameState::GetInstance().SetHero(this);
```

Accessing the Hero Instance

To access the Hero instance from other classes, use the following pattern:
```c++
#include "../Managers/GameState.h"

Hero* heroPtr = nullptr;

if (!heroPtr) {
heroPtr = GameState::GetInstance().GetHero();

//Access properties of heroPtr
heroPtr->GetPosition();
}
```

Performance Consideration

It's crucial to initialize `heroPtr` only when it is nullptr. This prevents repeated calls to `GameState::GetInstance().GetHero()` in functions like Update, which are executed every frame. Unnecessary calls could negatively impact performance.