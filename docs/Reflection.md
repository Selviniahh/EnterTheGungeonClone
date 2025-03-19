# Reflection

The project using [reflection](https://en.wikipedia.org/wiki/Reflective_programming) to collectively provide member variable inspection of any class in runtime through UI.

## Dependencies

- `boost-tpyeindex` used to **stringify** type names
- `boost-describe` for reflection of member variables and classes
- `boost-mp11` for meta-programming to iterate over member variables

___

# How to use reflection

- All objects that need to be reflected should inherit from `GameObjectBase` class
- At the end of the class declaration use `BOOST_DESCRIBE_CLASS` macro. Here's the example from `Hero` class

```c++
BOOST_DESCRIBE_CLASS(Hero,(GameObjectBase),
            (MouseAngle, CurrentDirection, CurrentHeroState, IsDashing, IsDashing ),
            (),
            ())
```

1. argument is the class name,
2. base class name,
3. public member variables that need to be reflected
4. protected member variables that need to be reflected
5. private member variables that need to be reflected
___

Inside the class `src/Managers/TypeRegistry.h` function `InitializeTypeRegistry`, the reflective class needs be defined. An example: 
```c++
static void InitializeTypeRegistry()
        {
            //It doesn't matter to add or not the base class if child is added
            RegisterType<GameClass>();
            RegisterType<GameObjectBase>();
            RegisterType<ComponentBase>();
        }
```

- Next step is to define how to display the class in the UI. Inside `src/Engine/UI/EngineUI.h`, there's  `ShowImGuiWidget` specialized template function.
- If the reflected member variable satisfies any specialization, it will be displayed in the UI. Here's an example for `bool` type

```c++
//Bool
template <>
void ShowImGuiWidget<bool>(const char* label, bool& value);
    
template <>
void ETG::ShowImGuiWidget<bool>(const char* label, bool& value)
{
    BeginProperty(label);

    // Save current style
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));

    //make checkbox a bit narrower
    ImGui::PushItemWidth(5.f);

    if (ImGui::Checkbox("##value", &value))
    {
        // Value changed
    }

    // Restore style
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();

    EndProperty();
}
```

- If the desired type is not supported, it can be added by defining a new specialization of `ShowImGuiWidget` function.
- If the desire type is object specific and not generic, then `GameObjectBase` class' `PopulateSpecificWidgets` function needs to be overriden. UI widgets defined in this function will be populated at UI. As an example, look at `src/Characters/Components/InputComponent` class' `PopulateSpecificWidgets` function.

# Possible errors
1. `error: no matching function for call to 'PopulateReflection'`
    - `BOOST_DESCRIBE_CLASS` given for the type but not defined in `TypeRegistry.h` with `RegisterType`
Track the error output to find the class that is not registered. Example: `note: in instantiation of function template specialization 'ETG::TypeRegistry::RegisterType<ETG::AmmoCounter>' requested here`

___

# Implementation Details

- Refer to `src/Engine/Reflection/Reflection.h`. This file contains the basic, simple but powerful reflection system implementation.
- Refer to `src/Engine/Reflection/Engine.h`. This file contains the reflection calls and UI display functions.
- Refer to `src/Engine/Reflection/UI/EngineUI.h`. This file contains the specialized template functions for generic UI display.
- Refer to `src/Engine/Reflection/UI/UIUtils.h`. This file contains the utility functions for UI display.