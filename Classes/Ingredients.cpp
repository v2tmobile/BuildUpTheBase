#include "Ingredients.h"

#include <algorithm>
#include <cassert>
#include <sstream>

#include "constants.h"

const std::map<Ingredient::SubType, std::string> Ingredient::type_map = {
    {Ingredient::SubType::Grain, "grain"},
    {Ingredient::SubType::PileOfGrain, "pileofgrain"},
    {Ingredient::SubType::Bread, "bread"},
    {Ingredient::SubType::Loaf, "loaf"},
    {Ingredient::SubType::Seed, "seed"},
    {Ingredient::SubType::Tree, "tree"},
    {Ingredient::SubType::Wood, "wood"},
    {Ingredient::SubType::Iron, "iron"},
    {Ingredient::SubType::Copper, "copper"},
    {Ingredient::SubType::Fly, "fly"},
    {Ingredient::SubType::Sand, "sand"},
    {Ingredient::SubType::Flesh, "flesh"},
    {Ingredient::SubType::Berry, "berry"},
    {Ingredient::SubType::Soul, "soul"},
    {Ingredient::SubType::Blood, "blood"},
    {Ingredient::SubType::Paper, "paper"},
    {Ingredient::SubType::Undead, "undead"},
    {Ingredient::SubType::Minecart, "minecart"},
    {Ingredient::SubType::MineRails, "minerails"}
};

const std::map<Ingredient::SubType, res_count_t> Ingredient::type_to_value = {
    //mis types
    {Ingredient::SubType::PileOfGrain,  30.0},
    {Ingredient::SubType::Bread,        45.0},
    {Ingredient::SubType::Loaf,         30.0},
    {Ingredient::SubType::Iron,         30.0},
    {Ingredient::SubType::Soul,        100.0},
    {Ingredient::SubType::Undead,       35.0},
    {Ingredient::SubType::Wood,         15.0},
    {Ingredient::SubType::Minecart,    250.0},
    {Ingredient::SubType::MineRails,    50.0},
    {Ingredient::SubType::Tree,          15.0},


    //auto harvested types
    {Ingredient::SubType::Grain,         1.75},
    {Ingredient::SubType::Sand,          1.0},
    {Ingredient::SubType::Blood,         2.5},
    {Ingredient::SubType::Paper,         4.0},
    {Ingredient::SubType::Fly,           1.0},
    {Ingredient::SubType::Seed,          5.50},
    {Ingredient::SubType::Copper,       13.0},
    {Ingredient::SubType::Flesh,         4.50},
    {Ingredient::SubType::Berry,         5.0}
};

const std::map<Ingredient::SubType, res_count_t> Ingredient::type_to_harvest_ratio = {
    //no harvesters for these
    {Ingredient::SubType::PileOfGrain,  1.0},
    {Ingredient::SubType::Bread,        1.0},
    {Ingredient::SubType::Loaf,         1.0},
    {Ingredient::SubType::Iron,         1.0},
    {Ingredient::SubType::Soul,         1.0},
    {Ingredient::SubType::Undead,       1.0},
    {Ingredient::SubType::Minecart,     1.0},
    {Ingredient::SubType::MineRails,    1.0},
    {Ingredient::SubType::Wood,         1.0},

    //only ingredients with auto harvesters
    {Ingredient::SubType::Grain,         2.0},
    {Ingredient::SubType::Sand,          0.6},
    {Ingredient::SubType::Blood,         0.5},
    {Ingredient::SubType::Paper,         0.9},
    {Ingredient::SubType::Fly,           3.0},
    {Ingredient::SubType::Seed,          0.7},
    {Ingredient::SubType::Copper,        0.3},
    {Ingredient::SubType::Flesh,         0.66},
    {Ingredient::SubType::Berry,         0.75},
};

const std::vector<Ingredient::SubType> Ingredient::basic_ingredients = {
    IngredientSubType::Grain, IngredientSubType::Seed, IngredientSubType::Tree,
    IngredientSubType::Fly, IngredientSubType::Sand, IngredientSubType::Flesh,
    IngredientSubType::Berry, IngredientSubType::Blood, IngredientSubType::Paper
};

const std::vector<Ingredient::SubType> Ingredient::advanced_ingredients = {
    IngredientSubType::PileOfGrain, IngredientSubType::Bread, IngredientSubType::Loaf,
    IngredientSubType::Iron, IngredientSubType::Copper, IngredientSubType::Wood,
    IngredientSubType::Soul, IngredientSubType::Undead, IngredientSubType::Minecart,
    IngredientSubType::MineRails
};

std::string Ingredient::type_to_string(Ingredient::SubType type)
{
    std::string result = "none";
    for (auto pair : Ingredient::type_map)
    {
        if (type == pair.first) { return pair.second; }
    }
    return result;
};

Ingredient::SubType Ingredient::string_to_type(std::string string_type)
{
    Ingredient::SubType result = Ingredient::SubType::None;
    std::transform(string_type.begin(), string_type.end(), string_type.begin(), ::tolower);
    for (auto& pair : Ingredient::type_map)
    {
        if (pair.second == string_type)
        {
            return pair.first;
        }
    }

    assert(false && "unknown type"); return result;
};
