// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>

#include "HouseBuilding.h"
#include <sstream>

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

void farm_task(Building* building)
{
    int new_products = 10;
    std::vector<std::shared_ptr<Product>> products = std::vector<std::shared_ptr<Product>>();
    for (int i = 0; i < new_products; i++)
    {
        spProduct p = std::make_shared<Product>("grain");
        building->products.push_back(p);
    };

    building->products.insert(
        building->products.end(),
        products.begin(),
        products.end()
    );

    std::cout << "\tDoing farm stuff, aka generating grain products" << std::endl;
};

void workshop_task(Building* building)
{
    std::cout << "\tDoing workshop stuff" << std::endl;
};

void dump_task(Building* building)
{
    std::cout << "\tDoing dump stuff" << std::endl;
};

void marketplace_task(Building* building)
{
    std::cout << "\tDoing marketplace stuff" << std::endl;
};

void Village::update(float dt)
{
    Updateable::update(dt);

    this->update_buildings(dt);
};


void Village::update_buildings(float dt)
{
    printf("\n\nupdating buildings\n");
    for (std::shared_ptr<Building> building : this->buildings)
    {
        building->update(dt);
    };

};

template<typename from_T, typename to_T>
void transfer(from_T from_vs, to_T to_vs, int quantity)
{
    if (from_vs.size() < quantity)
    {
        std::cout << quantity << " is too many" << std::endl;
        quantity = from_vs.size();
    }

    if (quantity > 0)
    {
        std::move(from_vs.begin(), std::next(from_vs.begin(), quantity), std::back_inserter(to_vs));
    }
};

void Animal::b2b_transfer(Building* from_bldg, Building* to_bldg, Resource::ResourceType res_type, int quantity)
{
    if (res_type == Resource::Ingredient)
    {
        transfer(from_bldg->ingredients, to_bldg->ingredients, quantity);
    }
    else if (res_type == Resource::Product)
    {
        transfer(from_bldg->products, to_bldg->products, quantity);
    }
    else if (res_type == Resource::Waste)
    {
        transfer(from_bldg->wastes, to_bldg->wastes, quantity);
    }
    else
    {
        std::cout << "type not recognized" << std::endl;
    }

};

void Building::update(float dt)
{
    Updateable::update(dt);
    printf("   %s - time at %.f\n", this->name.c_str(), this->update_clock->start_time);
    this->print_inventory();
    if (update_clock->passed_threshold())
    {
        this->do_task();
        update_clock->reset();
    }
    else
    {
        std::cout << "   \twaiting" << std::endl;
    }
    std::cout << std::endl;
};

void Building::print_inventory()
{
    std::stringstream ss;
    ss << "   ";
    ss << "I: " << this->ingredients.size();
    ss << " P: " << this->products.size();
    ss << " W: " << this->wastes.size();
    ss << std::endl;

    std::cout << ss.str();

};

void Building::do_task()
{
    if (this->task)
    {
        this->task(this);
    };
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
            this->player->update(game_clock.start_time);

            game_clock.reset();

            current_ticks = 0;
            start_time = clock() / CLOCKS_PER_SEC;
        }
    }


}

Village* init_city(Buildup* buildup)
{
    auto city = new Village(buildup, "Burlington");

    auto farm = std::make_shared<Building>(city, "The Farm", farm_task);
    farm->update_clock->set_threshold(2*CLOCKS_PER_SEC);
    auto dump= std::make_shared<Building>(city, "The Dump", dump_task);
    auto workshop = std::make_shared<Building>(city, "The Workshop", workshop_task);
    auto marketplace = std::make_shared<Building>(city, "The Marketplace", marketplace_task);

    city->buildings.push_back(farm);
    city->buildings.push_back(dump);
    city->buildings.push_back(workshop);
    city->buildings.push_back(marketplace);

    return city;
};

void Player::update(float dt)
{
    printf("   The Player has %i coins\n", this->coins);
};

int _tmain(int argc, _TCHAR* argv[])
{
    Buildup* buildup = new Buildup();

    auto player = new Player("Jimothy");
    player->coins = 100;
    buildup->player = player;

    auto animal = std::make_shared<Animal>("Tank");

    buildup->city = init_city(buildup);
    buildup->city->update_buildings(0);

    buildup->main_loop();

    std::cout << std::endl << "Done?";
    std::string val;
    std::cin.ignore();
    std::cout << "Peace" << std::endl;



    return 0;
}

