#include "Serializer.h"
#include <string>
#include <sstream>

#include <json/document.h>
#include <json/stringbuffer.h>

#include "FileOperation.h"

#include "HouseBuilding.h"
#include "Buildup.h"

#include "Technology.h"
#include "GameLogic.h"
#include "Item.h"
#include "StaticData.h"
#include "Util.h"
#include "Miner.h"
#include "Equipment.h"
#include "goals/Achievement.h"
#include "Worker.h"
#include "Ingredients.h"

#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXLayer.h"
#include "HarvestableManager.h"
#include "banking/Bank.h"

BaseSerializer::BaseSerializer(std::string filename)
    : filename(filename)
{

};

rjDocument BaseSerializer::get_document(std::string filename) const
{
    if (filename == "") filename = this->filename;

    return FileIO::open_json(filename, false);
}

void BaseSerializer::save_document(rjDocument& doc, std::string filename) const
{
    if (filename == "") filename = this->filename;

    FileIO::save_json(filename, doc, false);
}

void BaseSerializer::add_member(rjDocument& doc, std::string key, rjValue& value)
{
    //build the Values that become the key and values
    auto& allocator = doc.GetAllocator();
    rjValue doc_key = rjValue();

    //NOTE need to use allocator here, AND move the key later it seems like,
    //otherwise the value gets cleaned up
    doc_key.SetString(key.c_str(), key.size(), allocator);

    //all the member to the document
    this->_add_member(doc, doc_key, value, allocator);
};

rjValue& BaseSerializer::get_member(rjDocument& doc, std::string key)
{
    //build the Values that become the key and values
    rjDocument::AllocatorType& allocator = doc.GetAllocator();
    rjValue doc_key = rjValue();

    doc_key.SetString(key.c_str(), key.size());

    return this->_get_member(doc, doc_key, allocator);
};

void BaseSerializer::_add_member(rjDocument& doc, rjValue& key, rjValue& value, rapidjson::CrtAllocator& allocator)
{
    doc.AddMember(key.Move(), value.Move(), allocator);
};

rjValue& BaseSerializer::_get_member(rjDocument& doc, rjValue& key, rjDocument::AllocatorType& allocator)
{
    if (doc.HasMember(key) == false)
    {
        return doc; //null placeholder
    }
    rjValue& value = doc[key];
    return value;
};

void BaseSerializer::set_string(rjDocument & doc, std::string key, std::string value)
{
    rjDocument::AllocatorType& allocator = doc.GetAllocator();
    rjValue doc_value = rjValue();
    doc_value.SetString(value.c_str(), value.size(), allocator);

    this->add_member(doc, key, doc_value);

};

void BaseSerializer::set_double(rjDocument & doc, std::string key, double value)
{
    rjValue doc_value = rjValue();
    doc_value.SetDouble(value);

    this->add_member(doc, key, doc_value);

};

void BaseSerializer::set_int(rjDocument & doc, std::string key, int value)
{
    rjValue doc_value = rjValue();
    doc_value.SetInt(value);

    this->add_member(doc, key, doc_value);
};

std::string BaseSerializer::get_string(rjDocument & doc, std::string key, std::string fallback)
{
    rjValue& doc_value = this->get_member(doc, key);
    if (doc_value.IsObject())
    {
        //assume its returned a Document, since I don't know how to return a null reference
        return fallback;
    }
    return doc_value.GetString();

};

double BaseSerializer::get_double(rjDocument & doc, std::string key, double fallback)
{
    auto& doc_value = this->get_member(doc, key);
    if (doc_value.IsObject())
    {
        //assume its returned a Document, since I don't know how to return a null reference
        return fallback;
    }
    return doc_value.GetDouble();

};

int BaseSerializer::get_int(rjDocument & doc, std::string key, int fallback)
{
    rjValue& doc_value = this->get_member(doc, key);
    if (doc_value.IsObject())
    {
        //assume its returned a Document, since I don't know how to return a null reference
        return fallback;
    }
    return doc_value.GetInt();
};

