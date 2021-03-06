#include "GameLayer.h"
#ifdef __ANDROID__
#include "DataManager.h"
#endif
#include "Util.h"
#include "SoundEngine.h"
#include "Serializer.h"
#include "base/CCDirector.h"
#include "ui/UIScrollView.h"
#include "ui/CocosGUI.h"

#include "GameLogic.h"

std::string GameLayer::menu_font = DEFAULT_FONT;
float GameLayer::menu_fontsize = 0;
float GameLayer::menu_heightdiff = 0;

bool GameLayer::touch_in_node(Node* target, cocos2d::Touch* touch, float scale)
{
    auto bbox = target->getBoundingBox();
    if (scale != 1)
    {
        cocos2d::Rect new_rect(bbox);

        new_rect.size.width = bbox.size.width*scale;
        new_rect.size.height = bbox.size.height*scale;

        new_rect.origin.x -= (bbox.size.width / 2.0f)*(scale - 1.0f);// - bbox.size.width );
        new_rect.origin.y -= (bbox.size.height / 2.0f)*(scale - 1.0f);// - bbox.size.height);

        bbox.setRect(
            new_rect.origin.x,
            new_rect.origin.y,
            new_rect.size.width,
            new_rect.size.height
        );
    }
    if (bbox.containsPoint(this->convertTouchToNodeSpace(touch)))
    {
        return true;
    }
    return false;
};

bool GameLayer::vec2_in_node(Node* target, cocos2d::Vec2 pos)
{
    auto bbox = target->getBoundingBox();
    if (bbox.containsPoint(pos))
    {
        return true;
    }
    return false;
};

bool GameLayer::vec2_in_rect(cocos2d::Rect* rect, cocos2d::Vec2 pos)
{
    if (rect->containsPoint(pos))
    {
        return true;
    }
    return false;
}

bool GameLayer::init()
{

    //TODO this might actually solve all of my scaling problems at once.
    // if I take the time to remove all my scaling stuff, and fallback on this,
    // I might be in business
    // this->setScaleX(sx(1, true));
    // this->setScaleY(sy(1, true));

    isTouchDown = false;

    initialTouchPos[0] = 0;
    initialTouchPos[1] = 0;

    return true;
};

void GameLayer::update(float dt)
{
    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    if (true == isTouchDown)
    {
        if (initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.05)
        {
            this->onSwipeLeft(dt);
            isTouchDown = false;
        }
        else if (initialTouchPos[0] - currentTouchPos[0] < - visibleSize.width * 0.05)
        {
            this->onSwipeRight(dt);
            isTouchDown = false;
        }
        else if (initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05)
        {
            this->onSwipeDown(dt);
            isTouchDown = false;
        }
        else if (initialTouchPos[1] - currentTouchPos[1] < - visibleSize.width * 0.05)
        {
            this->onSwipeUp(dt);
            isTouchDown = false;
        }
        else if (initialTouchPos[0] == currentTouchPos[0] && initialTouchPos[1] == currentTouchPos[1])
        {
            // CCLOG("no move?");
        }
    }
};

bool GameLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    initialTouchPos[0] = touch->getLocation().x;
    initialTouchPos[1] = touch->getLocation().y;
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;

    isTouchDown = true;

    return true;
}

void GameLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
}

void GameLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    isTouchDown = false;
}

void GameLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    onTouchEnded(touch, event);
}

void GameLayer::onSwipeLeft(float dt)
{
    cocos2d::log("SWIPED LEFT");
};

void GameLayer::onSwipeUp(float dt)
{
    CCLOG("SWIPED UP");
};

void GameLayer::onSwipeRight(float dt)
{
    CCLOG("SWIPED RIGHT");
};

void GameLayer::onSwipeDown(float dt)
{
    CCLOG("SWIPED DOWN");
};

void GameLayer::pop_scene(Ref* pSender)
{
    cocos2d::Director::getInstance()->popToRootScene(); //sloppy way to delete scene, idk a better way yet
};

void GameLayer::quit(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
    return;
#endif

    GameLogic::save_all();

    CCLOG("QUITTING");
    cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
};


cocos2d::ui::ScrollView* GameLayer::create_center_scrollview()
{
    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto scroll = cocos2d::ui::ScrollView::create();
    //scroll->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    float scroll_w = sx(800);
    float scroll_h = sy(400);
    cocos2d::Size scroll_size = cocos2d::Size(scroll_w, scroll_h);
    scroll->setContentSize(scroll_size);

    scroll->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    scroll->setPosition( get_center_pos(0, -sy(50) ));
    scroll->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);

    this->addChild(scroll);
    //scroll->setBackGroundColor(Color3B::GREEN);

    //auto container_size = Size(scroll_w, scroll_h*3);
    // scroll->setInnerContainerSize(container_size);
    scroll->setLayoutType(cocos2d::ui::Layout::Type::VERTICAL);

    // scroll->setBounceEnabled(true); //this sucks, needs to be smoother
    scroll->setScrollBarAutoHideEnabled(false);

    return scroll;
}

cocos2d::ui::Button* GameLayer::create_button(std::string text, BoolFuncNoArgs cb, bool start_disabled, ButtonState* btn_state)
{
    auto param = cocos2d::ui::LinearLayoutParameter::create();
    param->setGravity(cocos2d::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    auto disable_button = [](cocos2d::ui::Button* button)
    {
        button->setTitleColor(cocos2d::Color3B::GRAY);
        button->setTouchEnabled(false);
        button->setPressedActionEnabled(false);
    };

    auto btn = cocos2d::ui::Button::create("main_UI_export_10_x4.png", "", "", cocos2d::ui::Widget::TextureResType::PLIST);

    btn->setTitleFontName(menu_font);
    btn->setTitleFontSize(menu_fontsize);
    btn->setTitleColor(cocos2d::Color3B::BLACK);
    btn->getTitleRenderer()->getFontAtlas()->setAliasTexParameters();

    btn->setColor(cocos2d::Color3B(114, 160, 72));

    btn->getVirtualRenderer()->setScale(sx(1.0f));
    btn->setScaleX(sx(1.0f));
    btn->setScaleY(sy(1.0f));

    param->setMargin(cocos2d::ui::Margin(0, sy(15.0f)*btn->getScaleY(), 0, sy(15.0f)*btn->getScaleY()));

    btn->setLayoutParameter(param);
    btn->setTouchEnabled(true);
    btn->setPressedActionEnabled(true);

    if (start_disabled)
    {
        disable_button(btn);
    };

    btn->setTitleText(text);
    if (btn_state)
    {
        //update the text
        btn_state->swap_state(btn, btn_state->current);
    };

    btn->addTouchEventListener(
        [btn_state, cb, btn, disable_button](Ref* ref, cocos2d::ui::Widget::TouchEventType event)
            {
                if (event == cocos2d::ui::Widget::TouchEventType::ENDED)
                {
                    if (!cb())
                    {
                        disable_button(btn);
                    };
                    if (btn_state)
                    {
                        btn_state->current = !btn_state->current;
                        btn_state->swap_state(btn, btn_state->current);
                    };
                }
            }
    );

    return btn;

};

void GameLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent)
{
};

void GameLayer::onEnter()
{
    cocos2d::Layer::onEnter();
    if (!SoundEngine::is_sound_enabled())
    {
        SoundEngine::stop_all_sound_and_music();
    }
};

void GameLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        CCLOG("GameLayer::onKeyReleased: popScene");
        cocos2d::Director::getInstance()->popScene();
        event->stopPropagation();
    }
};
