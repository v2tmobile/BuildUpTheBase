#pragma once
#ifndef HARVESTSCENE_H
#define HARVESTSCENE_H

#include "GameLayer.h"
#include "magic_particles/_core/magic.h"

class SideListView;
class PopupPanel;
class HarvestableManager;
class MagicEmitter;

namespace cocos2d
{
    class MotionStreak;
    class NodeGrid;
    class Sprite;

    namespace ui
    {
        class Text;
        class TextBMFont;
        class Widget;
    }
}


class BaseScene : public GameLayer
{
    private:
        static Node* _harvest_scene_from_editor;

    public:

        MagicEmitter* cur_node;
        int t_emitter;
        int k_emitter;
        HM_EMITTER m_emitter[1000];
        void SelectEmitter(int emitter);

        CREATE_FUNC(BaseScene);

        Node* get_original_scene_from_editor();

        virtual bool init();
        virtual void update(float dt);

        virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *pEvent) override;
        virtual void onEnter() override;
        virtual void onSwipeLeft(float dt) override;
        virtual void onSwipeRight(float dt) override;

        std::shared_ptr<SideListView> sidebar;
        std::shared_ptr<PopupPanel> popup_panel;

        cocos2d::ui::TextBMFont* upgrade_lbl;
        spRecipe target_recipe;

        void create_goal_loadingbar();
        void create_info_panel();
        void create_username_input();
        void create_player_info_panel();
        void create_building_choicelist();
        void create_inventory_listview();
        void create_shop_listview();
        void create_popup_panel();

        cocos2d::ui::Widget* create_ingredient_detail_alert(IngredientSubType ing_type);

        void generate_WIP_welcome_message(); //only exists to keep it somewhere for later
};

class HarvestScene : public BaseScene
{
    private:
        //level of the target building, for coloring backgrounds
        int _layer_building_level;

    public:
        CREATE_FUNC(HarvestScene);
        virtual bool init();
        virtual void update(float dt);

        Clock* autosave_clock;

        void add_harvestable();

        void toggle_ingredient_listviews(bool remove_children=true);
        Node* show_ingredient_listview_for_ing_type(IngredientSubType ing_type);
        ///return NULL if panel containing ing type is invisible
        Node* get_visible_ing_panel(
            IngredientSubType ing_type
        );

        void spawn_floating_label_for_ing_type(
            IngredientSubType ing_type,
            std::string message
        );
};

#endif
