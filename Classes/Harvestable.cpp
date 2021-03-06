#include "Harvestable.h"

#include <numeric>

#include "FShake.h"

#include "HouseBuilding.h"
#include "Buildup.h"
#include "ShatterNode.h"
#include "MiscUI.h"
#include "GameLogic.h"
#include "Util.h"
#include "Recipe.h"
#include "attribute.h"
#include "Fighter.h"
#include "attribute_container.h"
#include "ProgressBar.h"
#include "Technology.h"
#include "PopupPanel.h"
#include "StaticData.h"
#include "Item.h"
#include "RandomWeightMap.h"
#include "NodeBuilder.h"
#include "BuildingsSideBar.h"
#include "goals/Achievement.h"
#include "combat.h"
#include "Equipment.h"
#include "HarvestableManager.h"
#include "utilities/vibration.h"
#include "HarvestScene.h"
#include "Ingredients.h"
#include "SoundEngine.h"
#include "Modal.h"
#include "house_building/BuildingTypes.h"

#include "2d/CCClippingNode.h"
#include "2d/CCActionEase.h"
#include "base/CCDirector.h"
#include "2d/CCActionInstant.h"
#include "2d/CCRenderTexture.h"
#include "2d/CCSpriteFrameCache.h"
#include "2d/CCParticleSystemQuad.h"
#include "ui/UIText.h"
#include "ui/UITextBMFont.h"
#include "ui/UIButton.h" //button is used somewhere
#include "ui/UIImageView.h"
#include "ui/UIListView.h"
#include "ui/UIScale9Sprite.h"
#include "Gestures/LongPressGestureRecognizer.h"
#include "Gestures/GestureRecognizerUtils.h"
#include "Tutorial.h"
#include "2d/CCLabel.h"

Size Harvestable::get_sprite_size()
{
    return this->sprite->getContentSize() * this->sprite->getScale();
}

static unsigned int COUNT = 0;

void Harvestable::init_sprite()
{
    this->sprite = cocos2d::Sprite::createWithSpriteFrameName(this->get_sprite_path());
    this->sprite->setScale(4);
    this->sprite->getTexture()->setAliasTexParameters();

    auto build_increment_callback = [this](int increase_by, float wait_by) {
        LongPressGestureRecognizer* gesture_recognizer = LongPressGestureRecognizer::create(wait_by);
        gesture_recognizer->onLongPress = [this, increase_by](LongPressGestureRecognizer *recognizer)
        {
            auto stato = recognizer->getStatus();
            auto location = recognizer->getGestureLocation();
            if (stato == GestureStatus::RECOGNIZED)
            {
                if (nodeContainsThePoint(this->sprite, location))
                {
                    COUNT += increase_by;
                    CCLOG("COUNT: %i", COUNT);
                }
            }
        };
        this->addChild(gesture_recognizer);
    };

    build_increment_callback(1, 0.5);

    this->setContentSize(this->get_sprite_size());

    this->sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->sprite->setPosition(get_relative_middle(this->getContentSize()));

    this->clip->addChild(this->sprite);
};

std::string Harvestable::get_sprite_path()
{
    return "weapon_ice.png";
};

void Harvestable::init_clicks()
{
    this->click_limit = 150;
    this->reset_current_clicks();
}

bool Harvestable::init()
{
    this->setTouchEnabled(true); //true otherwise it cant be clicked

    //allow touches to propagate passed it
    this->setSwallowTouches(false); //this seems to mean to let the events propagate past this, i dont see the harm in allowing it

    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    this->building = BUILDUP->get_target_building();

    this->clip = cocos2d::ClippingNode::create();
    this->stencil = cocos2d::DrawNode::create();

    this->clip->setStencil(this->stencil);
    this->clip->setInverted(true);
    this->addChild(this->clip);

    //NOTE: this needs to add the sprite to this->clip
    this->init_sprite();

    this->init_clicks();

    Node* harvest_scene_editor = get_prebuilt_node_from_csb("editor/scenes/base_scene.csb");
    Node* harvestable_pos = harvest_scene_editor->getChildByName("harvestable_pos");
    this->setPosition(harvestable_pos->getPosition());
    this->setName("harvestable");

    this->initial_scale = 0.9f;
    this->setScale(this->initial_scale);

    this->_current_output_ing_type = this->building->punched_sub_type;

    this->is_shattering = false;

    return true;
};

void Harvestable::try_to_shatter()
{
    if (this->should_shatter()) {
        this->shatter();
    };
};

bool Harvestable::should_shatter()
{
    return this->get_current_clicks() >= this->click_limit;
}

void Harvestable::on_harvest()
{
    //this is on every click
    this->building->create_ingredients(this->get_output_ing_type(), this->get_per_touch_output());

    //this should be on shatter eventually, to line up with the floating label
    GameLogic::getInstance()->add_total_harvests(1);
}

res_count_t Harvestable::get_per_touch_output()
{
    res_count_t base = 1.0L;

    auto tech_map = CITY->building_by_type(BuildingTypes::TheMarketplace)->techtree->get_tech_map();
    TechSubType tech_type = Technology::SubType::ClickDoublePower;

    res_count_t _def = 0L;
    res_count_t times_doubled = map_get(tech_map, tech_type, _def);
    if (times_doubled > 0){
        base *= std::pow(2.0L, times_doubled);
    };

    return base;
}
Ingredient::SubType Harvestable::get_output_ing_type() const
{
    return this->_current_output_ing_type;
};

void Harvestable::set_output_ing_type(Ingredient::SubType ing_type)
{
    this->_current_output_ing_type = ing_type;
};