BuildingSerializer::BuildingSerializer(std::string filename)
    : BaseSerializer(filename), building(NULL)
{
}

void BuildingSerializer::serialize(spBuilding building)
{
    rjDocument doc = rjDocument();
    doc.SetObject();

    this->building = building;

    this->serialize_building_level(doc);
    this->serialize_workers(doc);
    this->serialize_tech(doc);

    this->save_document(doc);
}

void BuildingSerializer::load(spBuilding building)
{
    rjDocument doc = this->get_document();
    //dont do work if there's nothing to do
    if (doc.IsObject() == false){ return; }

    this->building = building;

    this->load_building_level(doc);
    this->load_ingredients(doc); //TODO remove this after like halfway through november 2016 so everyone has all their resources still
    this->load_workers(doc);
    this->load_tech(doc);
}

void BuildingSerializer::serialize_building_level(rjDocument& doc)
{
    this->set_int(doc, "building_level", this->building->get_building_level());
}

void BuildingSerializer::load_building_level(rjDocument& doc)
{
    //dont do work if there's nothing to do
    if (doc.IsObject() == false){ return; }

    int new_level = this->get_int(doc, "building_level", -1);
    if (new_level != -1)
    {
        //only used saved building level
        this->building->set_building_level(new_level);
    }
}


void BuildingSerializer::load_ingredients(rjDocument& doc)
{
    //dont do work if there's nothing to do
    if (doc.IsObject() == false){ return; }

    for (std::pair<Ingredient::SubType, std::string> pair : Ingredient::type_map)
    {
       // TODO this isn't res_count_t aka long double, so we are going to lose data eventually!!!!
       double ing_count = this->get_double(doc, pair.second, -1);
       if (ing_count != -1)
       {
           //TODO this uses all buildings, but we should make it more explicit
           //at some point
           this->building->create_ingredients(pair.first, ing_count);
       }
    }
}

void BuildingSerializer::serialize_workers(rjDocument& doc)
{
    auto save_worker = [&](std::string prefix, mistHarvester& workers, std::map<std::pair<Worker::SubType, Ingredient::SubType>, res_count_t>::value_type mist) {
        std::stringstream ss;
        WorkerSubType worker_type = mist.first.first;
        IngredientSubType ing_type = mist.first.second;
        res_count_t count = mist.second;

        ss << prefix << "_" << Ingredient::type_to_string(ing_type) << "_" << static_cast<int>(worker_type);
        this->set_double(doc, ss.str(), (double)count); //TODO fix res_count_t to double data loss here
    };

    for (auto mist : this->building->harvesters)
    {
        save_worker("harvester", this->building->harvesters, mist);
    }

    for (auto mist : this->building->salesmen)
    {
        save_worker("salesmen", this->building->salesmen, mist);
    }

    for (auto mist : this->building->consumers)
    {
        save_worker("consumer", this->building->consumers, mist);
    }

    for (auto mist : this->building->scavengers)
    {
        save_worker("scavenger", this->building->scavengers, mist);
    }
}

void BuildingSerializer::load_workers(rjDocument& doc)
{
    //dont do work if there's nothing to do
    if (doc.IsObject() == false){ return; }

    auto load_worker = [&](std::string prefix, mistHarvester& workers, std::string& type_str, int& i, IngredientSubType& ing_type) {
        std::stringstream ss;
        ss << prefix << "_" << type_str << "_" << i;

        double default_unset = -1;
        double harv_count = this->get_double(doc, ss.str(), default_unset);

        if (harv_count != default_unset)
        {
            std::pair<WorkerSubType, Ingredient::SubType> type_pair = { static_cast<WorkerSubType>(i), ing_type };
            workers[type_pair] = harv_count;
        };
    };

    //NOTE this goes through each ingredient, then for each ing, makes up to 10
    //queries on the doc per worker type. this could get slow, but C++ is great so who knows
    for (std::pair<Ingredient::SubType, std::string> pair : Ingredient::type_map)
    {
        Ingredient::SubType ing_type = pair.first;
        std::string type_str = pair.second;

        for (int i = 1; i <= 20; i++){
            load_worker("harvester", this->building->harvesters, type_str, i, ing_type);
            load_worker("salesmen", this->building->salesmen, type_str, i, ing_type);
            load_worker("consumer", this->building->consumers, type_str, i, ing_type);
            load_worker("scavenger", this->building->scavengers, type_str, i, ing_type);
        };
    }
}

