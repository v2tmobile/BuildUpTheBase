#pragma once
#ifndef WORKER_H
#define WORKER_H

#include "constants.h"

#include "Updateable.h"
#include "Nameable.h"

#include "SubTypes.h"


#include "Ingredients.h"

class ResourceCondition : public Nameable
{
    public:
        Resource::ResourceType type_choice;

        Ingredient::SubType ing_type;

        int quantity;

        ResourceCondition(Resource::ResourceType type_choice, int quantity, std::string name) : Nameable(name) {
            this->type_choice = type_choice;
            this->quantity = quantity;
        };

        static ResourceCondition* create_ingredient_condition(Ingredient::SubType ing_type, int quantity, std::string condition_name);

        bool is_satisfied(spBuilding building);
};


//if this is satisfied, the action can happen
class Condition : public Nameable
{

};

//
class Action : public Nameable
{

};

//
//attaches to a building and does something once certain conditions are hit
//

class Worker : public Nameable, Updateable
{
    public:
        using SubType = WorkerSubType;
        SubType sub_type;

        spBuilding building;

        //amount of workers of this type, ie if this unlocks 1 grain per update,
        //and there's 10 active, it'll get you 10 grain
        res_count_t active_count = 0; 

        Worker(spBuilding building, std::string name, SubType sub_type);

        void update(float dt);
        virtual void on_update(float dt);
};

class Harvester : public Worker
{
    public:
        Ingredient::SubType ing_type;

        Harvester(spBuilding building, std::string name, Ingredient::SubType ing_type, SubType sub_type);

        virtual void on_update(float dt) override;

        static res_count_t get_base_shop_cost(SubType harv_type);
        static res_count_t get_to_harvest_count(SubType harv_type);
};

class Salesman : public Harvester
{
    public:
        Salesman(spBuilding building, std::string name, Ingredient::SubType ing_type, SubType sub_type);

        virtual void on_update(float dt) override;
        static res_count_t get_base_shop_cost(SubType sub_type);
        static res_count_t get_to_sell_count(SubType sub_type);
};

class ConsumerHarvester : public Harvester
{
    public:
        ConsumerHarvester(spBuilding building, std::string name, Ingredient::SubType ing_type, SubType sub_type);
        virtual void on_update(float dt) override;

};

typedef std::pair<Worker::SubType, Ingredient::SubType> work_ing_t;
typedef std::map<work_ing_t, res_count_t> mistHarvester;
typedef std::map<Worker::SubType, res_count_t> mistWorkerSubType;

#endif