void Harvestable::spawn_label_on_touch(cocos2d::Touch* touch, float end_scale, float duration, std::string floating_msg, Color4B text_color=Color4B::WHITE)
{
    /*
     * if touch is null (in ::shatter() for instance), it uses center of sprite
     */

    //FIXME this is scaling the harvestable, this should be somewhere else
    auto scale_to = ScaleTo::create(duration, end_scale);
    auto ease = EaseElasticOut::create(scale_to, duration);
    this->runAction(ease);

    Vec2 pos;
    auto floating_label = do_float();
    floating_label->setString(floating_msg);
    floating_label->setColor(Color3B(text_color));
    if (touch)
    {
        pos = touch->getLocation();
    }
    else
    {
        pos = this->getPosition();
    }

    pos.x += cocos2d::rand_minus1_1()*30.0f;
    pos.y += cocos2d::rand_0_1()*50.0f;

#ifdef __ANDROID__
    pos.y += 75.0f; //dont get hidden by finger
#endif

    floating_label->setPosition(pos);
    floating_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->getParent()->addChild(floating_label);
}

std::string Harvestable::get_create_output_message()
{
    res_count_t output = this->get_per_touch_output();

    std::stringstream ss;
    ss << "+" << beautify_double(output) << " " << Ingredient::type_to_string(this->get_output_ing_type());
    return ss.str();
}

//default text floating color
Color4B Harvestable::get_create_output_color()
{
    return Color4B::WHITE;
};

void Harvestable::animate_touch_start(cocos2d::Touch* touch)
{
    float end_scale = this->initial_scale*0.85f;
    float duration = 0.5f;

    std::string floating_msg;
    Color4B floating_color;

    Ingredient::SubType ing_type = this->get_output_ing_type();
    if (this->building->can_fit_more_ingredients(ing_type) == false)
    {
        do_vibrate(5);

        std::vector<std::string> choices;
        if (Tutorial::getInstance()->is_tutorial_active()) {
            choices = {
                "Sell your grain"
            };
        } else {
            choices = {
                "Upgrade building",
                "Make space: sell resources"
            };
        };
        floating_msg = pick_one(choices);

        end_scale = this->initial_scale*0.95f;
        duration = 0.15f;

        Node* raw_harvest_scene = Director::getInstance()->getRunningScene()->getChildByName("HarvestScene");
        HarvestScene* harvest_scene = dynamic_cast<HarvestScene*>(raw_harvest_scene);
        Node* building_info_panel = harvest_scene->getChildByName("building_info_panel");
        Node* raw_ingredient_count = building_info_panel->getChildByName("ingredient_count");

        ui::TextBMFont* ingredient_count = dynamic_cast<ui::TextBMFont*>(raw_ingredient_count);
        if (ingredient_count == NULL)
        {
            CCLOG("ERROR ingredient count is NULL");
        } else
        {
            run_flash_action(raw_ingredient_count, 0.2f, 1.15f, 1.0f, Color3B::RED, Color3B::WHITE);
        }
        building_info_panel->runAction(FShake::actionWithDuration(0.1f, 1.5f, 1.5f));

        Node* ing_panel = harvest_scene->get_visible_ing_panel(ing_type);
        if (ing_panel == NULL)
        {
            harvest_scene->toggle_ingredient_listviews(false);
            ing_panel = harvest_scene->get_visible_ing_panel(ing_type);

            //if we can't find it anyway, reset to default visibility
            if (ing_panel == NULL)
            {
                harvest_scene->toggle_ingredient_listviews(false);
            }
        }

        if (ing_panel)
        {
            run_flash_action(ing_panel, 0.1f, 1.15f, 1.0f, Color3B::RED, Color3B::WHITE);
        }

        floating_color = Color4B::RED;

    }
    else
    {
        floating_msg = this->get_create_output_message();
        floating_color = this->get_create_output_color();
    }

    this->spawn_label_on_touch(touch, end_scale, duration, floating_msg, floating_color);
}

bool Harvestable::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    bool base_val = Widget::onTouchBegan(touch, event);

    if (this->_hitted)
    {
        SoundLibrary::getInstance()->play_general_harvest_touched();

        auto achievement_manager = AchievementManager::getInstance();
        std::shared_ptr<BaseAchievement> achievement = achievement_manager->getAchievement(AchievementType::TotalTaps);
        achievement->increment();

        this->animate_touch_start(touch);
    }

    return base_val;
};

void Harvestable::animate_touch_end(cocos2d::Touch* touch)
{
    float end_scale = this->initial_scale;
    float duration = 0.5f;
    auto scale_to = ScaleTo::create(duration, end_scale);
    auto ease = EaseElasticOut::create(scale_to, duration);
    this->runAction(ease);
}

bool Harvestable::is_harvestable()
{
    return this->building->can_fit_more_ingredients(this->get_output_ing_type()) == true || this->building->name == "The Arena";
}

void Harvestable::handle_harvest()
{
    this->add_current_clicks(1);

    this->animate_harvest();
    this->on_harvest();

    if (this->should_shatter()) {
        this->shatter();
    };
}

void Harvestable::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    //Widget::onTouchEnded(touch, event); //this shouldnt be called because releaseUpEvent hasnt been set I guess.


    animate_touch_end(touch);

    //FIXME remove hardcoded check for arena, otherwise the sword wont rotate
    if (this->is_harvestable() == false)
    {
        return;
    };

    this->handle_harvest();

};

void Harvestable::animate_clip()
{
    return; //do nothing
}

