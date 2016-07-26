#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <random>

#include "cocos2d.h"

float sx(float x, bool do_scale=false);
float sy(float y, bool do_scale=false);
cocos2d::Vec2 sv(cocos2d::Vec2 input); //note do_scale is true

cocos2d::Vec2 get_relative(cocos2d::Size input, cocos2d::Vec2 relative_pos=cocos2d::Vec2::ANCHOR_MIDDLE);

template<typename T>
T pick_one(std::vector<T> vec, std::mt19937* generator=NULL)
{
    //create the generator if it doesnt exist
    if (!generator) { 
        auto temp_gen = std::mt19937(std::random_device{}());
         generator = &temp_gen;
    };

    int size = vec.size();

    std::uniform_int_distribution<int> distribution(0, size-1);
    auto index = distribution(*generator);

    return vec.at(index);
};

void log_vector(cocos2d::Vec2 vector, std::string message="");

template<typename mapT>
typename mapT::mapped_type map_get(mapT& input_map, typename mapT::key_type key, typename mapT::mapped_type default_result)
{
    if (input_map.count(key)){
        return input_map[key];
    } else {
        return default_result;
    };
};

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::string beautify_double(long double& value);
void test_beautify_double();

void try_set_enabled(cocos2d::ui::Widget* widget, bool new_value);

template<typename number_t>
number_t scale_number(number_t base_number, number_t current_number, number_t rate)
{
    //base * (rate**current)
    return base_number * std::pow(rate, std::max(static_cast<number_t>(0), current_number));
};

#endif
