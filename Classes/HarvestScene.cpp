#include "HarvestScene.h"

#include <cctype>
#include <numeric>
#include <chrono>

#include "cocostudio/ActionTimeline/CSLoader.h"

#include "Harvestable.h"
#include "GameLogic.h"
#include "NuMenu.h"
#include "Beatup.h"
#include "Util.h"
#include "StaticData.h"
#include "attribute.h"
#include "FShake.h"
#include "MiscUI.h"
#include "Animal.h"
#include "Fighter.h"
#include "attribute_container.h"

#include "BuildingsSideBar.h"

#include "HouseBuilding.h"
#include "DataManager.h"
#include "Network.h"

#include "json/rapidjson.h"
#include <json/stringbuffer.h>
#include <json/writer.h>
#include "Serializer.h"
#include <regex>
#include "Modal.h"
#include "Logging.h"
#include "Recipe.h"
#include "Technology.h"

USING_NS_CC;

Node* BaseScene::_harvest_scene_from_editor = NULL;

bool BaseScene::init()
{
    FUNC_INIT(BaseScene);

    BaseScene::_harvest_scene_from_editor = NULL; //reset this because after it gets GCed, it points to garbage

    this->create_info_panel();
    this->create_player_info_panel();
    this->create_username_input();

    this->create_inventory_listview();
    this->create_shop_listview();
    
    this->create_goal_loadingbar();

    this->create_building_choicelist();

    this->create_popup_panel();


    return true;
};

void BaseScene::update(float dt)
{
    GameLayer::update(dt);

    GameLogic::getInstance()->update(dt);

};

void BaseScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) 
    {
        this->quit(NULL);
        event->stopPropagation();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A)
    {
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_B)
    {
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_F1) 
    {
        auto glView = Director::getInstance()->getOpenGLView();
        glView->setFrameSize(1920, 1080);
        glView->setDesignResolutionSize(1920, 1080, ResolutionPolicy::SHOW_ALL);
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_F2) 
    {
        auto glView = Director::getInstance()->getOpenGLView();
        glView->setFrameSize(960, 640);
        glView->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_F5)
    {
        BEATUP->reload_resources();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_GRAVE)
    {
        Director::getInstance()->popScene();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S)
    {
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_T)
    {
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_L)
    {
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_U)
    {
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
    {
        this->popup_panel->animate_open();
    }
}

void BaseScene::scroll_to_target_building()
{
    auto building_pageview = dynamic_cast<ui::PageView*>(this->getChildByName("building_pageview"));
    if (building_pageview)
    {
        vsBuilding buildings = BUILDUP->city->buildings;
        auto find_cb = [](spBuilding building) {
            return building == BUILDUP->get_target_building();
        };
        int index = std::find_if(buildings.begin(),
                                 buildings.end(),
                                 find_cb) - buildings.begin();

        if (index > (int)buildings.size())
        {
            CCLOG("no building found, not moving building pageview");
        } else
        {
            
            building_pageview->ListView::scrollToItem(index, { 0, 0 }, { 0, 0 }, 0.5f);
        }
    }
}

void BaseScene::onEnter()
{
    GameLayer::onEnter();
    scroll_to_target_building();
};

void BaseScene::onSwipeLeft(float dt)
{
    GameLayer::onSwipeLeft(dt);
    BEATUP->cycle_next_building(false);
    scroll_to_target_building();
};

void BaseScene::onSwipeRight(float dt)
{
    GameLayer::onSwipeRight(dt);
    BEATUP->cycle_next_building(true);
    scroll_to_target_building();
    
};

