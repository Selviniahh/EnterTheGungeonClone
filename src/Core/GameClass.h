//NOTE: This is so so soo important.
//The game drawable objects such as: Hero, Gun, UI are all derived from GameObjectBase
//The components such as: ComponentBase, AnimationBase, HeroAnimComp etc are components and they derived from ComponentBase. Also ComponentBase derived from GameObjectBase
//However Look at Animation folder. There's Animation and AnimationManager class. They are just utility classes not component. THe Animation component using as member variable.
//Now I need to somehow also expose Animation And AnimationManager's member variables to UI. However the reflection logic rejects everything but GameObjectBase. There's a need to change it
//So GameClass implemented. EVERY BUT EVERY Source file needs to be inherited from this class so that their desired member variables can be processed with reflection

#pragma once
#include <boost/describe.hpp>
class GameClass
{
public:
    virtual ~GameClass() = default;
    BOOST_DESCRIBE_CLASS(GameClass, (), (), (), ())
};