#include "HarvestScene.h"
#include "Harvestable.h"

#include "GameLogic.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "NuMenu.h"
#include "Beatup.h"
#include "Util.h"
#include "MainMenu.h"

USING_NS_CC;

bool HarvestScene::init()
{
    FUNC_INIT(HarvestScene);

    ui::Button* shop_button = this->create_shop_button();
    this->addChild(shop_button);

    ui::Button* city_button = this->create_city_button();
    this->addChild(city_button);

    ui::Layout* info_panel = this->create_info_panel();
    this->addChild(info_panel);

    this->add_harvestable();

    this->scheduleUpdate();

    return true;
};

void HarvestScene::add_harvestable()
{
    auto harvestable = Harvestable::create(GameLogic::getInstance()->beatup);
    harvestable->setPosition(this->get_center_pos());
    harvestable->setName("harvestable");

    harvestable->setScale(harvestable->getScale()*4); //4*4 scale now

    this->addChild(harvestable);
};

void HarvestScene::update(float dt)
{
    GameLogic::getInstance()->update(dt);

    if (!this->getChildByName("harvestable")) {
        this->add_harvestable();
    };
};

void HarvestScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) 
    {
        this->quit(NULL);
        event->stopPropagation();
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
        GameLogic::getInstance()->beatup->reload_resources();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_GRAVE)
    {
        Director::getInstance()->popScene();
    }
};

ui::Layout* HarvestScene::create_info_panel()
{
    auto building = GameLogic::getInstance()->buildup->target_building;

    auto inst = CSLoader::getInstance();
    Node* harvest_scene_editor = inst->createNode("editor/scenes/harvest_scene.csb");

    ui::Layout* building_info_panel = dynamic_cast<ui::Layout*>(harvest_scene_editor->getChildByName("building_info_panel"));
    building_info_panel->removeFromParent();

    auto create_count = [](std::string prefix, int count) {
        std::stringstream ss;
        ss << prefix << ": " << count;
        return ss.str();
    };

    auto ing_count = dynamic_cast<ui::Text*>(building_info_panel->getChildByName("ingredient_count"));
    this->schedule([create_count, building, ing_count](float dt)
    {
        ing_count->setString(create_count("ING", building->ingredients.size()));
    }, "ing_count_update");

    auto pro_count = dynamic_cast<ui::Text*>(building_info_panel->getChildByName("product_count"));
    this->schedule([create_count, building, pro_count](float dt)
    {
        pro_count->setString(create_count("PRO", building->products.size()));
    }, "pro_count_update");

    auto wst_count = dynamic_cast<ui::Text*>(building_info_panel->getChildByName("waste_count"));
    this->schedule([create_count, building, wst_count](float dt)
    {
        wst_count->setString(create_count("WST", building->wastes.size()));
    }, "wst_count_update");


    return building_info_panel;
};

ui::Button* HarvestScene::create_shop_button()
{

    auto inst = CSLoader::getInstance();
    Node* harvest_scene_editor = inst->createNode("editor/scenes/harvest_scene.csb");

    auto shop_button = ui::Button::create(
        "shop_banner.png",
        "shop_banner_hili.png",
        "shop_banner.png",
        ui::TextureResType::PLIST
    );

    auto shop_text_img = ui::ImageView::create(
        "text_shop.png",
        ui::TextureResType::PLIST
    );

    shop_text_img->setPosition(Vec2(28, 17));
    shop_button->addChild(shop_text_img);
    shop_button->setScale(4);

    Node* shop_pos_node = harvest_scene_editor->getChildByName("shop_pos");
    shop_button->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    shop_button->setPosition(shop_pos_node->getPosition());

    shop_button->addTouchEventListener([](Ref*, ui::Widget::TouchEventType evt)
    {
        if (evt == ui::Widget::TouchEventType::ENDED) {
            auto scene = Scene::create();
            BuyBuildingsNuMenu* building_menu = BuyBuildingsNuMenu::create(GameLogic::getInstance()->beatup);
            scene->addChild(building_menu);

            auto director = Director::getInstance();
            director->pushScene(scene);
        };
    });

    return shop_button;
};

ui::Button* HarvestScene::create_city_button()
{

    auto inst = CSLoader::getInstance();
    Node* harvest_scene_editor = inst->createNode("editor/scenes/harvest_scene.csb");

    auto shop_button = ui::Button::create(
        "shop_banner.png",
        "shop_banner_hili.png",
        "shop_banner.png",
        ui::TextureResType::PLIST
    );

    auto shop_text_img = ui::ImageView::create(
        "text_city.png",
        ui::TextureResType::PLIST
    );

    shop_text_img->setPosition(Vec2(28, 17));
    shop_button->addChild(shop_text_img);
    shop_button->setScale(4);

    Node* shop_pos_node = harvest_scene_editor->getChildByName("city_pos");
    shop_button->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    shop_button->setPosition(shop_pos_node->getPosition());

    shop_button->addTouchEventListener([](Ref*, ui::Widget::TouchEventType evt)
    {
        if (evt == ui::Widget::TouchEventType::ENDED) {
            auto scene = Scene::create();
            CityMenu* building_menu = CityMenu::create();
            scene->addChild(building_menu);

            auto director = Director::getInstance();
            director->pushScene(scene);
        };
    });

    return shop_button;
};