void BaseScene::create_goal_loadingbar()
{
    Node* harvest_scene_editor = this->get_original_scene_from_editor();

    auto progress_panel = (ui::Layout*)harvest_scene_editor->getChildByName("progress_panel");
    progress_panel->removeFromParent();
    this->addChild(progress_panel);

    ui::LoadingBar* loading_bar = dynamic_cast<ui::LoadingBar*>(progress_panel->getChildByName("goal_loadingbar"));
    loading_bar->setPercent(0.0f);

    progress_panel->addTouchEventListener([this](cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evt)
    {
        if (evt == ui::Widget::TouchEventType::ENDED)
        {
            float coin_goal = scale_number(10.0f, (float)BUILDUP->get_target_building()->building_level, 10.5f);
            float percentage = BEATUP->get_total_coins() / coin_goal * 100;
            if (percentage >= 100.0f) {
                this->sidebar->toggle_buttons(this->sidebar->tab_building_btn, ui::Widget::TouchEventType::ENDED);
            }
        }

    });

    auto update_loading_bar = [this, loading_bar](float dt)
        {
            //set progress to 1000 dollars
            float coin_goal = scale_number(10.0f, (float)BUILDUP->get_target_building()->building_level, 10.5f);
            float percentage = BEATUP->get_total_coins() / coin_goal * 100;
            loading_bar->setPercent(percentage);

            if (percentage >= 100.0f) {
                try_set_node_color(loading_bar, { 20, 92, 68 });
            }
            else {
                try_set_node_color(loading_bar, { 177, 212, 200 });
            };

        };
    loading_bar->schedule(update_loading_bar, 0.1f, "loadingbar_update");
    update_loading_bar(0);

    this->upgrade_lbl = dynamic_cast<ui::Text*>(progress_panel->getChildByName("upgrade_lbl"));
    this->upgrade_lbl->setString("");

    set_aliasing(this->upgrade_lbl, true);
}

void BaseScene::create_building_choicelist()
{
    struct ChoiceConfig {
        std::string building_name;
        std::string node_name;
    };
    std::vector<ChoiceConfig> configs = {
        {
            "The Farm", "farm_node"
        }, {
            "The Arena", "arena_node"
        }, {
            "The Underscape", "underscape_node"
        }, {
            "The Marketplace", "marketplace_node"
        }, {
            "The Dump", "dump_node"
        }, {
            "The Workshop", "workshop_node"
        }, {
            "The Mine", "mine_node"
        }, {
            "The Graveyard", "graveyard_node"
        }, {
            "The Forest", "forest_node"
        }
    };

    Node* harvest_scene_editor = this->get_original_scene_from_editor();

    for (auto conf : configs)
    {
        spBuilding building = BUILDUP->city->building_by_name(conf.building_name);

        //get initial node from the prebuilt scene
        Node* building_node = harvest_scene_editor->getChildByName(conf.node_name);
        ui::Button* panel = (ui::Button*)building_node->getChildByName("building_panel");
        building_node->removeFromParent();
        this->addChild(building_node);


        //set building name
        ui::Text* building_name = (ui::Text*)panel->getChildByName("building_name");
        building_name->setString(building->short_name);
        set_aliasing(building_name, true);

        //add building image
        ui::ImageView* building_image = (ui::ImageView*)panel->getChildByName("building_image");
        std::string img_path = "";
        if (building->name == "The Mine") {
            img_path = "dirt_1.png";
        } else if (building->name == "The Forest") {
            img_path = "tree.png";
        } else if (building->name == "The Dump") {
            img_path = "dump_darker.png";
        } else if (building->name == "The Workshop") {
            img_path = "anvil.png";
        } else if (building->name == "The Arena") {
            img_path = "sword.png";
        } else if (building->name == "The Graveyard") {
            img_path = "grave.png";
        } else if (building->name == "The Underscape") {
            img_path = "necro_open.png";
        } else if (building->name == "The Farm") {
            img_path = "farm.png";
        } else {
            img_path = "weapon_ice.png";
        };
        building_image->loadTexture(img_path, ui::TextureResType::PLIST);

        load_default_button_textures(panel);

        auto update_func = [panel, building, building_image, img_path](float dt)
        {
            auto target_building = BUILDUP->get_target_building();

            std::string tex_name;
            int res_type;

            if (target_building == building)
            {
                tex_name = "crosshair_red.png";
                res_type = (int)ui::TextureResType::PLIST;
            }
            else
            {
                tex_name = img_path;
                res_type = (int)ui::Widget::TextureResType::PLIST;
            }

            ResourceData r_data = building_image->getRenderFile();
            if (r_data.file != tex_name || r_data.type != (int)res_type)
            {
                building_image->loadTexture(tex_name, (ui::Widget::TextureResType)res_type);
            }

            if (building->is_storage_full_of_ingredients(building->punched_sub_type))
            {
                Color3B reddish = { 243, 162, 173 };
                try_set_node_color(panel, reddish);
            }
            else
            {
                try_set_node_color(panel, Color3B::WHITE);
            }
        };
        building_node->schedule(update_func, 0.01f, "update_func");
        update_func(0);

        auto touch_handler = [panel, building](Ref*, ui::Widget::TouchEventType evt)
        {
            if (evt == ui::Widget::TouchEventType::ENDED)
            {
                BUILDUP->set_target_building(building);
            }
        };
        panel->addTouchEventListener(touch_handler);
    };
};

