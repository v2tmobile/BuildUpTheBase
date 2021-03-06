#ifndef HARVESTABLEMANAGER_H
#define HARVESTABLEMANAGER_H

#include <memory>

class Fighter;
class Item;
class Harvestable;

enum class HarvestableTypes
{
    //TODO list them out, and then implement them
};

using res_count_t = long double;

///manages metadata about the harvestables
/// like current progress through the dump
/// eventually I'd like to make it manage the
/// creating and desctruction of the nodes themselves
class HarvestableManager
{
    private:
        //fighter harvestable
        int enemy_cur_hp;//TODO make doubles, so we can get higher than 32 000 hp
        int enemy_max_hp;//TODO make doubles, so we can get higher than 32 000 hp
        int enemy_damage;//TODO make doubles, so we can get higher than 32 000 hp

        //dumpster harvestable
        std::shared_ptr<Item> stored_item;

        Harvestable* _harvestable;
        static HarvestableManager* _instance;

    public:
        static HarvestableManager* getInstance();
        HarvestableManager();

        //generic harvestable
        Harvestable* get_active_harvestable();
        //not necessary so long as we've got a harvestscene active right?
        // void set_active_harvestable(Harvestable* harvestable);

        //fighter harvestable
        bool is_fighter_stored; // true if we want to load an existing fighter
        void store_fighter(std::shared_ptr<Fighter> fighter);
        void load_fighter(std::shared_ptr<Fighter> fighter);
        void reset_fighter();

        //dumpster harvestable
        bool is_item_stored;
        int stored_dumpster_clicks; //TODO use doubles for clicks
        void store_item(std::shared_ptr<Item> item);
        void load_item(std::shared_ptr<Item>& item);
        void reset_item();

        //reserve of scavengers
        res_count_t queued_scavengers;
        //uses against the scavenger being used
        res_count_t current_scavenger_uses;
        //max uses per queued scavenger
        res_count_t max_uses_per_scavenger;
};
#endif
