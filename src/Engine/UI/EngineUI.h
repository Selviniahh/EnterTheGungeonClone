#pragma once
#include <iostream>
#include <memory>
#include <boost/describe.hpp>
#include <boost/type_index.hpp>
#include <SFML/System/Vector2.hpp>
#include "../../Utils/StrManipulateUtil.h"


namespace sf
{
    class Texture;
}

namespace ETG
{
    class GameObjectBase;

    //Base template
    template <typename T>
    void ShowImGuiWidget(const char* label, T& value);

    //GameObjectBase*
    template <>
    void ShowImGuiWidget<GameObjectBase*>(const char* label, GameObjectBase*& obj);

    //Bool
    template <>
    void ShowImGuiWidget<bool>(const char* label, bool& value);

    //String
    template <>
    void ShowImGuiWidget<std::string>(const char* label, std::string& value);

    //shared-ptr<sf::Texture>
    template <>
    void ShowImGuiWidget<std::shared_ptr<sf::Texture>>(const char* label, std::shared_ptr<sf::Texture>& value);

    //vector2<float>
    template <>
    void ShowImGuiWidget<sf::Vector2<float>>(const char* label, sf::Vector2<float>& value);

    //float
    template <>
    void ShowImGuiWidget<float>(const char* label, float& value);

    class EngineUI
    {
    public:
        //TODO: I need to do something with this two function later I done with reflection
        void ImGuiSetRelativeOrientation(GameObjectBase* obj);
        void ImGuiSetAbsoluteOrientation(GameObjectBase* obj);

        void BeginProperty(const char* label);
        void EndProperty();
        
        template <class T, class Bd, class Md>
        static void PopulateReflection(const T& t);
        
    };

    //------------------------------IMPLEMENTATION----------------------------------------------------
    //Bases:
    template <class T,
    class Bd = boost::describe::describe_bases<T, boost::describe::mod_any_access>,
    class Md = boost::describe::describe_members<T, boost::describe::mod_any_access>>
    void EngineUI::PopulateReflection(const T& t)
    {
        boost::mp11::mp_for_each<Bd>([&]<typename T0>(T0)
        {
            using BTN = typename T0::type;
            auto parentType = boost::typeindex::type_id<BTN>().pretty_name(); //just for test
            PopulateReflection((BTN const&)t);
        });

        //Members
        boost::mp11::mp_for_each<Md>([&](auto D)
        {
            using member_type = std::decay_t<decltype(t.*D.pointer)>;
            auto TypeName = boost::typeindex::type_id<member_type>().pretty_name();
            //GameObjectBase* //TODO: Even though before I planned the Engine related stuffs separate from ETG namespace, they tightly coupled. I have to one day create separate library named Engine and ETG then link Engine to ETG

            auto VariableName = D.name; //"Owner"
            auto* ValuePtr = &(t.*D.pointer); //This is pointer-to-member type of pointer. It doesn't point to any memory
            member_type& valueRef = const_cast<member_type&>(*(ValuePtr)); // Cast away constness and return ref.
            // auto* NonCostValuePtr = const_cast<member_type*>(ValuePtr); //Cast away costness as pointer. Unused for now 

            ShowImGuiWidget<member_type>(VariableName, valueRef);
        });
    }

    template <typename T>
    void ShowImGuiWidget(const char* label, T& value)
    {
        const std::string ErrorMessage = boost::typeindex::type_id<T>().pretty_name() + "and " + label + " not found";
        std::cerr << ErrorMessage;
    }

    void BeginProperty(const char* label);
    void EndProperty();

}