void BaseScene::create_popup_panel()
{
    auto harvest_scene = this->get_original_scene_from_editor();

    auto panel_node = dynamic_cast<cocos2d::ui::Layout*>(harvest_scene->getChildByName("popup_panel"));
    panel_node->removeFromParent();
    this->popup_panel = std::make_shared<PopupPanel>(panel_node);
    this->popup_panel->set_visible(false);
    this->addChild(panel_node);
};

Node* BaseScene::get_original_scene_from_editor()
{
    if (BaseScene::_harvest_scene_from_editor == NULL)
    {
        auto inst = CSLoader::getInstance();
        BaseScene::_harvest_scene_from_editor = inst->CSLoader::createNode("editor/scenes/base_scene.csb");
    };

    return BaseScene::_harvest_scene_from_editor;
}

struct cached_building_info_t
{
    res_count_t ing_count;
    std::string ing_count_str;

    res_count_t storage_count;
    std::string storage_str;
};

void BaseScene::create_info_panel()
{
    auto building = BUILDUP->get_target_building();

    Node* harvest_scene_editor = this->get_original_scene_from_editor();

    ui::Layout* arena_kill_panel = dynamic_cast<ui::Layout*>(harvest_scene_editor->getChildByName("arena_kill_panel"));
    arena_kill_panel->removeFromParent();
    this->addChild(arena_kill_panel);
    ui::Text* arena_kill_lbl = dynamic_cast<ui::Text*>(arena_kill_panel->getChildByName("arena_kill_lbl"));

    auto update_arena_kill_display = [this, arena_kill_panel, arena_kill_lbl](float dt){
        if (BUILDUP->get_target_building()->name == "The Arena") {
            arena_kill_panel->setVisible(true);
            res_count_t kills = (res_count_t)GameLogic::getInstance()->get_total_kills(); //comes through as int though
            arena_kill_lbl->setString("Kills: "+beautify_double(kills));
        } else if (BUILDUP->get_target_building()->name == "The Workshop") {
            arena_kill_panel->setVisible(true);
            if (this->target_recipe != NULL)
            {
                arena_kill_lbl->setString(this->target_recipe->name);
            }
            else
            {
                arena_kill_lbl->setString("Choose a recipe");
            }
        } else {
            arena_kill_panel->setVisible(false);
        }
    };
    arena_kill_panel->schedule(update_arena_kill_display, LONG_DELAY, "update_arena_kill_display");
    update_arena_kill_display(0);


    ui::Layout* building_info_panel = dynamic_cast<ui::Layout*>(harvest_scene_editor->getChildByName("building_info_panel"));
    building_info_panel->removeFromParent();

    auto create_count = [](std::string prefix, res_count_t count) {
        char buffer[50];
        sprintf(buffer,"%s: %s", prefix.c_str(), beautify_double(count).c_str());
        return std::string(buffer);
    };

    const float AVERAGE_DELAY = 0.1f;

    auto building_name = dynamic_cast<ui::Text*>(building_info_panel->getChildByName("building_name"));
    auto update_building_name = [building_name](float dt){
        building_name->setString(BUILDUP->get_target_building()->name);
    };
    this->schedule(update_building_name, AVERAGE_DELAY, "building_name_update");
    update_building_name(0);

    auto ing_count = dynamic_cast<ui::Text*>(building_info_panel->getChildByName("ingredient_count"));
    cached_building_info_t* cached_building_info = new cached_building_info_t();
    ing_count->setUserData(static_cast<void*>(cached_building_info));

    auto update_ing_count = [create_count, ing_count](float dt)
    {
        cached_building_info_t* cached = static_cast<cached_building_info_t*>(ing_count->getUserData()); //cant use dynamic_cast on void*
        if (cached == NULL)
        {
            CCLOG("cached data needs to be initialized on create on widget");
        }

        spBuilding building = BUILDUP->get_target_building();
        res_count_t storage_space = building->get_storage_space();
        std::string storage;
        if (cached->storage_count != storage_space)
        {
            storage = beautify_double(storage_space);
            cached->storage_count = storage_space;
            cached->storage_str = storage;
        }
        else
        {
            storage = cached->storage_str;
        }
        res_count_t ing_count_val = building->count_ingredients();

        std::string count;
        if (cached->ing_count != ing_count_val)
        {
            count = create_count("ING", ing_count_val);
            cached->ing_count = ing_count_val;
            cached->ing_count_str = count;
        }
        else
        {
            count = cached->ing_count_str;
        }
        char buffer[50];
        sprintf(buffer, "%s/%s", count.c_str(), storage.c_str());
        ing_count->setString(buffer);

        ing_count->setUserData(cached);
    };
    this->schedule(update_ing_count, REALTIME_DELAY, "ing_count_update");
    update_ing_count(0);

    auto harvester_count = dynamic_cast<ui::Text*>(building_info_panel->getChildByName("harvester_count"));
    auto update_harvester_count = [harvester_count](float dt)
    {
        spBuilding building = BUILDUP->get_target_building();

        std::stringstream ss;
        res_count_t total_active = 0;
        for (auto&& types_to_count : building->harvesters)
        {
            total_active += types_to_count.second;
        };

        if (total_active >= 1){
            res_count_t harvester_output = building->get_total_harvester_output();
            ss << "Harvesters: " << beautify_double(total_active) << "\n " << beautify_double(harvester_output) << "/sec";
            harvester_count->setString(ss.str());
            harvester_count->setVisible(true);
        }
        else {
            harvester_count->setVisible(false);
        }
    };
    this->schedule(update_harvester_count, AVERAGE_DELAY, "harvester_count_update");
    update_harvester_count(0);

    auto salesmen_count = dynamic_cast<ui::Text*>(building_info_panel->getChildByName("salesmen_count"));
    auto update_salesmen_count = [salesmen_count](float dt)
    {
        spBuilding building = BUILDUP->get_target_building();

        std::stringstream ss;
        res_count_t total_active = 0;
        for (auto&& types_to_count : building->salesmen)
        {
            total_active += types_to_count.second;
        };
        if (total_active >= 1){
            res_count_t salesmen_output = building->get_total_salesmen_output();
            ss << "Salesmen: " << beautify_double(total_active) << "\n " << beautify_double(salesmen_output) << "/sec";
            salesmen_count->setString(ss.str());
            salesmen_count->setVisible(true);
        }
        else {
            salesmen_count->setVisible(false);
        }
    };
    this->schedule(update_salesmen_count, AVERAGE_DELAY, "salesmen_count_update");
    update_salesmen_count(0);

    this->addChild(building_info_panel);
};

