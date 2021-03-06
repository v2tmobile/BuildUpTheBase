#include "ItemScene.h"

#include <sstream>

#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerTouch.h"

#include "ui/UIListView.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"

#include "Modal.h"
#include "NodeBuilder.h"
#include "MiscUI.h"
#include "NuItem.h"
#include "Item.h"
#include "GameLogic.h"
#include "HouseBuilding.h"
#include "Buildup.h"
#include "Beatup.h"
#include "Util.h"
#include "Equipment.h"
#include "goals/Achievement.h"
#include "RandomWeightMap.h"
#include "Ingredients.h"
#include "2d/CCLabel.h"
#include "house_building/BuildingTypes.h"

using namespace cocos2d;

void ItemScene::init_children()
{
    this->panel = this->init_panel();

    this->init_title_lbl(panel, this->get_scene_title());

    this->item_misc_info_panel = dynamic_cast<ui::Layout*>(panel->getChildByName("item_misc_info"));
    this->item_misc_info_panel->setVisible(false);
    this->item_misc_info_header_lbl = dynamic_cast<ui::Text*>(this->item_misc_info_panel->getChildByName("header_lbl"));
    this->item_misc_info_body_lbl = dynamic_cast<ui::Text*>(this->item_misc_info_panel->getChildByName("body_lbl"));

    //items listview
    this->items_listview = dynamic_cast<ui::ListView*>(panel->getChildByName("items_listview"));

    //detail panel
    this->item_detail_panel = dynamic_cast<ui::Layout*>(panel->getChildByName("item_detail"));
    this->item_name = dynamic_cast<ui::Text*>(item_detail_panel->getChildByName("item_name"));
    this->item_listview_description = dynamic_cast<ui::ListView*>(item_detail_panel->getChildByName("item_listview_description"));
    this->item_desc = dynamic_cast<ui::Text*>(item_listview_description->getChildByName("item_description"));
    item_desc->setTextAreaSize({ 375, 0 }); //hardcode width of textarea so that it wraps properly
    this->item_sell_btn = this->init_sell_btn(item_detail_panel);

    this->init_back_btn(this->panel);

    this->reset_detail_panel();

};

void ItemScene::init_callbacks()
{
    auto update_detail_panel_on_touch = [this](NuItem* nuitem, spItem item) {
        this->item_name->setString(item->get_name());
        this->item_desc->setString(item->description);

        this->item_sell_btn->setVisible(true);
        auto sell_item_and_reset = [this, nuitem, item](){
            this->items_listview->removeChild(nuitem->button);
            this->items_listview->removeChild(nuitem);

            //remove from items list
            EQUIPMENT->remove_item_from_inventory(item);

            BEATUP->add_total_coins(item->get_effective_cost());

            this->reset_detail_panel();
        };
        bind_touch_ended(this->item_sell_btn, sell_item_and_reset);

        //update listviews layout to account for different content height
        this->item_listview_description->requestDoLayout();
    };

    for (spItem item : this->get_items()) {
        NuItem* nuitem = NuItem::create(this->items_listview);
        nuitem->set_title(item->get_name());
        nuitem->set_description(item->summary);

        nuitem->set_image(item->img_path, ui::Widget::TextureResType::LOCAL);

        res_count_t cost = item->get_effective_cost();
        std::string cost_str = beautify_double(cost);
        nuitem->set_cost_lbl(cost_str);

        auto update_panel = [update_detail_panel_on_touch, item, nuitem](){
            update_detail_panel_on_touch(nuitem, item);
        };
        bind_touch_ended(nuitem->button, update_panel);
    };

};

void ItemScene::reset_detail_panel()
{
    this->item_name->setString(this->get_default_detail_panel_title());
    this->item_desc->setString(this->get_default_detail_panel_description());
    this->item_sell_btn->setVisible(false);

    this->items_listview->requestDoLayout();

    //TODO fix the detail panel not being scrollable after resetting
};

