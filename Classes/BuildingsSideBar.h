#pragma once
#ifndef BUILDINGSSIDEBAR_H
#define BUILDINGSSIDEBAR_H

#include <memory>

#include "constants.h"
#include "ui/UIWidget.h" //for ui::Widget::TouchEventType

#include "enums/tab_types.h"

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
enum class BuildingTypes;

///Base ListView for sidebar
///
///It creates a ui::ListView specific to each building

using str_listview_t = std::pair<BuildingTypes, cocos2d::ui::ListView*>;
using listviewMap = std::map<BuildingTypes, cocos2d::ui::ListView*>;
using spListviewMap = std::shared_ptr<listviewMap>;

///for managing which tab and building is active
/// ideally only the visible tab would need to get loaded in
/// and maybe the rest would get loaded in overtime
///
/// if a tab wasn't active, it wouldnt do its processing
class TabManager
{
    public:
        std::shared_ptr<Building> active_building;

        //used for loading and saving last active tab, so dont change away from
        //0, 1, 2, 3
        TabTypes active_tab;

        TabManager();

        bool is_tab_unlocked(const TabTypes& tab_type, const std::shared_ptr<Building>& building) const;

        bool is_tab_active(const TabTypes& tab_type, const std::shared_ptr<Building>& building) const;
        void set_tab_active(TabTypes tab_type, const spBuilding& building);

        std::map<cocos2d::ui::Button*, std::pair<TabTypes, spListviewMap>> button_map;
        cocos2d::ui::ListView* get_active_listview();
        cocos2d::ui::Button* get_active_button();
};

class SideListView
{
    public:
        std::shared_ptr<Building> current_target;
        cocos2d::Node* parent;

        TabManager tabs;

        spListviewMap worker_listviews;
        spListviewMap detail_listviews;
        spListviewMap building_listviews;
        spListviewMap menu_listviews;

        cocos2d::ui::Button* tab_worker_btn;
        cocos2d::ui::Button* tab_detail_btn;
        cocos2d::ui::Button* tab_building_btn;
        cocos2d::ui::Button* tab_menu_btn;


        SideListView(cocos2d::Node* parent, std::shared_ptr<Building> current_target);

        virtual spListviewMap _create_listview(std::string node_name);
        virtual cocos2d::ui::Button* _create_button(std::string node_name);

        void toggle_buttons(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType evt);
        void setup_tab_buttons();

        void setup_listviews();
        bool if_tag_exists_in_listview(int child_tag, cocos2d::ui::ListView* listview);

        void setup_worker_listview();
        void setup_detail_listview();
        void setup_building_listview();
        void setup_menu_listview();
};

#endif