void BaseScene::create_username_input()
{
    auto building_panel = this->getChildByName("building_info_panel");
    ui::TextField* username_input = dynamic_cast<ui::TextField*>(building_panel->getChildByName("username_input"));

    username_input->setTextHorizontalAlignment(TextHAlignment::CENTER);
    ui::UICCTextField* renderer = dynamic_cast<ui::UICCTextField*>(username_input->getVirtualRenderer());
    cocos2d::Label* label = dynamic_cast<Label*>(renderer);
    set_aliasing(label);

    auto username = DataManager::get_string_from_data("username");
    username_input->setString(username);

    auto is_bad_character = [](char character){
        return !(std::isalnum(character) || character == '_');
    };
    auto clean_username = [is_bad_character](std::string username) {
        username.erase(std::remove_if(username.begin(), username.end(), is_bad_character), username.end());
        return username;
    };

    auto textfield_listener = [username_input, clean_username](Ref* target, ui::TextField::EventType evt)
        {
            if (evt == ui::TextField::EventType::INSERT_TEXT || 
                evt == ui::TextField::EventType::DELETE_BACKWARD)
            {
                std::string text = username_input->getString();
                CCLOG("Got raw username: %s", text.c_str());
                std::string cleaned_username = clean_username(text);
                CCLOG("is cleaned to %s", cleaned_username.c_str());
                username_input->setString(cleaned_username);

                DataManager::set_string_from_data("username", cleaned_username);
            }
        };
    username_input->addEventListener(textfield_listener);
}

