// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>

#include "HouseBuilding.h"

Buildup::Buildup()
{
    this->city = NULL;
};

Updateable::Updateable()
{
    this->update_clock = new Clock(15.0f);
};

void Updateable::update(float dt)
{
    this->update_clock->update(dt);
};

void farm_task(const Building* building)
{
    std::cout << "Doing farm stuff" << std::endl;
};

void workshop_task(const Building* building)
{
    std::cout << "Doing workshop stuff" << std::endl;
};

void dump_task(const Building* building)
{
    std::cout << "Doing dump stuff" << std::endl;
};

void marketplace_task(const Building* building)
{
    std::cout << "Doing marketplace stuff" << std::endl;
};

void Village::update(float dt)
{
    Updateable::update(dt);

    this->update_tasks();
};

void Buildup::main_loop()
{
    Clock game_clock = Clock(CLOCKS_PER_SEC);
    clock_t start_time = clock() / CLOCKS_PER_SEC;

    int current_ticks = 0;
    while (true)
    {
        game_clock.update((float)current_ticks);

        current_ticks = clock() / CLOCKS_PER_SEC - start_time;
        if (game_clock.passed_threshold())
        {
            this->city->update(game_clock.start_time);

            game_clock.reset();

            current_ticks = 0;
            start_time = clock() / CLOCKS_PER_SEC;
        }
    }


}

Village* init_city()
{
    auto city = new Village("Burlington");

    auto farm = std::make_shared<Building>("The Farm", farm_task);
    auto dump= std::make_shared<Building>("The Dump", dump_task);
    auto workshop = std::make_shared<Building>("The Workshop", workshop_task);
    auto marketplace = std::make_shared<Building>("The Marketplace", marketplace_task);

    city->buildings.push_back(farm);
    city->buildings.push_back(dump);
    city->buildings.push_back(workshop);
    city->buildings.push_back(marketplace);

    return city;
};


int _tmain(int argc, _TCHAR* argv[])
{
    Buildup* buildup = new Buildup();

    buildup->city = init_city();
    buildup->city->update_tasks();

    auto player = std::make_shared<Player>("Jimothy");
    player->coins = 100;

    auto animal = std::make_shared<Animal>("Tank");

    buildup->main_loop();

    std::cout << std::endl << "Done?";
    std::string val;
    std::cin.ignore();
    std::cout << "Peace" << std::endl;



    return 0;
}