vsItem ItemScene::get_items()
{
    vsItem items;
    //if theres no filter, return all
    if (this->filtered_slot_type == ItemSlotType::Unset){
        items = EQUIPMENT->inventory;
    }
    //otherwise return just the items matching the filter type
    else {
        for (auto& item : EQUIPMENT->inventory){
            if (item->slot_type == this->filtered_slot_type){
                items.push_back(item);
            }
        }
    };

    //remove equipped items
    auto& equipment = EQUIPMENT;
    auto slots = equipment->get_slots();
    auto remove_if_equipped = [slots](std::shared_ptr<Item>& item){
        for (auto& slot : slots){
            if (slot->get_item() == item){
                return true;
            };
        };

        return false;
    };

    auto it = std::remove_if(items.begin(), items.end(), remove_if_equipped);
    if (it != items.end()) items.erase(it);

    //limit number of items
    size_t limit = 50;
    auto end = std::next(items.begin(), std::min(limit, items.size()));
    vsItem limited_items(items.begin(), end);

    return limited_items;
};

cocos2d::ui::Layout* ItemScene::init_panel()
{
    auto items_scene_node = get_prebuilt_node_from_csb("editor/scenes/items_scene.csb");
    items_scene_node->removeFromParent();

    auto panel = dynamic_cast<cocos2d::ui::Layout*>(items_scene_node->getChildByName("panel"));
    panel->removeFromParent();

    this->addChild(panel);

    return panel;
};

void ItemScene::init_title_lbl(cocos2d::Node* panel, std::string title)
{
    //title
    auto title_lbl = dynamic_cast<ui::Text*>(panel->getChildByName("title_lbl"));
    set_aliasing(title_lbl);
    title_lbl->setString(title);
};

void ItemScene::init_back_btn(cocos2d::Node* panel)
{
    //back button
    auto back_btn = dynamic_cast<ui::Button*>(panel->getChildByName("back_btn"));
    prep_back_button(back_btn);
};

cocos2d::ui::Button* ItemScene::init_sell_btn(cocos2d::Node* item_detail_panel)
{
    auto item_sell_btn = dynamic_cast<ui::Button*>(item_detail_panel->getChildByName("item_sell"));
    item_sell_btn->setVisible(false);
    load_default_button_textures(item_sell_btn);
    item_sell_btn->getTitleRenderer()->setTextColor(Color4B::WHITE);
    item_sell_btn->getTitleRenderer()->enableOutline(Color4B::BLACK, 2);
    set_aliasing(item_sell_btn);

    item_sell_btn->setTitleText(this->get_sell_btn_text());

    return item_sell_btn;
};

const std::string& ItemScene::get_scene_title()
{
    static std::string scene_title = "Item Management";
    return scene_title;
};

const std::string& ItemScene::get_default_detail_panel_title()
{
    static std::string default_title = "Item Detail";
    return default_title;
};

const std::string& ItemScene::get_default_detail_panel_description()
{
    static std::string default_desc = "Collect Items at The Dump with the help of Undead scavengers\n\nThis screen will show you more information about them.\n\nYou're able to sell them, and we're planning to have things like people who want items, appeasing gods, improving chances of getting better loot, and even equipping items in slots for new abilities.";
    return default_desc;
};

const std::string& ItemScene::get_sell_btn_text()
{
    static std::string default_desc = "Sell";
    return default_desc;
}