void BaseScene::create_player_info_panel()
{
    auto building = BUILDUP->get_target_building();

    Node* harvest_scene_editor = this->get_original_scene_from_editor();

    ui::Layout* player_info_panel = dynamic_cast<ui::Layout*>(harvest_scene_editor->getChildByName("player_info_panel"));
    player_info_panel->removeFromParent();

    ui::Text* player_info_lbl = dynamic_cast<ui::Text*>(player_info_panel->getChildByName("player_info_lbl"));
    set_aliasing(player_info_lbl, true);

    player_info_panel->addTouchEventListener([player_info_panel, this](Ref* target, ui::Widget::TouchEventType type)
    {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            auto gold = BEATUP->get_total_coins();
            int heal_cost = 10;
            if (gold > heal_cost)
            {
                auto health = BUILDUP->fighter->attrs->health;
                if (health->is_full() == false)
                {
                    health->add_to_current_val(5);
                    BEATUP->add_total_coin(-heal_cost);
                }
            }
            else
            {
                auto player_gold_lbl = dynamic_cast<ui::Text*>(player_info_panel->getChildByName("player_gold_lbl"));
                player_gold_lbl->runAction(FShake::actionWithDuration(0.05f, 10));
            }
        }
    });

    auto create_count = [](std::string prefix, int count) {
        std::stringstream ss;
        ss << prefix << ": " << count;
        return ss.str();
    };

    auto player_gold_lbl = dynamic_cast<ui::Text*>(player_info_panel->getChildByName("player_gold_lbl"));
    set_aliasing(player_gold_lbl, true);
    auto player_hp_lbl = dynamic_cast<ui::Text*>(player_info_panel->getChildByName("player_hp_lbl"));
    auto update_gold_lbl = [player_gold_lbl, player_hp_lbl](float dt){
        //set gold
        std::stringstream coin_ss;
        res_count_t total_coins = BEATUP->get_total_coins();
        auto gold = std::round(total_coins);
        coin_ss << "$" << beautify_double(gold);
        std::string coin_msg = coin_ss.str();
        player_gold_lbl->setString(coin_msg);

        //set hp
        std::stringstream hp_ss;
        auto hp = BUILDUP->fighter->attrs->health;
        hp_ss << "HP: " << hp->current_val << "/" << hp->max_val;
        player_hp_lbl->setString(hp_ss.str());

        if (hp->is_empty()) {
            try_set_text_color(player_hp_lbl, Color4B::RED);
        } else {
            try_set_text_color(player_hp_lbl, Color4B::WHITE);
        }

    };
    this->schedule(update_gold_lbl, AVERAGE_DELAY, "player_gold_lbl_update");
    update_gold_lbl(0);

    this->addChild(player_info_panel);
};

