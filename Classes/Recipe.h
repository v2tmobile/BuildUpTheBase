#pragma once
#ifndef RECIPE_H
#define RECIPE_H

#include <map>

#include "constants.h"
#include "Nameable.h"

typedef mistIngredient ComponentMap;
typedef mistIngredient OutputMap;

class Recipe : public Nameable
{
    public:
        int current_clicks;
        int clicks_required;

        ComponentMap components;
        OutputMap outputs;

        VoidFunc _on_recipe_complete;
        void on_recipe_complete();

        std::string description;

        Recipe(std::string name="Unnamed", std::string description="");

        //if there's enough ingredients in the input vector to make recipe
        bool is_satisfied(mistIngredient input);
        void consume();

};

#endif
