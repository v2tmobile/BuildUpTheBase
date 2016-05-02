#pragma once
#ifndef BUILDINGSSIDEBAR_H
#define BUILDINGSSIDEBAR_H
#include <string>
#include <memory>

namespace cocos2d
{
    class Ref;
    class Node;

    namespace ui
    {
        class ListView;
        class Button;
    }
}

class Building;

///Base ListView for sidebar
/// 
///It creates a ui::ListView specific to each building
class SideListView
{
    public:
        std::shared_ptr<Building> current_target;
        cocos2d::Node* parent;

        cocos2d::ui::ListView* shop_listview;
        cocos2d::ui::ListView* detail_listview;

        cocos2d::ui::Button* tab_shop_btn;
        cocos2d::ui::Button* tab_detail_btn;


        SideListView(cocos2d::Node* parent, std::shared_ptr<Building> current_target);

        virtual cocos2d::ui::ListView* _create_listview(std::string node_name);
        virtual cocos2d::ui::Button* _create_button(std::string node_name);

        void setup_tab_buttons();

        void setup_listviews();
        void setup_shop_listview_as_harvesters();
        void setup_detail_listview_as_recipes();
};

#endif