void BuildingSerializer::serialize_tech(rjDocument& doc)
{
    auto save_tech = [&](std::string prefix, std::map<TechSubType, res_count_t>::value_type mist) {
        std::stringstream ss;
        TechSubType worker_type = mist.first;
        res_count_t count = mist.second;

        ss << prefix << "_" << Technology::type_to_string(worker_type);
        this->set_double(doc, ss.str(), (double)count); //TODO fix res_count_t to double data loss here
    };

    for (auto mist : this->building->techtree->get_tech_map())
    {
        save_tech("tech", mist);
    }

}

void BuildingSerializer::load_tech(rjDocument& doc)
{
    //dont do work if there's nothing to do
    if (doc.IsObject() == false){ return; }

    auto load_tech = [&](std::string prefix, TechMap& tech_map, std::string& type_str, TechSubType& ing_type) {
        std::stringstream ss;
        ss << prefix << "_" << type_str;
        double harv_count = this->get_double(doc, ss.str(), -1);
        if (harv_count != -1)
        {
            tech_map[ing_type] = harv_count;
        };
    };

    for (std::pair<Technology::SubType, std::string> pair : Technology::type_map)
    {
        Technology::SubType tech_type = pair.first;
        std::string type_str = pair.second;

        load_tech("tech", this->building->techtree->tech_map, type_str, tech_type);
    }
}

void BuildingSerializer::_add_member(rjDocument& doc, rjValue& key, rjValue& value, rjDocument::AllocatorType& allocator)
{
    if (doc.HasMember(this->building->name.c_str()) == false)
    {
        doc.AddMember(
            rjValue().SetString(rj::StringRef(building->name.c_str())),
            rjValue(rj::kObjectType).Move(),
            allocator
        );
    }
    doc[building->name.c_str()].AddMember(key.Move(), value.Move(), allocator);
};

rjValue& BuildingSerializer::_get_member(rjDocument& doc, rjValue& key, rjDocument::AllocatorType& allocator)
{
    if (doc.HasMember(this->building->name.c_str()) == false ||
        doc[this->building->name.c_str()].HasMember(key) == false)
    {
        return doc;
    }
    return doc[building->name.c_str()][key];
};

ItemSerializer::ItemSerializer(std::string filename)
    : BaseSerializer(filename)
{
}

void ItemSerializer::serialize()
{

    //NOTE this nukes the existing items array
    auto doc = this->get_document();
    auto allocator = doc.GetAllocator();
    doc.SetArray();
    auto build_str_member = [&allocator](rjValue& row, std::string key, std::string value){

        rjValue rj_key = rjValue();
        auto key_str = key.c_str();
        auto key_len = key.length();
        rj_key.SetString(key_str, key_len, allocator);

        rjValue rj_value = rjValue();
        auto value_str = value.c_str();
        auto value_len = value.length();
        rj_value.SetString(value_str, value_len, allocator);


        row.AddMember(rj_key.Move(), rj_value.Move(), allocator);
    };
    auto build_dbl_member = [&allocator](rjValue& row, std::string key, res_count_t value){

        rjValue rj_key = rjValue();
        auto key_str = key.c_str();
        auto key_len = key.length();
        rj_key.SetString(key_str, key_len, allocator);

        rjValue rj_value = rjValue();
        rj_value.SetDouble(value);


        row.AddMember(rj_key.Move(), rj_value.Move(), allocator);
    };

    for (spItem item : EQUIPMENT->inventory)
    {
        rjValue row = rjValue();
        row.SetObject();

        build_str_member(row, "type_name", item->type_name);
        build_str_member(row, "rarity", ITEM_RARITY_STRINGS.at(item->rarity));
        build_dbl_member(row, "level", item->level);

        doc.PushBack(row, allocator);
    }

    this->save_document(doc);
}

