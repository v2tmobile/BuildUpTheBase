#pragma once
#ifndef MINER_H
#define MINER_H

#include <vector>

#include "math/Vec2.h"

using tile_gid_t = uint32_t;


namespace cocos2d
{
    class Node;

    class TMXTiledMap;
    class TMXLayer;
}

class Miner
{
    public:
        static tile_gid_t resource_tile_id;

        //special tiles
        static tile_gid_t tile_X;
        static tile_gid_t tile_START;

        //across
        static tile_gid_t tile_TL_BR;
        static tile_gid_t tile_BL_TR;

        //corners
        static tile_gid_t tile_TL_TR;
        static tile_gid_t tile_TR_BR;
        static tile_gid_t tile_BL_BR;
        static tile_gid_t tile_TL_BL;


    public:
        cocos2d::TMXTiledMap* tilemap;
        cocos2d::TMXLayer* active_layer;

        cocos2d::Vec2 active_tile_pos;
        cocos2d::Vec2 prev_active_tile_pos;
        cocos2d::Vec2 resource_tile_pos;

        cocos2d::Node* parent;
        Miner(cocos2d::Node* parent);
        void init(bool use_existing);
        void init_start_pos(cocos2d::Vec2 new_start_pos);
        void reset();

        cocos2d::Vec2 get_start_pos();
        cocos2d::Vec2 get_default_start_pos();
        cocos2d::Vec2 get_existing_start_pos();



        bool get_tile_is_blocked_pos(cocos2d::Vec2 pos);
        bool is_valid_pos(cocos2d::Vec2 pos);
        tile_gid_t get_tile_gid_at_offset(cocos2d::Vec2 pos, cocos2d::Vec2 offset);

        void move_active_tile(cocos2d::Vec2 pos);
        void move_active_top_right();
        void move_active_bottom_left();
        void move_active_top_left();
        void move_active_bottom_right();

        //check if tiles around the resource tiles are rails
        // TODO: use a param for the tile instead of looking for it
        bool rails_connect_a_resource();
};

#endif