ItemScene* ItemScene::create(ItemSlotType slot_type)
{
    ItemScene* pRet = new(std::nothrow) ItemScene();
    if (pRet && pRet->init(slot_type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
};

bool ItemScene::init(ItemSlotType slot_type)
{
    this->filtered_slot_type = slot_type;

#ifdef _WIN32
    FUNC_INIT_WIN32(ItemScene);
#else
    FUNC_INIT(ItemScene);
#endif

    this->init_children();
    this->init_callbacks();

    return true;
};

cocos2d::Scene* ItemScene::createScene(ItemSlotType slot_type)
{
    auto scene = cocos2d::Scene::create();
    auto* layer = ItemScene::create(slot_type);
    scene->addChild(layer);
    return scene;
};

void AltarItemScene::init_callbacks()
{
    auto update_detail_panel_on_touch = [this](NuItem* nuitem, spItem item) {
        this->item_name->setString(item->get_name());
        this->item_desc->setString(item->description);

        this->item_sell_btn->setVisible(true);
        auto upgrade_and_pop = [item](){
            item->level += 3;

            auto achievement_manager = AchievementManager::getInstance();
            std::shared_ptr<BaseAchievement> achievement = achievement_manager->getAchievement(AchievementType::TotalItemsPlaced);
            achievement->increment();

            cocos2d::Director::getInstance()->popScene();
        };
        bind_touch_ended(this->item_sell_btn, upgrade_and_pop);

        //update listviews layout to account for different content height
        this->item_listview_description->requestDoLayout();
    };

    for (spItem item : this->get_items()) {
        auto nuitem = NuItem::create(items_listview);
        nuitem->set_title(item->get_name());
        nuitem->set_description(item->summary);

        nuitem->set_image(item->img_path, ui::Widget::TextureResType::LOCAL);

        res_count_t cost = item->get_effective_cost();
        std::string cost_str = beautify_double(cost);
        nuitem->set_cost_lbl(cost_str);

        auto update_panel = [item, nuitem, update_detail_panel_on_touch](){
            update_detail_panel_on_touch(nuitem, item);
        };
        bind_touch_ended(nuitem->button, update_panel);
    };

    this->init_back_btn(panel);
};
const std::string& AltarItemScene::get_scene_title()
{
    static std::string scene_title = "Item Altar";
    return scene_title;
};

const std::string& AltarItemScene::get_default_detail_panel_title()
{
    static std::string default_title = "Item";
    return default_title;
};

const std::string& AltarItemScene::get_default_detail_panel_description()
{
    static std::string default_desc = "Place an item at the altar and the gods will see to that it gets strengthed.";
    return default_desc;
};

const std::string& AltarItemScene::get_sell_btn_text()
{
    static std::string default_desc = "Place";
    return default_desc;
}

AltarItemScene* AltarItemScene::create(ItemSlotType slot_type)
{
    AltarItemScene* pRet = new(std::nothrow) AltarItemScene();
    if (pRet && pRet->init(slot_type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
};

bool AltarItemScene::init(ItemSlotType slot_type)
{
    this->filtered_slot_type = slot_type;

#ifdef _WIN32
    FUNC_INIT_WIN32(AltarItemScene);
#else
    FUNC_INIT(AltarItemScene);
#endif

    this->init_children();
    this->init_callbacks();

    return true;
};

cocos2d::Scene* AltarItemScene::createScene(ItemSlotType slot_type)
{
    auto scene = cocos2d::Scene::create();
    auto* layer = AltarItemScene::create(slot_type);
    scene->addChild(layer);
    return scene;
};


void ChanceItemScene::init_callbacks()
{
    auto update_detail_panel_on_touch = [this](NuItem* nuitem, spItem item) {
        this->item_name->setString(item->get_name());
        this->item_desc->setString(item->description);

        this->item_sell_btn->setVisible(true);
        auto upgrade_and_pop = [this, item](){
            this->convert_item_to_chance(item);
        };
        bind_touch_ended(this->item_sell_btn, upgrade_and_pop);

        //update listviews layout to account for different content height
        this->item_listview_description->requestDoLayout();
    };

    for (spItem item : this->get_items()) {
        auto nuitem = NuItem::create(items_listview);
        nuitem->set_title(item->get_name());
        nuitem->set_description(item->summary);

        nuitem->set_image(item->img_path, ui::Widget::TextureResType::LOCAL);

        res_count_t cost = item->get_effective_cost();
        std::string cost_str = beautify_double(cost);
        nuitem->set_cost_lbl(cost_str);

        auto update_panel = [item, nuitem, update_detail_panel_on_touch](){
            update_detail_panel_on_touch(nuitem, item);
        };
        bind_touch_ended(nuitem->button, update_panel);
    };

    this->init_back_btn(panel);
};
const std::string& ChanceItemScene::get_scene_title()
{
    static std::string scene_title = "Hallowed Crypt";
    return scene_title;
};

const std::string& ChanceItemScene::get_default_detail_panel_title()
{
    static std::string default_title = "Item";
    return default_title;
};

const std::string& ChanceItemScene::get_default_detail_panel_description()
{
    static std::string default_desc = "Conceal an item within the crypt and chaos will take it.";
    return default_desc;
};

const std::string& ChanceItemScene::get_sell_btn_text()
{
    static std::string default_desc = "Conceal";
    return default_desc;
}

ChanceItemScene* ChanceItemScene::create(ItemSlotType slot_type)
{
    ChanceItemScene* pRet = new(std::nothrow) ChanceItemScene();
    if (pRet && pRet->init(slot_type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
};

bool ChanceItemScene::init(ItemSlotType slot_type)
{
    this->filtered_slot_type = slot_type;

#ifdef _WIN32
    FUNC_INIT_WIN32(ChanceItemScene);
#else
    FUNC_INIT(ChanceItemScene);
#endif

    this->init_children();
    this->init_callbacks();

    return true;
};

cocos2d::Scene* ChanceItemScene::createScene(ItemSlotType slot_type)
{
    auto scene = cocos2d::Scene::create();
    auto* layer = ChanceItemScene::create(slot_type);
    scene->addChild(layer);
    return scene;
};

void ChanceItemScene::convert_item_upgrade(std::stringstream& body_ss, const spItem& item)
{
    body_ss << "You've received more powerful item!" << std::endl;
    body_ss << std::endl;
    body_ss << std::endl;

    body_ss << item->get_name() << "is now:" << std::endl;
    item->level++;
    body_ss << item->get_name() << std::endl;

};

void ChanceItemScene::convert_item_resource(std::stringstream& body_ss, const spItem& item)
{
    body_ss << "A bundle of resources!" << std::endl;
    body_ss << std::endl;
    body_ss << std::endl;

    spBuilding building;
    Ingredient::SubType ing_type;
    ItemSlotType slot_type = item->slot_type;
    res_count_t quantity = item->level * ITEM_RARITY_MODIFIER.at(item->rarity);

    if (slot_type == ItemSlotType::Combat) {
        building = BUILDUP->city->building_by_type(BuildingTypes::TheArena);
        ing_type = Ingredient::SubType::Soul;
    }
    else if (slot_type == ItemSlotType::Mining) {
        building = BUILDUP->city->building_by_type(BuildingTypes::TheMine);
        ing_type = Ingredient::SubType::Copper;
    }
    else if (slot_type == ItemSlotType::Recipe) {
        building = BUILDUP->city->building_by_type(BuildingTypes::TheWorkshop);
        ing_type = Ingredient::SubType::Bread;
    } else {
        assert(false && "what slot type");
    }

    building->create_ingredients(ing_type, quantity);

    body_ss << beautify_double(quantity) << " " << Ingredient::type_to_string(ing_type) << " added to your resources" << std::endl;
    EQUIPMENT->remove_item_from_inventory(item);

};

void ChanceItemScene::convert_item_coins(std::stringstream& body_ss, const spItem& item)
{
    body_ss << "A pouch of coins!" << std::endl;
    body_ss << std::endl;
    body_ss << std::endl;

    res_count_t quantity = item->get_effective_cost() * 3.0;
    body_ss << beautify_double(quantity) << " coins gained." << std::endl;
    BEATUP->add_total_coins(quantity);

    EQUIPMENT->remove_item_from_inventory(item);

};

void ChanceItemScene::convert_item_to_chance(spItem item)
{
    /*
     *Takes an item, and depending on its type, it generates
     *a specific type of advanced resource.
     *
     *Potentially depends on the quality type of the item to determine
     *the types of chances you get, so a Poor miner tool gets you copper
     *while a rare miner tool gets you more iron
     */

    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    TextBlobModal* modal = TextBlobModal::create();
    modal->set_title("The Gods have seen it");

    std::stringstream body;
    body << "In exchange for your item, you have been blessed with a bounty:" << std::endl;
    enum class WallChanceChoice
    {
        Upgrade = 0,
        Resource = 1,
        Coins = 2
    };

    RandomWeightMap<WallChanceChoice> weight_map({
        { WallChanceChoice::Upgrade, 15 },
        { WallChanceChoice::Resource, 50 },
        { WallChanceChoice::Coins, 50 }
    });

    WallChanceChoice result = weight_map.get_item();
    if (result == WallChanceChoice::Upgrade) {
        this->convert_item_upgrade(body, item);
    }
    else if (result == WallChanceChoice::Resource) {
        this->convert_item_resource(body, item);
    }
    else if (result == WallChanceChoice::Coins) {
        this->convert_item_coins(body, item);
    } else {
        body << "UNKNOWN REWARD" << std::endl;
    };

    modal->set_body(body.str());

    auto on_touch = [this, item, modal](){
        modal->removeFromParent();
        cocos2d::Director::getInstance()->popScene();
    };
    modal->on_layout_touched = on_touch;

    scene->addChild(modal);
};

void ChanceItemScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    ItemScene::onKeyReleased(keyCode, event);
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        spItem item = {};
        this->convert_item_to_chance(item);
    }
};

const std::string& EquipItemScene::get_scene_title()
{
    static std::string scene_title = "Equip Item";
    return scene_title;
};

const std::string& EquipItemScene::get_default_detail_panel_title()
{
    static std::string default_title = "Item";
    return default_title;
};

const std::string& EquipItemScene::get_default_detail_panel_description()
{
    static std::string default_desc = "Equip an item and harness its power.";
    static std::string combat_desc = "Equip an item and harness its power in combat.\n\nRaises your damage dealt in combat in The Arena.";
    static std::string mining_desc = "Equip an item and harness its power in mining.\n\nRaises likelihood of finding iron at The Mine.";
    static std::string recipe_desc = "Equip an item and harness its power in crafting.\n\nRaises chance to earn double or triple crafting output at The Workshop.";
    if (this->filtered_slot_type == ItemSlotType::Combat) {
        return combat_desc;
    };
    if (this->filtered_slot_type == ItemSlotType::Mining) {
        return mining_desc;
    };
    if (this->filtered_slot_type == ItemSlotType::Recipe) {
        return recipe_desc;
    };
    return default_desc;
};

const std::string& EquipItemScene::get_sell_btn_text()
{
    static std::string default_desc = "Equip";
    return default_desc;
}

EquipItemScene* EquipItemScene::create(ItemSlotType slot_type)
{
    EquipItemScene* pRet = new(std::nothrow) EquipItemScene();
    if (pRet && pRet->init(slot_type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
};

void EquipItemScene::init_callbacks()
{

    auto update_detail_panel_on_touch = [this](NuItem* nuitem, spItem item) {
        this->item_name->setString(item->get_name());
        this->item_desc->setString(item->description);

        this->item_sell_btn->setVisible(true);
        auto equip_and_pop = [this, item](){
                EQUIPMENT->get_slot_by_type(
                        this->filtered_slot_type
                )->set_item(item);
                cocos2d::Director::getInstance()->popScene();
        };
        bind_touch_ended(this->item_sell_btn, equip_and_pop);

        //update listviews layout to account for different content height
        this->item_listview_description->requestDoLayout();
    };

    for (spItem item : this->get_items()) {
        auto nuitem = NuItem::create(items_listview);
        nuitem->set_title(item->get_name());
        nuitem->set_description(item->summary);

        nuitem->set_image(item->img_path, ui::Widget::TextureResType::LOCAL);

        res_count_t cost = item->get_effective_cost();
        std::string cost_str = beautify_double(cost);
        CCLOG("cost %f, beauty cost %s", cost, cost_str.c_str());
        nuitem->set_cost_lbl(cost_str);

        auto update_panel = [update_detail_panel_on_touch, item, nuitem](){
            update_detail_panel_on_touch(nuitem, item);
        };
        bind_touch_ended(nuitem->button, update_panel);
    };
};

bool EquipItemScene::init(ItemSlotType slot_type)
{
    this->filtered_slot_type = slot_type;

#ifdef _WIN32
    FUNC_INIT_WIN32(EquipItemScene);
#else
    FUNC_INIT(EquipItemScene);
#endif

    this->init_children();
    this->init_callbacks();


    return true;
};

cocos2d::Scene* EquipItemScene::createScene(ItemSlotType slot_type)
{
    auto scene = cocos2d::Scene::create();
    auto* layer = EquipItemScene::create(slot_type);
    scene->addChild(layer);
    return scene;
};

const std::string& ScrapItemScene::get_scene_title()
{
    static std::string scene_title = "Scrap Item";
    return scene_title;
};

const std::string& ScrapItemScene::get_default_detail_panel_title()
{
    static std::string default_title = "Item";
    return default_title;
};

const std::string& ScrapItemScene::get_default_detail_panel_description()
{
    static std::string default_desc = "Scrap an item, converting it to city investment.\n\nAt The Marketplace, use the invested currency to increase wallet size.\n\nPlace items on Altars in the tunnels, or sacrifice souls to the gods for better items.";
    return default_desc;
};

const std::string& ScrapItemScene::get_sell_btn_text()
{
    static std::string default_desc = "Scrap";
    return default_desc;
}

ScrapItemScene* ScrapItemScene::create()
{
    ScrapItemScene* pRet = new(std::nothrow) ScrapItemScene();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
};

void ScrapItemScene::init_callbacks()
{

    auto update_detail_panel_on_touch = [this](NuItem* nuitem, spItem item) {
        this->item_name->setString(item->get_name());
        this->item_desc->setString(item->description);

        this->item_sell_btn->setVisible(true);
        auto scrap_item = [this, nuitem, item](){
                CCLOG("scrapped item!");

                this->items_listview->removeChild(nuitem->button);
                this->items_listview->removeChild(nuitem);

                GameLogic::getInstance()->add_city_investment((double)item->get_effective_cost());

                //remove from items list
                EQUIPMENT->remove_item_from_inventory(item);

                this->reset_detail_panel();
        };
        bind_touch_ended(this->item_sell_btn, scrap_item);

        //update listviews layout to account for different content height
        this->item_listview_description->requestDoLayout();
    };

    //bind the item nuitems to the open detail
    for (spItem item : this->get_items()) {
        auto nuitem = NuItem::create(items_listview);
        nuitem->set_title(item->get_name());
        nuitem->set_description(item->summary);

        nuitem->set_image(item->img_path, ui::Widget::TextureResType::LOCAL);

        res_count_t cost = item->get_effective_cost();
        std::string cost_str = beautify_double(cost);
        nuitem->set_cost_lbl(cost_str);

        auto update_panel = [item, nuitem, update_detail_panel_on_touch](){
            update_detail_panel_on_touch(nuitem, item);
        };
        bind_touch_ended(nuitem->button, update_panel);
    };

    //update the city investment label
    auto update_city_investment_lbl = [this](float dt){
        res_count_t city_investment = GameLogic::getInstance()->get_city_investment();
        this->item_misc_info_body_lbl->setString(beautify_double(city_investment));
    };
    update_city_investment_lbl(0);
    this->item_misc_info_panel->schedule(update_city_investment_lbl, FPS_10, "update_city_investment_lbl");
};

bool ScrapItemScene::init()
{
    this->filtered_slot_type = ItemSlotType::Unset;

#ifdef _WIN32
    FUNC_INIT_WIN32(ScrapItemScene);
#else
    FUNC_INIT(ScrapItemScene);
#endif

    this->init_children();
    this->init_callbacks();

    this->item_misc_info_panel->setVisible(true);
    this->item_misc_info_header_lbl->setString("City Investment");


    return true;
};

cocos2d::Scene* ScrapItemScene::createScene()
{
    auto scene = cocos2d::Scene::create();
    auto* layer = ScrapItemScene::create();
    scene->addChild(layer);
    return scene;
};
