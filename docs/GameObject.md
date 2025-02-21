# What is a Game Object
- A Game Object is the base class for every drawable object. It contains three basic orientation properties: `Position`, `Rotation`, and `Scale`.

# Base, Relative, and Final Properties
- The `Position` property comprises `BasePosition`, `RelativePosition`
- Other basic game properties such as `Scale`, `Rotation`, `Origin`, `Depth` also contains same 2 propery. 

## Base Property
- The `GetPosition` function returns `BasePosition`. `BasePosition` is the primary position that other objects modify.
  - For instance, the `Gun`'s `BasePosition` is attached to the hand position.
  - The `Hero`'s `BasePosition` is modified by the Move component.

## Relative Property
- `Relative` properties are modified only through the UI.
- For example, when a `GunHero` is selected in `Details` -> `Relative Orientation`, the given position is treated as an offset relative to `BasePosition`.

# Final Property calculation 
The `GameObject` in every frame calculates the last drawable state properties as: 
```c++
virtual void Update()
    {
        //Calculate the final drawing properties. The Base position modified from the source and relative pos given in the UI will be summed to form the final value before being drawn
        DrawProps.Position = Position + RelativePos;
        DrawProps.Scale = {Scale.x * RelativeScale.x, Scale.y * RelativeScale.y};
        DrawProps.Rotation = Rotation + RelativeRotation;
        DrawProps.Origin = Origin + RelativeOrigin;
    }
```

To draw a game object, follow this example from `GunBase` class
```c++
const auto& DrawProps = GetDrawProperties();
AnimationComp->Draw(DrawProps.Position, DrawProps.Origin, DrawProps.Scale, DrawProps.Rotation, DrawProps.Depth);
```