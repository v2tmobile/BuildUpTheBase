#include "GameLogic.h"

#include "Beatup.h"
#include "HouseBuilding.h"

GameLogic* GameLogic::_instance = NULL;

GameLogic::GameLogic()
{

};

bool GameLogic::init()
{
    GameLogic* instance = GameLogic::getInstance();
    instance->beatup = Beatup::create();
    instance->buildup = instance->beatup->buildup;

    return true;
};

void GameLogic::update(float dt)
{

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