float Harvestable::get_click_ratio() const
{
    return static_cast<float>(this->get_current_clicks()) / this->click_limit;
}

void Harvestable::animate_harvest()
{
    this->animate_clip();

    auto shake_action = FShake::actionWithDuration(0.075f, 2.5f);
    this->runAction(shake_action);

    this->animate_rotate();
}

void Harvestable::animate_rotate()
{

    float click_ratio = this->get_click_ratio();
    float rotation = 0.0f;

    if (click_ratio >= 0.8f) {
        rotation = 15.0f;
    } else if (click_ratio >= 0.6f) {
        rotation = 6.0f;
    } else if (click_ratio >= 0.4f) {
        rotation = 2.0f;
    };

    if (rotation != 0.0f) {
        auto rotate_by = RotateBy::create(0.05f, rotation);
        auto rotate_to = RotateTo::create(0.05f, 0);
        this->sprite->runAction(Sequence::createWithTwoActions(rotate_by, rotate_to));
        this->clip->runAction(Sequence::createWithTwoActions(rotate_by, rotate_to));
    };
}

std::string Harvestable::get_shatter_text()
{
    //nothing anymore, only crafting
    return "";
}

cocos2d::Color4B Harvestable::get_shatter_text_color()
{
    return Color4B::WHITE;
};

res_count_t Harvestable::get_current_clicks() const
{
    return this->_current_clicks;
};

void Harvestable::reset_current_clicks()
{
    this->set_current_clicks(0);
};
void Harvestable::set_current_clicks(res_count_t count)
{
    this->_current_clicks = count;
};

void Harvestable::add_current_clicks(res_count_t new_clicks)
{
    this->_current_clicks += new_clicks;
};

void Harvestable::shatter()
{
    if (this->is_shattering) { return; };
    this->is_shattering = true;

    try_set_visible(this->sprite, false);
    this->setTouchEnabled(false);

    float sprite_scale = this->sprite->getScale();
    Size sprite_size = this->get_sprite_size();

    auto shatter_sprite = ShatterSprite::createWithSpriteFrame(this->sprite->getSpriteFrame());
    shatter_sprite->setScale(sprite_scale);
    shatter_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    shatter_sprite->setPosition(get_relative_middle(this->getContentSize()));
    this->addChild(shatter_sprite);

    shatter_sprite->setOpacity(0); //hide this so it shatters it doesnt leave anything behind

    //spawn label
    this->spawn_label_on_touch(NULL, 1, 1, this->get_shatter_text(), this->get_shatter_text_color());

    CallFunc* remove = CallFunc::create([this](){
            this->removeFromParent();
            });

    auto shatter_action = ShatterAction::create(0.5f);
    //shatter_sprite->runAction(shatter_action);
    shatter_sprite->runAction(Sequence::createWithTwoActions(shatter_action, remove));

};

std::string FarmingHarvestable::get_sprite_path()
{
    return "farm.png";
};

std::string GraveyardHarvestable::get_sprite_path()
{
    return "gravestone.png";
};

std::string TreeHarvestable::get_sprite_path()
{
    return "forest.png";
}

void TreeHarvestable::animate_touch_start(cocos2d::Touch* touch)
{
    RandomWeightMap<Ingredient::SubType> item_rarity_map;
    item_rarity_map.add_item(Ingredient::SubType::Berry, 95);

    int wood_weight = 5; //standard default
    auto tech_map = this->building->techtree->get_tech_map();
    TechSubType tech_type = Technology::SubType::RaiseWoodFind;

    res_count_t _def = 1L;
    res_count_t times_doubled = map_get(tech_map, tech_type, _def);
    wood_weight *= times_doubled+1;


    item_rarity_map.add_item(Ingredient::SubType::Wood, wood_weight);

    Ingredient::SubType output = item_rarity_map.get_item();

    //vibrate if player gets wood
    if (output == Ingredient::SubType::Wood){
        do_vibrate(10);
    };

    this->set_output_ing_type(output);

    Harvestable::animate_touch_start(touch); //super()
};

Color4B TreeHarvestable::get_create_output_color()
{
    if (this->get_output_ing_type() == Ingredient::SubType::Berry)
    {
        return Color4B::WHITE;
    }
    else
    {
        return Color4B::GREEN;
    };
};

void TreeHarvestable::shatter()
{
    Harvestable::shatter();

    res_count_t output_num = this->get_per_touch_output()*20;
    this->building->create_ingredients(Ingredient::SubType::Wood, output_num);

    std::stringstream ss;
    ss << "+" << output_num << " bonus wood!";
    this->spawn_label_on_touch(NULL, this->initial_scale, 1.0f, ss.str(), Color4B::GREEN);

};

void DumpsterHarvestable::init_clicks()
{

    auto harvestable_manager = HarvestableManager::getInstance();

    if (harvestable_manager->is_item_stored == false)
    {
        this->generate_item();
        harvestable_manager->store_item(this->scavenge_item);

        this->reset_current_clicks();
    }
    else
    {
        harvestable_manager->load_item(this->scavenge_item); //NOTE this replaces scavenge_item with the stored item
        this->set_current_clicks(harvestable_manager->stored_dumpster_clicks);
    };

    //item level * 15 clicks
    this->click_limit = this->scavenge_item->level*15.0;
}

std::string DumpsterHarvestable::get_sprite_path()
{
    return "dump.png";
}

void DumpsterHarvestable::on_harvest()
{
    Harvestable::on_harvest();

    auto harvestable_manager = HarvestableManager::getInstance();
    harvestable_manager->stored_dumpster_clicks = this->get_current_clicks();
};

