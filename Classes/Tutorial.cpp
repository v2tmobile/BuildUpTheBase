#include "Tutorial.h"

#include <sstream>

#include "ui/UIText.h"
#include "ui/UILoadingBar.h"
#include "ui/UILayout.h"
#include "2d/CCActionProgressTimer.h"
#include "2d/CCActionEase.h"
#include "2d/CCParticleExamples.h"

#include "constants.h"
#include "Beatup.h"
#include "GameLogic.h"
#include "HouseBuilding.h"
#include "Util.h"
#include "MiscUI.h"

Tutorial* Tutorial::_instance = nullptr;

Tutorial::Tutorial()
{
    //TODO serialize this
    this->_show_sidebar = true;
    this->_show_building_buttons = true;
    this->_show_player_info = true;
    this->_show_building_info = true;
    this->_show_progress_panel = true;

    this->current_step = NULL;
    this->next_step = NULL;

};

Tutorial* Tutorial::getInstance()
{
    if (!Tutorial::_instance)
    {
        Tutorial::_instance = new Tutorial();
    };

    return Tutorial::_instance;
}

void Tutorial::load_step(int step_index)
{
    std::function<bool(std::shared_ptr<TutorialStep>)> matches_step_index = [step_index](std::shared_ptr<TutorialStep> step)-> bool
    {
        return step->step_index == step_index;
    };
    auto step_it = std::find_if(this->steps.begin(), this->steps.end(), matches_step_index);

    if (step_it != this->steps.end())
    {
        (*step_it)->load_step();
    }
    else
    {
        CCLOG("couldn't find matching step for %i", step_index);
    }
};

void Tutorial::first_start(cocos2d::Node* parent)
{
    this->hide_game_ui();

    //default to The Farm, just in case
    BUILDUP->set_target_building(BUILDUP->city->building_by_name("The Farm"));

    auto tutorial_sidebar_panel = parent->getChildByName("tutorial_sidebar_panel")->getChildByName("tutorial_sidebar_panel"); //FIXME i wish there was a way to name these better to reduce repetition

    //handles prepping this steps ui
    auto first_step = std::make_shared<TutorialStep>(
        tutorial_sidebar_panel,
        "Welcome to\n Build Up The Base",
        "You're going to want to gather some resources.\nTap the farm and gather a few grains.",
        "  Grain Harvested!"
    );
    first_step->step_index = 0;
    this->steps.push_back(first_step);

    this->current_step = first_step;


    auto check_grain = [first_step](float dt){
        //update progress bar
        res_count_t target_total_grain = building_storage_limit.at(1);
        res_count_t grain_count = BUILDUP->count_ingredients(Ingredient::SubType::Grain);
        res_count_t satisfied_percentage = grain_count/target_total_grain*100;
        first_step->tutorial_loadingbar->setPercent((float)satisfied_percentage);

        //update progress label
        res_count_t remaining_grain = target_total_grain - grain_count;
        std::stringstream progress_ss;
        res_count_t adjusted_remaining = std::max(remaining_grain, 0.0L);
        progress_ss << beautify_double(adjusted_remaining) << " grain to harvest";
        first_step->tutorial_progress_lbl->setString(progress_ss.str());

        if (remaining_grain < 1) {
            //add fireworks, change text to complete, show next button etc
            first_step->celebrate();
        } else {
            first_step->reset();
        };
    };
    first_step->set_scheduled_func(check_grain);

    //setup next_step
    auto second_step = std::make_shared<TutorialStep>(
        tutorial_sidebar_panel,
        "Makin' some money",
        "Now you've gathered some resources, it's time to spend it. Gather and sell enough grain to make 300$.\n\nTap the grain icon along the bottom and choose a quantity to sell.",
        "   You're loaded."
    );
    second_step->step_index = 1;
    this->steps.push_back(second_step);

    auto check_money = [this, second_step](float dt){
        //update progress bar
        res_count_t target_coins = 300.0;
        res_count_t total_coins = BEATUP->get_total_coins();
        res_count_t satisfied_percentage = total_coins/target_coins*100;
        second_step->tutorial_loadingbar->setPercent((float)satisfied_percentage);

        //update progress label
        res_count_t remaining_coins = target_coins - total_coins;
        std::stringstream progress_ss;
        res_count_t adjusted_remaining = std::max(remaining_coins, 0.0L);
        progress_ss << beautify_double(adjusted_remaining) << " coins to earn";
        second_step->tutorial_progress_lbl->setString(progress_ss.str());

        if (remaining_coins < 1) {
            //add fireworks, change text to complete, show next button etc
            second_step->celebrate();
        } else {
            second_step->reset();
        };
    };
    second_step->set_scheduled_func(check_money);

    std::vector<std::pair<bool Tutorial::*, bool>> switch_map;
    switch_map.push_back(
        std::make_pair<bool Tutorial::*, bool>(&Tutorial::_show_player_info, true)
    );
    second_step->set_switch_map(switch_map);

    this->load_step(0);
};

bool Tutorial::get_show_sidebar()
{
    return this->_show_sidebar;
};

void Tutorial::set_show_sidebar(bool is_visible)
{
    this->_show_sidebar = is_visible;
};

bool Tutorial::get_show_building_buttons()
{
    return this->_show_building_buttons;
};

void Tutorial::set_show_building_buttons(bool is_visible)
{
    this->_show_building_buttons = is_visible;
};

bool Tutorial::get_show_player_info()
{
    return this->_show_player_info;
};