void BaseScene::create_inventory_listview()
{
    Node* harvest_scene_editor = this->get_original_scene_from_editor();
    ui::ListView* inventory_listview = dynamic_cast<ui::ListView*>(harvest_scene_editor->getChildByName("inventory_listview"));

    inventory_listview->removeFromParent();
    this->addChild(inventory_listview);

    auto inst = CSLoader::getInstance();
    auto update_listview = [this, inst, inventory_listview](float dt)
    {
        auto raw_node = inst->createNode("editor/buttons/inventory_button.csb");
        auto orig_item_panel = dynamic_cast<ui::Button*>(raw_node->getChildByName("item_panel"));
        orig_item_panel->removeFromParent();

        typedef std::pair<Ingredient::SubType, res_count_t> maptype;
        auto order_by_count = [](maptype left, maptype right)
        {
            return left.second > right.second;
        };

        std::vector<maptype> type_vec;
        mistIngredient city_ingredients = BUILDUP->get_all_ingredients();

        for (auto&& ts : Ingredient::type_map)
        {
            // type_vec.push_back({ ts.first, map_get(city_ingredients, ts.first, 0) });

            res_count_t _def = 0;
            Ingredient::SubType ing_type = ts.first;
            auto count = map_get(city_ingredients, ing_type, _def);
            // if (count != 0)
            type_vec.push_back({ ts.first, count });
        }

        auto begin = type_vec.begin();
        auto end = type_vec.end();
        std::sort(begin, end, order_by_count);

        for (auto&& type_to_count : type_vec)
        {
            Ingredient::SubType ing_type = type_to_count.first;
            auto str_type = Ingredient::type_to_string(ing_type);

            IngredientData res_data = IngredientData(str_type);

            auto zero_ingredients = city_ingredients[ing_type] <= 0;

            //if the child already exists, put it at the back 
            ui::Button* existing_node = dynamic_cast<ui::Button*>(inventory_listview->getChildByName(str_type));
            if (existing_node)
            {
                //last frame, there was probably ingredients, so cleaup the
                //leftover button
                if (zero_ingredients) 
                {
                    inventory_listview->removeChild(existing_node, false);
                }
                else 
                {
                    continue;
                };
            }

            //dont build the ingredient button if for zero ingredients
            if (zero_ingredients) 
            {
                continue;
            }

            auto new_item_panel = dynamic_cast<ui::Button*>(orig_item_panel->clone());
            load_default_button_textures(new_item_panel);
            new_item_panel->setName(str_type);

            //set aliasing on font
            auto item_lbl = dynamic_cast<ui::Text*>(new_item_panel->getChildByName("item_lbl"));
            set_aliasing(item_lbl, true);

            //add ingredient image
            ui::ImageView* item_img = (ui::ImageView*)new_item_panel->getChildByName("item_img");
            item_img->loadTexture(res_data.get_img_large());
            set_aliasing(item_img, true);

            //if there's less than 1 ingredient, hide the item panel altogether
            if (city_ingredients[ing_type] <= 0) { new_item_panel->setVisible(false); }
            else { new_item_panel->setVisible(true); }

            auto on_touch_cb = [ing_type, this, new_item_panel](Ref* ref, ui::Widget::TouchEventType type) {

                if (type == ui::Widget::TouchEventType::ENDED) {
                    auto alert = this->create_ingredient_detail_alert(ing_type);
                    this->addChild(alert, 10);

                    //animate
                    Vec2 start_pos = new_item_panel->getTouchEndPosition();
                    alert->setPosition(start_pos);

                    alert->setScale(0);

                    float duration = 0.25f;
                    auto scale = ScaleTo::create(duration, 1.0f, 1.0f);

                    Vec2 end_pos = this->get_center_pos();
                    auto move = MoveTo::create(duration, end_pos);

                    Sequence* seq = Sequence::create(Spawn::createWithTwoActions(move, scale), NULL);
                    alert->runAction(seq);

                    do_vibrate(32);
                };
            };
            new_item_panel->addTouchEventListener(on_touch_cb);

            float AVERAGE_DELAY = 0.1f;
            auto update_lbl_cb = [new_item_panel, this, ing_type](float)
            {
                auto it = ing_type; 
                auto type_str = Ingredient::type_to_string(it);
                char buffer[50];

                mistIngredient city_ingredients = BUILDUP->get_all_ingredients();
                res_count_t _def = 0;
                res_count_t count = map_get(city_ingredients, it, _def);
                sprintf(buffer, "%s\n%s", beautify_double(count).c_str(), type_str.c_str());
                auto item_lbl = dynamic_cast<ui::Text*>(new_item_panel->getChildByName("item_lbl"));
                item_lbl->setString(buffer);

                if (count > 0)
                {
                    new_item_panel->setVisible(true);
                }
                else
                {
                    new_item_panel->setVisible(false);
                }
            };
            update_lbl_cb(0); //fire once immediately
            new_item_panel->schedule(update_lbl_cb, SHORT_DELAY, "item_lbl_update");

            inventory_listview->addChild(new_item_panel);
        };
    };

    update_listview(0.0f); //why doesnt this do anything
    inventory_listview->schedule(update_listview, 0.1f, "update_listview");
};

void BaseScene::create_shop_listview()
{

    this->sidebar = std::make_shared<SideListView>(this, BUILDUP->get_target_building());

    this->sidebar->setup_shop_listview_as_harvesters();
    this->sidebar->setup_detail_listview_as_recipes();
    this->sidebar->setup_building_listview_as_upgrades();
    this->sidebar->setup_powers_listview_as_powers();
};

bool HarvestScene::init()
{
    BaseScene::init();

    this->target_recipe = NULL;
    this->create_recipe_lbl();

    this->add_harvestable();

    //color layer based on building level
    LayerColor* layer_color = LayerColor::create(Color4B(255, 0, 0, 120));
    layer_color->setName("building_level_color");
    this->addChild(layer_color, -100); //set it behind everything

    this->_layer_building_level = -99; //using some bogus number

    //update layer color when the building's level has changed
    std::function<void(float)> update_layer_color = [this, layer_color](float dt)
    {
        spBuilding target_building = BUILDUP->get_target_building();
        if (target_building->building_level == this->_layer_building_level)
        {
            return;
        }

        this->_layer_building_level = target_building->building_level;

        std::map<int, Color3B> color_map ={
            {1,  {0,   0,   0}},
            {2,  {25,  25,  25}},
            {3,  {50,  50,  50}},
            {4,  {75,  75,  75}},
            {5,  {100, 100, 100}},
            {6,  {125, 125, 125}},
            {7,  {150, 150, 150}},
            {7,  {175, 175, 175}},
            {8,  {200, 200, 200}},
            {9,  {225, 225, 225}},
            {10, {250, 250, 250}},
            {11, {251, 251, 251}},
            {12, {252, 252, 252}},
            {13, {253, 253, 253}},
            {14, {254, 254, 254}},
            {15, {255, 255, 255}},
            {16, {255, 0,   0  }},
            {17, {255, 255, 0  }},
            {18, {255, 255, 255}}
        };

        Color3B _def = Color3B::BLACK;
        auto color = map_get(color_map, this->_layer_building_level, _def);
        try_set_node_color(layer_color, color);


    };
    this->schedule(update_layer_color, 0.0f, "update_layer_color");
    update_layer_color(0.0f);

    this->setOnEnterCallback([]()
    {
        GameLogic::getInstance()->post_load();
    });

    //autosave every minute
    this->autosave_clock = new Clock(60.0f);


    return true;

}