void DumpsterHarvestable::animate_clip()
{
    //deliberately do nothing
}

void DumpsterHarvestable::shatter()
{
    if (this->is_shattering) { return; };
    Harvestable::shatter();

    auto harvestable_manager = HarvestableManager::getInstance();
    harvestable_manager->reset_item();
    harvestable_manager->stored_dumpster_clicks = 0;

    auto achievement_manager = AchievementManager::getInstance();
    std::shared_ptr<BaseAchievement> achievement = achievement_manager->getAchievement(AchievementType::TotalItems);
    achievement->increment();

    EQUIPMENT->inventory.push_back(scavenge_item);
    auto popup_panel = GameLogic::get_popup_panel();
    if (popup_panel != NULL)
    {
        popup_panel->set_string("Scavenged: " + scavenge_item->get_name() + "!");
        popup_panel->animate_open();
        popup_panel->set_image(scavenge_item->img_path);
    }
};

void DumpsterHarvestable::generate_item()
{
    ItemData item_data = ItemData();

    RandomWeightMap<std::string> item_type_map;
    for (auto item : item_data.get_all_items())
    {
        //pull weight off item, from the item_data.json
        item_type_map.add_item(item->type_name, item->scavenge_weight);
    }

    RandomWeightMap<ItemRarityType> item_rarity_map;
    item_rarity_map.add_item(ItemRarityType::Poor, 40);
    item_rarity_map.add_item(ItemRarityType::Normal, 50);
    item_rarity_map.add_item(ItemRarityType::Rare, 10);

    spItem item = item_data.get_item(item_type_map.get_item());
    item->rarity = item_rarity_map.get_item();

    this->scavenge_item = item;

    auto game_logic = GameLogic::getInstance();
    //max item level goes up every 10 souls used to appease, plus 1
    res_count_t max_level = std::floor(((res_count_t)game_logic->get_appeasements()) / 10)+1;

    //pick a level between 1 and max_level
    std::mt19937 gen = std::mt19937(std::random_device{}());
    std::uniform_real_distribution<res_count_t> distribution(1, max_level);
    res_count_t item_level = std::floor(distribution(gen));
    this->scavenge_item->level = item_level;
};

std::string MiningHarvestable::get_sprite_path()
{
    return "mine.png";
};

void MiningHarvestable::animate_touch_start(cocos2d::Touch* touch)
{
    RandomWeightMap<Ingredient::SubType> item_rarity_map;
    item_rarity_map.add_item(Ingredient::SubType::Copper, 95);

    int iron_weight = 5; //standard default

    if (EQUIPMENT->mining_slot->has_item())
    {
        spItem item = EQUIPMENT->mining_slot->get_item();
        iron_weight += (0.05 * item->level)* ITEM_RARITY_MODIFIER.at(item->rarity);
    };

    item_rarity_map.add_item(Ingredient::SubType::Iron, iron_weight);

    Ingredient::SubType output = item_rarity_map.get_item();

    //vibrate if player gets wood
    if (output == Ingredient::SubType::Iron){
        do_vibrate(10);
    };

    this->set_output_ing_type(output);

    Harvestable::animate_touch_start(touch); //super()
};


void MiningHarvestable::animate_clip()
{
    float click_ratio = this->get_click_ratio();

    auto sprite_size = this->get_sprite_size();
    auto size = 20.0f;
    Vec2 origin = Vec2(
            MIN(sprite_size.width, sprite_size.width - size)*CCRANDOM_0_1(), //random along the width, dont go so far right
            sprite_size.height - (click_ratio*sprite_size.height) //from the top, go a percentage of the way down
            );

    Vec2 destination = origin + Vec2(size, size);
    this->stencil->drawSolidRect(origin, destination, Color4F::MAGENTA);
}

//default text floating color
Color4B MiningHarvestable::get_create_output_color()
{
    if (this->get_output_ing_type() == Ingredient::SubType::Copper)
    {
        return Color4B::WHITE;
    }
    else
    {
        return Color4B::GREEN;
    };
};


void MiningHarvestable::animate_rotate()
{
    float intensity = 0.0f;
    float click_ratio = this->get_click_ratio();

    if (click_ratio >= 0.8f) {
        intensity = 15.0f;
    } else if (click_ratio >= 0.6f) {
        intensity = 6.0f;
    } else if (click_ratio >= 0.4f) {
        intensity = 2.0f;
    };

    if (intensity != 0.0f) {
        this->runAction(FShake::actionWithDuration(0.075f, intensity));
    };
}
CraftingHarvestable::CraftingHarvestable(spRecipe recipe)
    : recipe(recipe)
{
}

