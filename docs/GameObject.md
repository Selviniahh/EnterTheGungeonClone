# What is a Game Object
- A Game Object is the base class for every drawable object. It contains three basic orientation properties: `Position`, `Rotation`, and `Scale`.

# Base, Relative, and Final Properties
- The `Position` property comprises `BasePosition`, `RelativePosition`, and `FinalPosition`.
- Other basic game properties such as `Scale`, `Rotation`, `Origin`, `Depth` also contains same 3 propery. 

## Base Property
- The `GetPosition` function returns `BasePosition`. `BasePosition` is the primary position that other objects modify.
  - For instance, the `Gun`'s `BasePosition` is attached to the hand position.
  - The `Hero`'s `BasePosition` is modified by the Move component.

## Relative Property
- `Relative` properties are modified only through the UI.
- For example, when a `GunHero` is selected in `Details` -> `Relative Orientation`, the given position is treated as an offset relative to `BasePosition`.

## Final Property
- The `Final Property` is the sum of `BasePosition` and `RelativePosition`. Final properties are passed to the `Draw` function.