void ItemSerializer::load()
{
    EQUIPMENT->inventory = {};
    auto doc = this->get_document();

    ItemData item_data;
    if (doc.IsArray())
    {
        for (auto it = doc.Begin(); it != doc.End(); it++)
        {
            rjValue& data = *it;

            std::string type_name = data["type_name"].GetString();
            std::string rarity_str = data["rarity"].GetString();
            res_count_t item_level = data["level"].GetDouble();

            auto result_it = std::find_if(ITEM_RARITY_STRINGS.begin(), ITEM_RARITY_STRINGS.end(), [rarity_str](std::pair<ItemRarityType, std::string> pair){return pair.second == rarity_str; });
            ItemRarityType rarity = ItemRarityType::Unset;
            if (result_it != ITEM_RARITY_STRINGS.end())
            {
                rarity = (*result_it).first;
            }

            spItem item = item_data.get_item(type_name);
            item->rarity = rarity;
            item->level = item_level;
            EQUIPMENT->inventory.push_back(item);
        }
        CCLOG("found an array of items for Items, as expected");
    }
    else
    {
        CCLOG("item doc is not an array, so we're assuming it doesnt exist");
        //assert(false && "item json is not an array");
    }
}

void ItemSerializer::serialize_item(spItem item)
{
}

void ItemSerializer::load_item(rjValue& item_value)
{
}

void ItemSerializer::_add_member(rjDocument& doc, rjValue& key, rjValue& value, rjDocument::AllocatorType& allocator)
{
}

rjValue& ItemSerializer::_get_member(rjDocument& doc, rjValue& key, rjDocument::AllocatorType& allocator)
{
    CCLOG("WARNING THIS SHOULD NOT BE CALLED");
    auto temp = new rjValue();
    return *temp;
}

IngredientSerializer::IngredientSerializer(std::string filename)
    : BaseSerializer(filename)
{
};

void IngredientSerializer::serialize()
{
    rjDocument doc = rjDocument();
    doc.SetObject();

    rjDocument::AllocatorType& allocator = doc.GetAllocator();

    for (std::pair<Ingredient::SubType, res_count_t> mist : BUILDUP->get_all_ingredients())
    {
        this->set_double(doc, Ingredient::type_to_string(mist.first), mist.second);
    };

    this->save_document(doc);
}

void IngredientSerializer::load()
{
    rjDocument doc = this->get_document();
    //dont do work if there's nothing to do
    if (doc.IsObject() == false){ return; }

    auto& all_ingredients = BUILDUP->get_all_ingredients();
    for (std::pair<Ingredient::SubType, std::string> type_str : Ingredient::type_map)
    {
        //TODO make this res_count_t instead
        double saved_ing_count = this->get_double(doc, type_str.second, -1);
        if (saved_ing_count != -1)
        {
            all_ingredients[type_str.first] = saved_ing_count;
        }
    }

}
MinerSerializer::MinerSerializer(std::string filename, Miner* miner)
    : BaseSerializer(filename), miner(miner), existing_json_found(false)
{
};