void Tutorial::set_show_player_info(bool is_visible)
{
    this->_show_player_info = is_visible;
};

bool Tutorial::get_show_player_hp_lbl()
{
    return this->_show_player_hp_lbl;
};

void Tutorial::set_show_player_hp_lbl(bool is_visible)
{
    this->_show_player_hp_lbl = is_visible;
};

bool Tutorial::get_show_progress_panel()
{
    return this->_show_progress_panel;
};

void Tutorial::set_show_progress_panel(bool is_visible)
{
    this->_show_progress_panel = is_visible;
};

bool Tutorial::get_show_building_info()
{
    return this->_show_building_info;
};

void Tutorial::set_show_building_info(bool is_visible)
{
    this->_show_building_info = is_visible;
};

void Tutorial::hide_game_ui()
{
    this->set_show_sidebar(false);
    this->set_show_building_buttons(false);
    this->set_show_player_info(false);
    this->set_show_player_hp_lbl(false);
    this->set_show_building_info(false);
    this->set_show_progress_panel(false);
};


TutorialStep::TutorialStep(
        cocos2d::Node* parent, std::string title, std::string body, std::string completion_message
    ) : parent(parent), title(title), body(body),
    completion_message(completion_message), _has_celebrated(false),
    step_index(-1), _scheduled_func([](float dt){})
{
};

void TutorialStep::load_step()
{
    auto tutorial_sidebar_panel = this->parent;

    this->tutorial_title_lbl = dynamic_cast<cocos2d::ui::Text*>(tutorial_sidebar_panel->getChildByName("tutorial_title_lbl"));
    this->tutorial_lbl = dynamic_cast<cocos2d::ui::Text*>(tutorial_sidebar_panel->getChildByName("tutorial_lbl"));
    this->next_tutorial_step_btn = dynamic_cast<cocos2d::ui::Button*>(tutorial_sidebar_panel->getChildByName("next_tutorial_step_btn"));

    this->tutorial_progress_panel = dynamic_cast<cocos2d::ui::Layout*>(tutorial_sidebar_panel->getChildByName("progress_panel"));
    this->tutorial_progress_lbl = dynamic_cast<cocos2d::ui::Text*>(tutorial_progress_panel->getChildByName("label"));
    this->tutorial_loadingbar = dynamic_cast<cocos2d::ui::LoadingBar*>(tutorial_progress_panel->getChildByName("loading_bar"));

    //make tutorial panel visible
    tutorial_sidebar_panel->setVisible(true);

    this->reset();

    this->run_scheduled_func();
};

void TutorialStep::celebrate()
{
    if (this->_has_celebrated == false)
    {
        auto parts = cocos2d::ParticleFireworks::create();
        parts->setName("celebration_particles");
        parts->setPosition(200, 100); //TODO fix hardcoded pos
        this->parent->addChild(parts);

        //change text to complete
        this->tutorial_lbl->setString(this->completion_message);

        //animate showing button
        this->next_tutorial_step_btn->setVisible(true);
        auto scale_to = cocos2d::ScaleTo::create(0.15f, 1.0f);
        this->next_tutorial_step_btn->setScale(0.0f);
        this->next_tutorial_step_btn->runAction(cocos2d::EaseBackOut::create(scale_to));

        bind_button_touch_ended(this->next_tutorial_step_btn, [this](){
            auto tutorial = Tutorial::getInstance();
            tutorial->load_step(this->step_index+1);
        });

        this->_has_celebrated = true;
    };
};

void TutorialStep::set_switch_map(std::vector<std::pair<bool Tutorial::* , bool>> switch_map)
{
    this->_switch_map = switch_map;
};

void TutorialStep::set_scheduled_func(std::function<void(float)> scheduled_func)
{
    this->_scheduled_func = scheduled_func;
};

void TutorialStep::run_scheduled_func()
{
    this->parent->unschedule("scheduled_func");

    if (!this->_scheduled_func)
    {
        CCLOG("this step %i, needs an scheduled func", this->step_index);
        return;
    };

    this->parent->schedule(this->_scheduled_func, SHORT_DELAY, "scheduled_func");
    this->_scheduled_func(0);
};

void TutorialStep::reset()
{
    this->_has_celebrated = false;

    //setup title
    this->tutorial_title_lbl->setString(this->title);
    this->tutorial_title_lbl->unscheduleAllCallbacks();

    //setup body
    this->tutorial_lbl->setString(this->body);
    this->tutorial_lbl->unscheduleAllCallbacks();

    //setup buttons
    prep_button(next_tutorial_step_btn);
    this->next_tutorial_step_btn->setVisible(false);
    this->next_tutorial_step_btn->unscheduleAllCallbacks();

    auto parts = this->parent->getChildByName("celebration_particles");
    if (parts) parts->removeFromParent();


    this->tutorial_progress_lbl->unscheduleAllCallbacks();
    this->tutorial_progress_panel->unscheduleAllCallbacks();
    this->tutorial_loadingbar->unscheduleAllCallbacks();

    //for each of the bool members of tutorial (ideally _show_player_info etc),
    // set it to the bool that matches the switch
    Tutorial* tutorial = Tutorial::getInstance();
    for (std::vector<std::pair<bool Tutorial::*, bool>>::iterator tutorial_member = this->_switch_map.begin(); tutorial_member != this->_switch_map.end(); tutorial_member++)
    {
        //bool data member on tutorial
        bool Tutorial::* tutorial_flag = tutorial_member->first;
        tutorial->*tutorial_flag = tutorial_member->second;
    };
};