CraftingHarvestable* CraftingHarvestable::create(spRecipe recipe)
{
    CraftingHarvestable *pRet = new(std::nothrow) CraftingHarvestable(recipe);
    if (pRet && pRet->init()) {
        pRet->autorelease(); return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

std::string CraftingHarvestable::get_sprite_path()
{
    return "workshop.png";
};

float CraftingHarvestable::get_click_ratio() const
{
    if (this->recipe == NULL)
    {
        return Harvestable::get_click_ratio();
    }
    else
    {
        //TODO fix hardcoded 3
        return static_cast<float>(this->recipe->current_clicks) / 3;
    };
}

bool CraftingHarvestable::can_satisfy_recipe_per_click()
{
    if (this->recipe == NULL) { return false; }

    auto& all_ingredients = BUILDUP->get_all_ingredients();

    //go through each component, for each ing, make sure we've got at least one
    // otherwise short circuit and fail fast
    for (auto component : this->recipe->components)
    {
        IngredientSubType ing_type = component.first;

        //either the amount required minus click so far, or 0 so its not negative
        res_count_t amount_required = std::max(0.0L, component.second - this->recipe->current_clicks);

        //if there's at least one required, check for at least one
        res_count_t _def = 0.0;
        if (amount_required >= 1.0 && map_get(all_ingredients, ing_type, _def) < 1.0)
        {
            return false;
        }

    };

    return true;
};

void CraftingHarvestable::on_harvest()
{
    if (!this->recipe) { return; }

    auto& all_ingredients = BUILDUP->get_all_ingredients();

    bool can_satisfy = this->can_satisfy_recipe_per_click();

    //if all parts are satisfied, then actually use them and increment counter
    if (can_satisfy == true)
    {
        for (auto component : this->recipe->components)
        {
            IngredientSubType ing_type = component.first;
            res_count_t amount_required = std::max(0.0L, component.second - this->recipe->current_clicks);

            if (amount_required > 0)
            {
                BUILDUP->remove_shared_ingredients_from_all(ing_type, 1);
                CCLOG("spending a %s", Ingredient::type_to_string(ing_type).c_str());
            }
            else
            {
                CCLOG("dont need to spend %s", Ingredient::type_to_string(ing_type).c_str());
            }
        };
        this->recipe->current_clicks += 1;

    }
};

void CraftingHarvestable::animate_touch_start(cocos2d::Touch* touch)
{
    float end_scale = this->initial_scale*0.85f;
    float duration = 0.5f;

    std::stringstream ss;
    if (this->recipe == NULL)
    {
        ss << "Choose a recipe!";
        std::string floating_msg = ss.str();
        do_vibrate(5);
        this->spawn_label_on_touch(touch, end_scale, duration, floating_msg, Color4B::RED);

        //move to Craft tab and shake it
        HarvestScene* harvest_scene = dynamic_cast<HarvestScene*>(this->getParent());
        if (harvest_scene){
            if (harvest_scene->sidebar->tabs.active_tab != TabTypes::DetailTab){
                harvest_scene->sidebar->toggle_buttons(harvest_scene->sidebar->tab_detail_btn, TouchEventType::ENDED);
            };

            auto detail_listview = harvest_scene->sidebar->tabs.get_active_listview();
            for (auto& child : detail_listview->getChildren()){
                run_flash_action(child, 0.1f, 1.05f, 1.0f, Color3B::RED, Color3B::WHITE);
            };
        };
    }
    else
    {
        if (this->can_satisfy_recipe_per_click() && !this->should_shatter())
        {
            //TODO doesn't make sure it needs at least one of any given ing type
            ss << "Used ";
            for (auto component : this->recipe->components)
            {
                ss << "a " << Ingredient::type_to_string(component.first) << " ";
            }
            std::string floating_msg = ss.str();
            this->spawn_label_on_touch(touch, end_scale, duration, floating_msg);
        }
        else
        {
            //TODO make sure you're actually missing each ing type
            for (auto component : this->recipe->components)
            {
                ss << "Need " << Ingredient::type_to_string(component.first) << ". ";
            }

            std::string floating_msg = ss.str();
            do_vibrate(5);
            this->spawn_label_on_touch(touch, end_scale, duration, floating_msg, Color4B::RED);
        }
    }


    //TODO handle message for being too full on recipe, like in
    //Harvestable::animate_touch_start

};

void CraftingHarvestable::init_clicks()
{
    if (!this->recipe) {
        CCLOG("trying to init_clicks without a recipe");
        Harvestable::init_clicks();
        return;
    };

    this->reset_current_clicks();
    this->click_limit = this->recipe->clicks_required;
};

//same as Harvestable::onTouchEnded except it doesn't check for room
void CraftingHarvestable::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

    animate_touch_end(touch);

    this->add_current_clicks(1);

    this->animate_harvest();
    this->on_harvest();

    //recipes completed
    if (this->should_shatter()) {
        this->shatter();
    };

};

void CraftingHarvestable::animate_clip()
{
    if (this->can_satisfy_recipe_per_click())
    {
        auto sprite_size = this->get_sprite_size();
        auto size = 20.0f;
        Vec2 origin = Vec2(
                MIN(sprite_size.width, sprite_size.width - size)*CCRANDOM_0_1(), //random along the width, dont go so far right
                sprite_size.height
                );

        float click_ratio = this->get_click_ratio();
        auto spark_parts = ParticleSystemQuad::create("particles/anvil_spark.plist");
        spark_parts->setScale(1.0 / 4.0f);
        spark_parts->setPosition(origin);
        int total_particles = spark_parts->getTotalParticles();
        int active_particles = (int)(total_particles * click_ratio);
        //add 10 so there's always visible sparks, cheaper than mix/max calls
        spark_parts->setTotalParticles(active_particles+10);
        this->addChild(spark_parts);
    };
}

bool CraftingHarvestable::should_shatter()
{
    bool has_recipe = this->recipe != NULL;
    //dont ever shatter if no recipe
    if (has_recipe == false)
    {
        return false;
    }

    //use clicks instead of ingredient count, because it needs partial progress
    res_count_t clicks_required = 3.0; //TODO use biggest number of component in recipe
    bool is_satisfied = this->recipe->current_clicks >= clicks_required;

    return is_satisfied;
};

std::string CraftingHarvestable::get_shatter_text()
{
    return "Crafted!";
}

cocos2d::Color4B CraftingHarvestable::get_shatter_text_color()
{
    return Color4B::GREEN;
};

void CraftingHarvestable::shatter()
{
    if (this->recipe != NULL) {
        enum class Output {
            Single = 1,
            Double = 2,
            Triple = 3
        };

        RandomWeightMap<Output> boosted_output;
        boosted_output.add_item(Output::Single, 100);

        if (EQUIPMENT->recipe_slot->has_item())
        {
            spItem item = EQUIPMENT->recipe_slot->get_item();

            boosted_output.add_item(Output::Double, 15 + (item->level+1)*0.05);
            boosted_output.add_item(Output::Triple, 1 + (item->level+1)*0.025);
        }
        else
        {
            boosted_output.add_item(Output::Double, 15);
            boosted_output.add_item(Output::Triple, 1);
        };

        Output result = boosted_output.get_item();

        auto achievement_manager = AchievementManager::getInstance();
        std::shared_ptr<BaseAchievement> achievement = achievement_manager->getAchievement(AchievementType::TotalRecipesCrafted);

        //FIXME reduce hardcoding maybe?
        if (result == Output::Single) {
            this->recipe->on_recipe_complete();
            achievement->increment();
            do_vibrate(10); //vibrate normally

        } else if (result == Output::Double) {
            this->recipe->on_recipe_complete();
            this->recipe->on_recipe_complete();
            achievement->increment();
            achievement->increment();
            do_vibrate(20); //vibrate stronger

        } else if (result == Output::Triple) {
            this->recipe->on_recipe_complete();
            this->recipe->on_recipe_complete();
            this->recipe->on_recipe_complete();
            achievement->increment();
            achievement->increment();
            achievement->increment();
            do_vibrate(30); //vibrate even stronger
        }
        this->recipe->current_clicks = 0;
    };

    Harvestable::shatter();
}

bool FightingHarvestable::init()
{
    Harvestable::init();

    this->initial_scale = 4;
    this->setScale(this->initial_scale);

    this->click_limit = 1000000; //some really high number they'll never click

    //setup enemy node
    this->enemy = std::make_shared<Fighter>("temp");
    this->enemy->combat = std::make_shared<Combat>("enemy combat", this->enemy);

    FighterNode* fighter_node = FighterNode::create(this->enemy);
    fighter_node->setScale(0.25f);
    fighter_node->setPosition(Vec2(50, 0));
    //try_set_visible(fighter_node->xp_bar->front_timer, false);
    fighter_node->xp_bar->setVisible(false);
    this->addChild(fighter_node);

    this->spawn_enemy();

    fighter_node->img_view->addTouchEventListener([this](Ref* target, ui::Widget::TouchEventType type)
            {
            if (type == TouchEventType::BEGAN)
            {
            //// this nearly worked, the coords werent right, so this->_hitted
            ////was always false, otherwise worked to sim a click
            // EventTouch evt = cocos2d::EventTouch();
            // evt.setEventCode(EventTouch::EventCode::BEGAN);
            // Touch touch = Touch();
            // Vec2 vec = { 1, 1 };
            // vec = this->sprite->convertToWorldSpace(vec);

            // touch.setTouchInfo(-1, vec.x, vec.y);
            // std::vector<Touch*> touches ={  &touch  };
            // evt.setTouches(touches);
            // this->sprite->getEventDispatcher()->dispatchEvent(&evt);


            this->handle_harvest();

            }
            });

    //adjust position because the enemy and sword are both offset from the center
    this->setPosition(Vec2(this->getPosition().x-110, this->getPosition().y));


    this->is_critical_hit = false;

    return true;
};

std::string FightingHarvestable::get_sprite_path()
{
    return "sword.png";
}

void FightingHarvestable::animate_clip()
{
    auto player = BUILDUP->fighter;
    double damage = player->attrs->damage->current_val;
    res_count_t output = damage;

    std::stringstream ss;
    ss << beautify_double(output);

    std::string floating_msg = ss.str();
    auto floating_label = do_float();
    floating_label->setString(floating_msg);

    if (this->is_critical_hit)
    {
        floating_label->setColor(Color3B::ORANGE);
    }

    auto fighter_node = dynamic_cast<FighterNode*>(this->getChildByName("fighter_node"));
    auto pos = fighter_node->convertToWorldSpace(this->getTouchEndPosition());
    pos.x += cocos2d::rand_minus1_1()*30.0f;
    pos.y += cocos2d::rand_0_1()*50.0f;

#ifdef __ANDROID__
    pos.y += 75.0f; //dont get hidden by finger
#endif

    floating_label->setPosition(pos);
    floating_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->getParent()->addChild(floating_label);

};

void FightingHarvestable::animate_harvest()
{
    if (BUILDUP->fighter->attrs->health->is_empty() == false)
    {
        Harvestable::animate_harvest();
    }
    else
    {
        //shake hp panel if dead
        auto harvest_scene = this->getParent();
        auto player_info_panel = harvest_scene->getChildByName("player_info_panel");
        auto player_hp_gold = player_info_panel->getChildByName("player_hp_lbl");
        float duration = 0.075f;
        run_flash_action(player_hp_gold, duration, 1.5f, 1.0f, Color3B::RED, Color3B::WHITE);
        player_info_panel->runAction(FShake::actionWithDuration(duration, 10));

        this->spawn_label_on_touch(NULL, this->initial_scale, 0, "Cast Bloodoath\nto regenerate!\n(The Underscape)", Color4B::RED);
    }
}

void FightingHarvestable::animate_rotate()
{
    float rotation = 30.0f;
    auto rotate_by = RotateBy::create(0.05f, rotation);
    auto rotate_to = RotateTo::create(0.05f, 0);
    this->sprite->runAction(Sequence::createWithTwoActions(rotate_by, rotate_to));
    this->clip->runAction(Sequence::createWithTwoActions(rotate_by, rotate_to));
};

bool FightingHarvestable::should_shatter()
{
    if (this->enemy == NULL) { return false; }
    return this->enemy->attrs->health->is_empty();
}

void FightingHarvestable::spawn_enemy()
{
    this->is_shattering = false;
    GameLogic* game_logic = GameLogic::getInstance();

    this->enemy = NULL;
    this->enemy = std::make_shared<Fighter>("Challenger");
    this->enemy->combat = std::make_shared<Combat>("challengers's combat", this->enemy);
    this->enemy->team = Fighter::TeamTwo;
    this->enemy->sprite_name = "harvester.png";

    res_count_t base_hp = 20.0;
    res_count_t base_slow_rate = 3.0;

    spBuilding workshop = BUILDUP->city->building_by_type(BuildingTypes::TheWorkshop);
    TechMap tech_map = workshop->techtree->get_tech_map();

    auto tech_type = Technology::SubType::CombatWeakenEnemy;
    res_count_t _def = 0L;
    res_count_t decrease_count = map_get(tech_map, tech_type, _def);
    res_count_t final_slow_rate = std::max(0.01L, base_slow_rate + (0.005*decrease_count));

    res_count_t enemy_hp = scale_number_slow(base_hp, (res_count_t)game_logic->get_total_kills(), 1.05L, final_slow_rate);

    res_count_t max_possible_hp = 9999999999999999999.0;
    enemy_hp = std::min(std::max(base_hp, enemy_hp), max_possible_hp); //make sure the enemy has at least 20 hp, but never more than max_possible_hp
    enemy_hp = std::floor(enemy_hp);

    res_count_t enemy_dmg = 3;

    auto harvestable_manager = HarvestableManager::getInstance();
    if (harvestable_manager->is_fighter_stored == false)
    {
        this->enemy->attrs->health->set_vals(enemy_hp);
        this->enemy->attrs->damage->set_vals(enemy_dmg);
        harvestable_manager->store_fighter(this->enemy);
    }
    else
    {
        harvestable_manager->load_fighter(this->enemy);
    };

    FighterNode* fighter_node = dynamic_cast<FighterNode*>(this->getChildByName("fighter_node"));
    fighter_node->set_fighter(this->enemy);

    auto gen_paths = [](std::string base_path, int max_num)
    {
        std::vector<int> nums(max_num);
        std::iota(nums.begin(), nums.end(), 0);

        std::vector<std::string> output;
        for (auto num : nums)
        {
            output.push_back(base_path + "_" + std::to_string(num)+".png");
        }

        return  output;
    };

    Node* base_node = Node::create();

    std::vector<std::string> sprites = {
        pick_one(gen_paths("set", 4)),
        pick_one(gen_paths("body", 49)),
        pick_one(gen_paths("headwear", 49)),
        pick_one(gen_paths("legs", 22)),
        pick_one(gen_paths("shield", 49)),
        pick_one(gen_paths("weapon", 49))
    };

    for (std::string& path : sprites)
    {
        base_node->addChild(Sprite::createWithSpriteFrameName(path));
    }

    base_node->setPosition(8,8);
    base_node->setScaleY(-1.0f);

    RenderTexture* rt = RenderTexture::create(16, 16);
    rt->begin();
    base_node->visit();
    rt->end();

    //use the texture from RenderTexture and replace the ImageView's Scale9 texture
    cocos2d::ui::ImageView* fighter_node_img_view = fighter_node->img_view;
    ui::Scale9Sprite* scale9_sprite = dynamic_cast<ui::Scale9Sprite*>(fighter_node_img_view->getVirtualRenderer());

    Sprite* tex_sprite = dynamic_cast<Sprite*>(rt->getSprite());
    scale9_sprite->setSpriteFrame(tex_sprite->getSpriteFrame());
    scale9_sprite->setScale9Enabled(false); //NOTE!! scale9 must be disabled on the sprite, otherwise there's a memory crash

};

const std::map<res_count_t, std::string> kill_messages = {
    { 1.0, "The first kill!" },
    { 10.0, "The tenth kill! This is a strange feeling." },
    { 30.0, "The thirtieth kill! This isn't right." },
    { 50.0, "The fiftieth kill! It doesn't matter." },
    { 75.0, "The 75th kill! I'll ignore the feeling." },
    { 100.0, "The 100th kill! But I can't, it won't go away." },
    { 150.0, "The 150th kill! It gnaws on me..." },
    { 250.0, "The 250th kill! I need more." },
    { 350.0, "The 350th kill! I need more strength." },
    { 500.0, "The 500th kill! I need more power." },
    { 750.0, "The 750th kill! I need more souls." },
    { 1000.0, "The 1000th kill! I've done it. I've killed one thousand souls..." }
};

void FightingHarvestable::shatter()
{
    if (this->is_shattering) { return; };

    this->is_shattering = true;

    auto game_logic = GameLogic::getInstance();
    game_logic->add_total_kills(1);

    //auto it = std::find(kill_messages.begin(), kill_messages.end(), game_logic->get_total_kills());
    auto it = kill_messages.find(game_logic->get_total_kills());
    if (it != kill_messages.end())
    {
        auto popup_panel = GameLogic::get_popup_panel();
        if (popup_panel != NULL)
        {
            std::string kill_message = (*it).second;
            popup_panel->set_string(kill_message);
            popup_panel->animate_open();
        }
    }

    //spawn shatter
    FighterNode* fighter_node = dynamic_cast<FighterNode*>(this->getChildByName("fighter_node"));
    Node* renderer = fighter_node->img_view->getVirtualRenderer();
    Sprite* sprite = dynamic_cast<ui::Scale9Sprite*>(renderer)->getSprite();
    auto shatter_sprite = ShatterSprite::createWithTexture(sprite->getTexture());
    shatter_sprite->setScale(4.0f);
    shatter_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    shatter_sprite->setPosition(get_center_pos());
    this->getParent()->addChild(shatter_sprite);

    auto shatter_action = ShatterAction::create(0.5f);
    shatter_sprite->setOpacity(0);
    shatter_sprite->runAction(Sequence::createWithTwoActions(shatter_action, cocos2d::RemoveSelf::create()));

    auto harvestable_manager = HarvestableManager::getInstance();
    harvestable_manager->reset_fighter();
    this->spawn_enemy();

    this->building->create_ingredients(Ingredient::SubType::Soul, 1);
}

void FightingHarvestable::on_harvest()
{
    //TODO name fighter to player_avatar or something
    auto player = BUILDUP->fighter;
    res_count_t damage = this->get_per_touch_output();

    std::mt19937 gen = std::mt19937(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 100);
    int crit_result = distribution(gen);

    int crit_chance = this->get_combat_crit_chance();

    if (crit_result <= crit_chance)
    {
        this->is_critical_hit = true;
        res_count_t crit_factor = this->get_combat_crit_factor();
        damage *= crit_factor;
        do_vibrate(5);
    }
    else
    {
        this->is_critical_hit = false;
    }

    player->attrs->damage->set_vals(damage);

    auto battle = std::make_shared<Battle>();
    battle->combatants.push_back(player);

    battle->combatants.push_back(this->enemy);

    battle->do_battle();

    auto harvestable_manager = HarvestableManager::getInstance();
    harvestable_manager->store_fighter(this->enemy);
};

void FightingHarvestable::animate_touch_start(cocos2d::Touch* touch)
{
    //do nothing
};

void FightingHarvestable::animate_touch_end(cocos2d::Touch* touch)
{
    //do nothing
};

res_count_t FightingHarvestable::get_per_touch_output()
{
    res_count_t base = 5.0L;
    if (EQUIPMENT->combat_slot->has_item())
    {
        spItem item = EQUIPMENT->combat_slot->get_item();
        base = scale_number_flat(base, item->level, static_cast<res_count_t>(2) * ITEM_RARITY_MODIFIER.at(item->rarity));
    }

    auto tech_map = this->building->techtree->get_tech_map();

    auto tech_type = Technology::SubType::CombatDamage;
    res_count_t _def = 0L;
    res_count_t times_increased = map_get(tech_map, tech_type, _def);

    base = scale_number(base, times_increased, 1.2L);

    return std::floor(base + (base*0.15*CCRANDOM_MINUS1_1()));
}

int FightingHarvestable::get_combat_crit_chance()
{
    int base = 1;

    auto tech_map = this->building->techtree->get_tech_map();

    auto tech_type = Technology::SubType::CombatCritChance;
    res_count_t _def = 0L;
    res_count_t times_increased = map_get(tech_map, tech_type, _def);

    if (times_increased > 0){
        base += 1.0L * times_increased;
    };

    return std::floor(base);
}

res_count_t FightingHarvestable::get_combat_crit_factor()
{
    int base = 5;

    auto tech_map = this->building->techtree->get_tech_map();

    auto tech_type = Technology::SubType::CombatCritFactor;
    res_count_t _def = 0L;
    res_count_t times_increased = map_get(tech_map, tech_type, _def);

    if (times_increased > 0){
        base += 1.0L * times_increased;
    };

    return std::floor(base);
}

std::string UndeadHarvestable::get_sprite_path()
{
    return "pink_monster.png";
}

bool UndeadHarvestable::init()
{
    Harvestable::init();

    this->initial_scale = 1.0f;
    this->setScale(this->initial_scale);

    return true;
};

void UndeadHarvestable::animate_clip()
{
    auto sprite_size = this->get_sprite_size();
    auto size = 20.0f;
    Vec2 origin = Vec2(
        sprite_size.width*CCRANDOM_0_1(),
        sprite_size.height*CCRANDOM_0_1()
    );

    float click_ratio = this->get_click_ratio();
    auto spark_parts = ParticleSystemQuad::create("particles/blood.plist");
    spark_parts->setPosition(origin);
    int total_particles = spark_parts->getTotalParticles();
    int particle_count = (int)(total_particles * click_ratio);
    spark_parts->setTotalParticles(particle_count+10);
    this->addChild(spark_parts);

};

void UndeadHarvestable::animate_rotate()
{
    float intensity = 0.0f;
    float click_ratio = this->get_click_ratio();

    if (click_ratio >= 0.8f) {
        intensity = 15.0f;
    } else if (click_ratio >= 0.6f) {
        intensity = 6.0f;
    } else if (click_ratio >= 0.4f) {
        intensity = 4.0f;
    };

    if (intensity != 0.0f) {
        this->runAction(FShake::actionWithDuration(0.075f, intensity));
    };
}

std::string MarketHarvestable::get_sprite_path()
{
    return "marketplace.png";
};
