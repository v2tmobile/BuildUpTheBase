#pragma once
#ifndef MINER_H
#define MINER_H

#include "cocos2d.h"

using tile_gid_t = uint32_t;

class Miner
{
    public:
        tile_gid_t resource_tile_id = 130;

    public:
        cocos2d::TMXTiledMap* tilemap;
        cocos2d::Vec2 active_tile_pos;

        cocos2d::TMXLayer* active_layer;
        int active_tile_id;

        Miner();

        bool resource_at_tile_pos(cocos2d::Vec2 pos);
        bool is_valid_pos(cocos2d::Vec2 pos);
        tile_gid_t get_tile_gid_at_offset(cocos2d::Vec2 pos, cocos2d::Vec2 offset);

        void move_active_tile(cocos2d::Vec2 pos);
        void move_active_up();
        void move_active_down();
        void move_active_left();
        void move_active_right();
};

#endif