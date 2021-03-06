#include "StaticData.h"

#include <json/document.h>
#include <json/reader.h>

#include <sstream>

#include "FileOperation.h"

#include "Recipe.h"
#include "Item.h"
#include "Ingredients.h"

std::map<std::string, rjDocument> BaseStaticData::_file_content_cache = {};

BaseStaticData::BaseStaticData(std::string filename)
{
	this->_filename = filename;

    auto it = BaseStaticData::_file_content_cache.find(filename);
    if (it == BaseStaticData::_file_content_cache.end())
    {
        //grab a document from filesystem, cache it
        rjDocument doc = FileIO::open_json(this->_filename, true);
        BaseStaticData::_file_content_cache[filename] = std::move(doc);
    }

    //copy the cached doc into the instance's doc
    this->jsonDoc.CopyFrom(BaseStaticData::_file_content_cache[filename], this->jsonDoc.GetAllocator());
}

BaseStaticData::BaseStaticData(const BaseStaticData&)
{
    //this copy ctor shouldn't be called
    // otherwise TODO figure out a good way to copy this, maybe
    // read a new copy into memory
    assert(false && "dont copy this, write a proper copy constructor");
	auto a = 1 + 1;
};

std::string BaseStaticData::_get_data(std::string key_top, std::string key_child, std::string key_grandchild)
{

    std::stringstream ss;
    ss << key_top << "__" << key_child << "__" << key_grandchild;
    std::string cache_key = ss.str();

    if (this->_cache.find(cache_key) != this->_cache.end())
    {
        return this->_cache.at(cache_key);
    }


    assert(jsonDoc.HasMember(key_top.c_str()) && "this should be here");
    rjValue& body = jsonDoc[key_top.c_str()];

    if (key_child == "")
    {
        this->_cache[cache_key] = body.GetString();
        return body.GetString();
    };

    rjValue& child = body[key_child.c_str()];
    if (key_grandchild == "")
    {
        this->_cache[cache_key] = child.GetString();
        return child.GetString();
    };

    rjValue& grand_child = child[key_grandchild.c_str()];

    this->_cache[cache_key] = grand_child.GetString();
    return grand_child.GetString();
}

BuildingData::BuildingData(std::string building_name)
	: BaseStaticData("buildings_data.json")
{
    this->building_name = building_name;
}

std::string BuildingData::getter(std::string key)
{
    return this->_get_data("buildings", building_name, key);
};

vsRecipe BuildingData::get_all_recipes()
{
	// TODO remove after we're sure this isn't needed
    // //assume if its not an object, its null
    // if (this->recipe_doc.IsObject() == false)
    // {
    //     this->recipe_doc = FileIO::open_json(this->_filename, true);
    // }

    rjValue& body = this->jsonDoc["buildings"];
    rjValue& building_info = body[this->building_name.c_str()];

    if (!building_info.HasMember("recipes"))
    {
        return vsRecipe();
    }
    rjValue& recipe_info = building_info["recipes"];

    vsRecipe recipes;

    for (auto itr = recipe_info.MemberBegin();
        itr != recipe_info.MemberEnd();
        itr += 1)
    {
        std::string recipe_id = itr->name.GetString();
        auto recipe_data = &itr->value;

        recipes.push_back(this->build_recipe(recipe_data));

    };

    return recipes;

};

spRecipe BuildingData::build_recipe(rjValue* raw_data)
{
    rjValue& recipe_ref = *raw_data;
    rjValue& recipe_components = recipe_ref["components"];

    auto recipe_name = recipe_ref["name"].GetString();
    auto recipe_description = recipe_ref["description"].GetString();
    spRecipe result = std::make_shared<Recipe>(recipe_name, recipe_description);

    if (recipe_ref.HasMember("clicks_required"))
    {
        result->clicks_required = recipe_ref["clicks_required"].GetInt();
    }


    for (auto itr = recipe_components.MemberBegin();
         itr != recipe_components.MemberEnd();
         itr+=1)
    {
        std::string val = itr->name.GetString();
        int count = itr->value.GetInt();
        result->components[Ingredient::string_to_type(val)] = count;
    };

    rjValue& recipe_output = recipe_ref["output"];
    for (auto itr = recipe_output.MemberBegin();
         itr != recipe_output.MemberEnd();
         itr+=1)
    {
        std::string val = itr->name.GetString();
        int count = itr->value.GetInt();
        result->outputs[Ingredient::string_to_type(val)] = count;
    };

    return result;
}

spRecipe BuildingData::get_recipe(std::string recipe_key)
{

    auto& body = jsonDoc["buildings"];
    auto& building_info = body[this->building_name.c_str()];
    auto& recipe_info = building_info["recipes"];

    auto& recipe_data = recipe_info[recipe_key.c_str()];

    return build_recipe(&recipe_data);
};


IngredientData::IngredientData(std::string resource_id)
	: BaseStaticData("resources_data.json")
{
    this->resource_id = resource_id;
}

IngredientData::IngredientData(IngredientSubType resource_type)
    : IngredientData(Ingredient::type_to_string(resource_type))
{

}

std::string IngredientData::getter(std::string key)
{
    return this->_get_data("resources", this->resource_id, key);
};

ItemData::ItemData()
	: BaseStaticData("item_data.json")
{
}

std::string ItemData::getter(std::string key)
{
    return "unimplemented";
};

spItem ItemData::build_item(std::string type_name, rjValue* item_data)
{
    auto& item_ref = *item_data;

    auto name = item_ref["name"].GetString();
    auto summary = item_ref["summary"].GetString();
    auto description = item_ref["description"].GetString();
    auto base_cost = item_ref["base_cost"].GetDouble();
    auto scavenge_weight = item_ref["scavenge_weight"].GetDouble();
    auto img_path = item_ref["img_path"].GetString();

    std::string slot_type_raw = item_ref["slot_type"].GetString();
    ItemSlotType slot_type = ItemSlotType::Unset;

    //TODO create a type_to_str for this
    if (slot_type_raw == "combat") {
        slot_type = ItemSlotType::Combat;
    }
    else if (slot_type_raw == "mining") {
        slot_type = ItemSlotType::Mining;
    }
    else if (slot_type_raw == "recipe") {
        slot_type = ItemSlotType::Recipe;
    };

    //create item with default dynamic stats
    auto item = std::make_shared<Item>(type_name, ItemRarityType::Normal, 1);

    item->name = name;
    item->summary = summary;
    item->description = description;
    item->_base_cost = base_cost;
    item->scavenge_weight = scavenge_weight;
    item->img_path = img_path;
    item->slot_type = slot_type;

    return item;
};

spItem ItemData::get_item(std::string item_id)
{

    auto body = &jsonDoc["items"];
    auto item_info = &(*body)[item_id.c_str()];

    return this->build_item(item_id, item_info);
}

vsItem ItemData::get_all_items()
{
    vsItem result;

    auto&& items = jsonDoc["items"];

    for (rjValue::MemberIterator it = items.MemberBegin(); it != items.MemberEnd(); ++it)
    {
        spItem item = this->build_item(it->name.GetString(), &it->value);
        result.push_back(item);
    }
    return result;
};