void MinerSerializer::serialize()
{
    rjDocument doc = rjDocument();
    doc.SetObject();

    rjDocument::AllocatorType& allocator = doc.GetAllocator();

    rjValue all_layers = rjValue(rj::kArrayType);

    for (auto child : this->miner->tilemap->getChildren())
    {
        auto layer = dynamic_cast<cocos2d::TMXLayer*>(child);
        if (layer != NULL)
        {
            auto serialized_tiles = this->serialize_layer(layer);
            rjValue layer_array = rjValue(rapidjson::kArrayType);
            for (auto& tile_id : serialized_tiles)
            {
                rjValue tile_val = rjValue(rapidjson::kNumberType);
                tile_val.SetInt(tile_id);
                layer_array.PushBack(tile_val, allocator);
            }
            all_layers.PushBack(layer_array, allocator);
        }
    }

    rjValue layer_key = rjValue("all_layers");
    doc.AddMember(layer_key, all_layers, allocator);

    this->set_double(doc, "prev_active_tile_pos_x", this->miner->prev_active_tile_pos.x);
    this->set_double(doc, "prev_active_tile_pos_y", this->miner->prev_active_tile_pos.y);

    this->set_double(doc, "active_tile_pos_x", this->miner->active_tile_pos.x);
    this->set_double(doc, "active_tile_pos_y", this->miner->active_tile_pos.y);

    this->set_double(doc, "altar_tile_pos_x", this->miner->altar_tile_pos.x);
    this->set_double(doc, "altar_tile_pos_y", this->miner->altar_tile_pos.y);

    this->set_double(doc, "chance_tile_pos_x", this->miner->chance_tile_pos.x);
    this->set_double(doc, "chance_tile_pos_y", this->miner->chance_tile_pos.y);

    this->set_double(doc, "depth", this->miner->depth);

    this->save_document(doc);
}

std::vector<tile_gid_t> MinerSerializer::serialize_layer(cocos2d::TMXLayer* layer)
{
    cocos2d::Size layer_size = layer->getLayerSize();

    std::vector<tile_gid_t> tiles = std::vector<tile_gid_t>();
    tiles.reserve(int(layer_size.height*layer_size.width));

    for (tile_gid_t y = 0; y < layer_size.height; y++)
    {
        for (tile_gid_t x = 0; x < layer_size.width; x++)
        {
            tile_gid_t pos = static_cast<int>(x + layer_size.width * y);
            tile_gid_t gid = layer->getTiles()[pos];
            tiles.push_back(gid);
        }
    }

    return tiles;
};