void HarvestScene::update(float dt)
{
    BaseScene::update(dt);

    //TODO get this percentage from a method or something so if building scale
    //changes i dont need to adjust this
    float coin_goal = scale_number(10.0f, (float)BUILDUP->get_target_building()->building_level, 10.5f);
    float percentage = BEATUP->get_total_coins() / coin_goal * 100;
    if (percentage >= 100.0f) {
        ((HarvestScene*)this)->upgrade_lbl->setString("Upgrade available!");
    }
    else {
        ((HarvestScene*)this)->upgrade_lbl->setString("Next upgrade:");
    };

    auto harvestable = dynamic_cast<Harvestable*>(this->getChildByName("harvestable"));
    if (!harvestable) {
        this->add_harvestable();
    }
    else if (BUILDUP->get_target_building() != harvestable->building) {
        harvestable->removeFromParent();
    };

    this->autosave_clock->update(dt);
    if (this->autosave_clock->passed_threshold())
    {
        this->autosave_clock->reset();

        CCLOG("Autosaving");
        GameLogic::save_all();
    };


}

ui::Widget* BaseScene::create_ingredient_detail_alert(Ingredient::SubType ing_type)
{
    //make sure one doesn't already exist first
    if (this->getChildByName("inventory_detail_panel")) {
        this->getChildByName("inventory_detail_panel")->removeFromParent();
    };

    IngredientData res_data = IngredientData(Ingredient::type_to_string(ing_type));

    auto inst = CSLoader::getInstance();
    auto raw_node = inst->createNode("editor/details/inventory_detail.csb");

    auto alert_panel = dynamic_cast<ui::Layout*>(raw_node->getChildByName("Panel_1"));
    alert_panel->removeFromParent();
    alert_panel->setBackGroundColor(Color3B(85, 114, 145));
    alert_panel->setBackGroundColorOpacity(255);

    ui::Layout* close_panel = dynamic_cast<ui::Layout*>(alert_panel->getChildByName("close_panel"));
    auto cb = [alert_panel](Ref* target, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            alert_panel->removeFromParent();
        };
    };
    close_panel->addTouchEventListener(cb);

    auto resource_name = dynamic_cast<ui::Text*>(alert_panel->getChildByName("resource_name"));
    std::string res_name = Ingredient::type_to_string(ing_type);
    resource_name->setString(res_name);

    auto resource_description = dynamic_cast<ui::Text*>(alert_panel->getChildByName("resource_description"));
    resource_description->setString(res_data.get_description());

    auto count_desc = dynamic_cast<ui::Text*>(alert_panel->getChildByName("count_desc"));
    auto count_lbl = dynamic_cast<ui::Text*>(alert_panel->getChildByName("count_lbl"));

    auto AVERAGE_DELAY = 0.1f;

    alert_panel->schedule([count_lbl, ing_type](float) {
        auto it = ing_type;
        auto all_ing = BUILDUP->get_all_ingredients();
        res_count_t _def = 0.0;
        res_count_t count = map_get(all_ing, it, _def);
        count_lbl->setString(beautify_double(count));
    }, AVERAGE_DELAY, "alert_count_update");

    res_count_t coins_gained = 3;
    auto create_sell_button = [this, alert_panel, ing_type, coins_gained, AVERAGE_DELAY](std::string name, int amount_sold)
    {
        auto sell_btn = dynamic_cast<ui::Button*>(alert_panel->getChildByName(name));
        load_default_button_textures(sell_btn);

        sell_btn->addTouchEventListener([this, ing_type, coins_gained, amount_sold, AVERAGE_DELAY](Ref* touch, ui::Widget::TouchEventType type){
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                mistIngredient city_ingredients = BUILDUP->get_all_ingredients();

                res_count_t _def = 0;
                auto it = ing_type;
                int num_sellable = map_get(city_ingredients, it, _def);
                if (num_sellable != 0)
                {
                    int to_sell = std::min(num_sellable, amount_sold);
                    CCLOG("selling %i of %s", to_sell, Ingredient::type_to_string(ing_type).c_str());
                    BEATUP->add_total_coin(to_sell*coins_gained);

                    //for each building that has one of the ingredients, remove one at a time until there's no to sell left
                    BUILDUP->remove_shared_ingredients_from_all(ing_type, to_sell);

                    do_vibrate(32);
                }
                else
                {
                    //smaller vibrate if you can't sell any
                    do_vibrate(16);
                };
            }
        });

        sell_btn->schedule([sell_btn, this, ing_type, amount_sold](float){
            mistIngredient city_ingredients = BUILDUP->get_all_ingredients();
            res_count_t _def = 0;
            auto it = ing_type;
            if (map_get(city_ingredients, it, _def) < amount_sold)
            {
                //this doesnt let the button accept touches, so we need to subclass widget or something to fix it
                try_set_enabled(sell_btn, false);
            }
            else
            {
                try_set_enabled(sell_btn, true);
            }
        }, AVERAGE_DELAY, "sell_btn_state_cb");
    };

    create_sell_button("sell_1_btn", 1);
    create_sell_button("sell_10_btn", 10);
    create_sell_button("sell_100_btn", 100);
    create_sell_button("sell_1000_btn", 1000);

    auto value_lbl = dynamic_cast<ui::Text*>(alert_panel->getChildByName("value_lbl"));
    auto update_value_lbl = [coins_gained, alert_panel, value_lbl](float dt){
        auto cg = coins_gained;
        std::stringstream value_ss;
        value_ss << "for " << beautify_double(cg) << "$ each";
        value_lbl->setString(value_ss.str().c_str());
    };
    value_lbl->schedule(update_value_lbl, 0.2f, "update_value_lbl");
    update_value_lbl(0);



    auto move_btn = dynamic_cast<ui::Button*>(alert_panel->getChildByName("move_btn"));
    load_default_button_textures(move_btn);
    move_btn->schedule([move_btn, this, ing_type](float){
        spBuilding target_building = BUILDUP->get_target_building();
        mistIngredient& ingredients = target_building->ingredients;
        if (target_building == BUILDUP->city->building_by_name("The Workshop")){
            move_btn->setVisible(false);
        }
        else if (target_building->count_ingredients(ing_type) < 10)
        {
            try_set_enabled(move_btn, false);
        }
        else
        {
            try_set_enabled(move_btn, true);
        }
    }, AVERAGE_DELAY, "move_btn_state_cb");

    move_btn->addTouchEventListener([this, ing_type](Ref* touch, ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            mistIngredient& ingredients = BUILDUP->get_target_building()->ingredients;
            if (ingredients.empty()){ return; }

            Animal animal("WorkshopWagon");
            animal.transfer_ingredients(
                BUILDUP->get_target_building(),
                BUILDUP->city->building_by_name("The Workshop"),
                ing_type,
                10);

        }
    });

    alert_panel->setPosition(this->get_center_pos());

    alert_panel->setName("inventory_detail_panel");

    return alert_panel;
};

void HarvestScene::add_harvestable()
{
    Harvestable* harvestable;
    auto target_building = BUILDUP->get_target_building();


    if (target_building->name == "The Mine") {
        harvestable = MiningHarvestable::create();
    } else if (target_building->name == "The Forest") {
        harvestable = TreeHarvestable::create();
    } else if (target_building->name == "The Dump") {
        harvestable = DumpsterHarvestable::create();
    } else if (target_building->name == "The Workshop") {
        harvestable = CraftingHarvestable::create(this->target_recipe);
    } else if (target_building->name == "The Arena") {
        harvestable = FightingHarvestable::create();
    } else if (target_building->name == "The Graveyard") {
        harvestable = GraveyardHarvestable::create();
    } else if (target_building->name == "The Underscape") {
        harvestable = UndeadHarvestable::create();
    } else if (target_building->name == "The Farm") {
        harvestable = FarmingHarvestable::create();
    } else {
        harvestable = Harvestable::create();
    };

    this->addChild(harvestable);
};

void HarvestScene::create_recipe_lbl()
{
}
