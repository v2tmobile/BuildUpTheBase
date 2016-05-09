#include "GameLogic.h"

#include "Beatup.h"
#include "HouseBuilding.h"
#include "DataManager.h"
#include "NuMenu.h"

GameLogic* GameLogic::_instance = NULL;

GameLogic::GameLogic()
{

};

bool GameLogic::init()
{
    GameLogic* instance = GameLogic::getInstance();
    instance->beatup = Beatup::create();
    instance->beatup->setName("beatup");
    instance->beatup->retain();
    instance->buildup = instance->beatup->buildup;

    return true;
};

void GameLogic::update(float dt)
{
    this->beatup->update_buildup(dt);
};

GameLogic* GameLogic::getInstance()
{
    if (!GameLogic::_instance)
    {
        GameLogic::_instance = new GameLogic();
        GameLogic::_instance->init();
    };

    return GameLogic::_instance;
};

void GameLogic::add_total_harvests(int value)
{
    DataManager::incr_key("total_harvests", value);
};

int GameLogic::get_total_harvests()
{
    return DataManager::get_int_from_data("total_harvests");
};


void GameDirector::switch_to_building_menu()
{
    auto scene = cocos2d::Scene::create();
    BuildingNuMenu* building_menu = BuildingNuMenu::create(
        BUILDUP->get_target_building()
        );
    scene->addChild(building_menu);

    auto director = cocos2d::Director::getInstance();
    director->pushScene(scene);
};