void MinerSerializer::load()
{
    rjDocument doc = this->get_document();
    //dont do work if there's nothing to do
    if (doc.IsObject() == false)
    {
        CCLOG("NOT OBJECT");
        this->existing_json_found = false;
        return;
    }

    std::vector<std::string> required_members = {
        "all_layers",
        "prev_active_tile_pos_x",
        "prev_active_tile_pos_y",
        "active_tile_pos_x",
        "active_tile_pos_y",
        "altar_tile_pos_x",
        "altar_tile_pos_y",
        "chance_tile_pos_x",
        "chance_tile_pos_y",
        "depth"
    };

    for (auto& required_member : required_members)
    {
        if (doc.HasMember(required_member.c_str()) == false)
        {
            CCLOG("MISSING MEMBER %s", required_member.c_str());
            this->existing_json_found = false;
            return;
        };
    };

    rjValue& all_layers = doc["all_layers"];

    //through toplevel arrays, each one being a layer
    int layer_index = 0;
    for (rjValue::ConstValueIterator layer_array_it = all_layers.Begin(); layer_array_it != all_layers.End(); ++layer_array_it)
    {
        cocos2d::TMXLayer* current_layer = dynamic_cast<cocos2d::TMXLayer*>(this->miner->tilemap->getChildByTag(layer_index));
        if (!current_layer)
        {
            CCLOG("invalid layer");
            this->existing_json_found = false;
            return;
        };

        std::vector<tile_gid_t> tiles;

        //grab the tiles from the json array for the current layer
        tile_gid_t tile_index = 0;
        for (rjValue::ConstValueIterator tile_value_it = layer_array_it->Begin(); tile_value_it != layer_array_it->End(); ++tile_value_it)
        {
            tile_gid_t tile_value = tile_value_it->GetInt();
            tiles.push_back(tile_value);

            tile_index++;

        };

        //update current layers' tiles
        tile_index = 0;
        cocos2d::Size layer_size = current_layer->getLayerSize();
        for (tile_gid_t y = 0; y < layer_size.height; y++)
        {
            for (tile_gid_t x = 0; x < layer_size.width; x++)
            {
                tile_gid_t new_val = tiles.at(tile_index);
                cocos2d::Vec2 new_pos = { float(x), float(y) };
                current_layer->setTileGID(new_val, new_pos);
                tile_index++;
            }
        };
        layer_index++;
    }

    this->miner->prev_active_tile_pos.x = doc["prev_active_tile_pos_x"].GetDouble();
    this->miner->prev_active_tile_pos.y = doc["prev_active_tile_pos_y"].GetDouble();
    this->miner->active_tile_pos.x = doc["active_tile_pos_x"].GetDouble();
    this->miner->active_tile_pos.y = doc["active_tile_pos_y"].GetDouble();

    this->miner->altar_tile_pos.x = doc["altar_tile_pos_x"].GetDouble();
    this->miner->altar_tile_pos.y = doc["altar_tile_pos_y"].GetDouble();

    this->miner->chance_tile_pos.x = doc["chance_tile_pos_x"].GetDouble();
    this->miner->chance_tile_pos.y = doc["chance_tile_pos_y"].GetDouble();

    this->miner->depth = doc["depth"].GetDouble();

    CCLOG("done loading");
    this->existing_json_found = true;
}

EquipmentSerializer::EquipmentSerializer(std::string filename)
    : BaseSerializer(filename)
{
};

void EquipmentSerializer::serialize()
{
    rjDocument doc = rjDocument();
    doc.SetObject();

    rjDocument::AllocatorType& allocator = doc.GetAllocator();

    //finds the item in EQUIPMENT->inventory and returns its index
    auto get_index_of_item = [](std::shared_ptr<EquipmentSlot>& slot) {
        if (slot->has_item())
        {
            auto items = EQUIPMENT->inventory;
            auto itr = std::find(items.begin(), items.end(), slot->get_item());

            return std::distance(items.begin(), itr);
        }
        return -1;
    };

    //TODO use some sort of id, index is weak
    this->set_int(doc, "combat_slot", get_index_of_item(EQUIPMENT->combat_slot));
    this->set_int(doc, "mining_slot", get_index_of_item(EQUIPMENT->mining_slot));
    this->set_int(doc, "recipe_slot", get_index_of_item(EQUIPMENT->recipe_slot));

    this->save_document(doc);
}

void EquipmentSerializer::load()
{
    rjDocument doc = this->get_document();
    //dont do work if there's nothing to do
    if (doc.IsObject() == false)
    {
        CCLOG("NOT OBJECT");
        return;
    }

    auto get_item_by_index = [](int index) {
        if (index != -1) {
            spItem item = EQUIPMENT->inventory.at(index);
            return item;
        };

        return spItem();
    };

    auto& equipment = EQUIPMENT;
    rjValue combat_slot_key = rjValue("combat_slot");
    if (doc.HasMember(combat_slot_key))
    {
        auto raw_combat_slot = doc[combat_slot_key].GetInt();
        equipment->combat_slot->set_item(get_item_by_index(raw_combat_slot));
    };

    rjValue mining_slot_key = rjValue("mining_slot");
    if (doc.HasMember(mining_slot_key))
    {
        auto raw_mining_slot = doc[mining_slot_key].GetInt();
        equipment->mining_slot->set_item(get_item_by_index(raw_mining_slot));
    };

    rjValue recipe_slot_key = rjValue("recipe_slot");
    if (doc.HasMember(recipe_slot_key))
    {
        auto raw_recipe_slot = doc[recipe_slot_key].GetInt();
        equipment->recipe_slot->set_item(get_item_by_index(raw_recipe_slot));
    };


    CCLOG("done loading");
}


