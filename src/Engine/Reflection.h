#pragma once
#include <boost/describe.hpp>
#include <boost/type_index.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include "UI/EngineUI.h"

class Reflection
{
public:
    //Bases:
    template <class T,
              class Bd = boost::describe::describe_bases<T, boost::describe::mod_any_access>,
              class Md = boost::describe::describe_members<T, boost::describe::mod_any_access>>
    static void PopulateReflection(const T& t)
    {
        boost::mp11::mp_for_each<Bd>([&]<typename T0>(T0)
        {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNode("Base Properties"))
            {
                using BTN = typename T0::type;
                auto parentType = boost::typeindex::type_id<BTN>().pretty_name(); //just for test
                PopulateReflection((BTN const&)t);
                ImGui::TreePop();
            }
        });

        //Members
        boost::mp11::mp_for_each<Md>([&](auto D)
        {
            using member_type = std::decay_t<decltype(t.*D.pointer)>;
            auto TypeName = boost::typeindex::type_id<member_type>().pretty_name();
            //GameObjectBase* //TODO: Even though before I planned the Engine related stuffs separate from ETG namespace, they tightly coupled. I have to one day create separate library named Engine and ETG then link Engine to ETG

            auto VariableName = D.name; //Directly get the variable's exact name. ex: "Owner"
            auto* ValuePtr = &(t.*D.pointer); //This is pointer-to-member type of pointer. It doesn't point to any memory
            member_type& valueRef = const_cast<member_type&>(*(ValuePtr)); // Cast away constness and return ref.
            // auto* NonCostValuePtr = const_cast<member_type*>(ValuePtr); //Cast away costness as pointer. Unused for now 

            ETG::ShowImGuiWidget<member_type>(VariableName, valueRef);
        });
    }
    
};