AchievementSerializer::AchievementSerializer(std::string filename)
    : BaseSerializer(filename)
{
};

void AchievementSerializer::serialize()
{
    rjDocument doc = rjDocument();
    doc.SetObject();

    rjDocument::AllocatorType& allocator = doc.GetAllocator();

    auto achievement_manager = AchievementManager::getInstance();
    for (auto& achv : achievement_manager->getAchievements()){
        std::string type_str = achv->type_to_string(achv->achievement_type);
        CountAchievement* count_achievement = dynamic_cast<CountAchievement*>(achv.get());
        if (count_achievement != NULL)
        {
            this->set_double(doc, type_str, count_achievement->_current_count);
        }
        else
        {
            CCLOG("Achievement '%s' skipped serializing, not count type", type_str);
        }
    }

    this->save_document(doc);
}

void AchievementSerializer::load()
{
    rjDocument doc = this->get_document();
    //dont do work if there's nothing to do
    if (doc.IsObject() == false)
    {
        CCLOG("NOT OBJECT");
        return;
    }

    auto achievement_manager = AchievementManager::getInstance();
    for (auto& achv : achievement_manager->getAchievements()){
        std::string type_str = achv->type_to_string(achv->achievement_type);
        CountAchievement* count_achievement = dynamic_cast<CountAchievement*>(achv.get());
        if (count_achievement != NULL)
        {
            //load the achievement progress
            double stored_count = this->get_double(doc, type_str);
            count_achievement->_current_count = stored_count;

            //if satisfied dont celebrate again
            if (count_achievement->satisfied_completion()){
                count_achievement->_celebrated = true;
                count_achievement->_completed = true;
            };
        }
        else
        {
            CCLOG("Achievement '%s' skipped loading, not count type", type_str);
        }
    }

}

void HarvestableManagerSerializer::serialize()
{
    rjDocument doc = rjDocument();
    doc.SetObject();

    rjDocument::AllocatorType& allocator = doc.GetAllocator();

    HarvestableManager* h_man = HarvestableManager::getInstance();
    this->set_double(doc, "queued_scavengers", h_man->queued_scavengers);
    this->set_double(doc, "current_scavenger_uses", h_man->current_scavenger_uses);
    this->set_double(doc, "max_uses_per_scavenger", h_man->max_uses_per_scavenger);

    this->save_document(doc);
}

void HarvestableManagerSerializer::load()
{
    rjDocument doc = this->get_document();
    //dont do work if there's nothing to do
    if (doc.IsObject() == false)
    {
        CCLOG("NOT OBJECT");
        return;
    }

    HarvestableManager* h_man = HarvestableManager::getInstance();
    h_man->queued_scavengers = this->get_double(doc, "queued_scavengers", 0);
    h_man->current_scavenger_uses = this->get_double(doc, "current_scavenger_uses", 0);
    h_man->max_uses_per_scavenger = this->get_double(doc, "max_uses_per_scavenger", 50);

}

void BankSerializer::serialize()
{
    rjDocument doc = rjDocument();
    doc.SetObject();

    rjDocument::AllocatorType& allocator = doc.GetAllocator();

    Bank* bank = Bank::getInstance();
    this->set_double(doc, "total_coins_banked", bank->_total_coins_banked);

    this->save_document(doc);
}

void BankSerializer::load()
{
    rjDocument doc = this->get_document();
    //dont do work if there's nothing to do
    if (doc.IsObject() == false)
    {
        CCLOG("NOT OBJECT");
        return;
    }

    Bank* bank = Bank::getInstance();
    bank->_total_coins_banked = this->get_double(doc, "total_coins_banked", 0);

}
