#include "Harvestable.h"
#include "FShake.h"

#include "Beatup.h"
#include "HouseBuilding.h"
#include "ShatterNode.h"
#include "MiscUI.h"
#include "GameLogic.h"
#include "Util.h"

USING_NS_CC;

void Harvestable::init_sprite()
{
    this->sprite = cocos2d::Sprite::createWithSpriteFrameName("weapon_ice.png");
    this->sprite->setScale(4);

    auto sprite_size = this->sprite->getContentSize();
    this->setContentSize(sprite_size*this->sprite->getScale());

    this->sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->sprite->setPosition(get_relative(this->getContentSize()));

    this->clip->addChild(this->sprite);
}

bool Harvestable::init()
{
    this->setTouchEnabled(true); //true otherwise it cant be clicked
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    this->building = BUILDUP->target_building;

    this->clip = cocos2d::ClippingNode::create();
    this->stencil = cocos2d::DrawNode::create();

    this->clip->setStencil(this->stencil);
    this->clip->setInverted(true);
    this->addChild(this->clip);

    //NOTE: this needs to add the sprite to this->clip
    this->init_sprite();

    this->click_limit = 15;
    this->current_clicks = 0;

    return true;
};

void Harvestable::animate_harvest()
{
    auto size = 20.0f;
    Vec2 origin = Vec2(
        this->sprite->getContentSize().width*this->sprite->getScaleX()*CCRANDOM_0_1(),
        this->sprite->getContentSize().height*this->sprite->getScaleY()*CCRANDOM_0_1()
    );

    Vec2 destination = origin +Vec2(size, size);
    this->stencil->drawSolidRect(origin, destination, Color4F::MAGENTA);

    this->runAction(FShake::actionWithDuration(0.075f, 2.5f));

    auto building = BUILDUP->target_building;
    building->create_resources(Resource::Ingredient, 1, building->punched_ingredient_type);

    float click_ratio = static_cast<float>(this->current_clicks) / this->click_limit;
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

void Harvestable::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    //TODO dont dig out points already dug out OR dig consecutively more from the top
    this->current_clicks += 1;
    if (this->current_clicks < this->click_limit) {
        this->animate_harvest();
    } else {
        this->shatter();

        GameLogic::getInstance()->add_total_harvests(1);
        CCLOG("total of %i harvests now", GameLogic::getInstance()->get_total_harvests());
    };
};

void Harvestable::shatter()
{
    this->sprite->setVisible(false);
    this->setTouchEnabled(false);

    float sprite_scale = this->sprite->getScale();
    Size sprite_size = this->sprite->getContentSize();

    auto shatter_sprite = ShatterSprite::createWithSpriteFrame(this->sprite->getSpriteFrame());
    shatter_sprite->setScale(sprite_scale);
    shatter_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    shatter_sprite->setPosition(get_relative(this->getContentSize()));
    this->addChild(shatter_sprite);

    shatter_sprite->setOpacity(0); //hide this so it shatters it doesnt leave anything behind

    //spawn label //TODO fix hardcoded name
    auto floating_label = FloatingLabel::createWithTTF("+1 Harvest", DEFAULT_FONT, 24);
    floating_label->setPosition(
            this->getPosition() + Vec2(sprite_size*sprite_scale)
        );
    floating_label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    this->getParent()->addChild(floating_label);
    floating_label->do_float();

    CallFunc* remove = CallFunc::create([this](){ this->removeFromParent(); });

    auto shatter_action = ShatterAction::create(0.75f);
    shatter_sprite->runAction(Sequence::createWithTwoActions(shatter_action, remove));

};

void MiningHarvestable::init_sprite()
{

    float scale = 4;
    auto create_sprite = [this,scale](std::string sprite_path, Vec2 relative_pos)
    {
        auto sprite = Sprite::createWithSpriteFrameName(sprite_path);
        sprite->setScale(scale);
        sprite->setAnchorPoint(relative_pos);

        auto pos = get_relative(this->getContentSize(), relative_pos);
        sprite->setPosition(pos);

        return sprite;

    };


    auto sprite_size = Size(15, 16);
    sprite_size.width *= scale * 2;
    sprite_size.height *= scale * 2;
    this->setContentSize(sprite_size);

    auto top_left_sprite = create_sprite("dirt_1.png", Vec2::ANCHOR_TOP_LEFT);
    auto top_right_sprite = create_sprite("dirt_2.png", Vec2::ANCHOR_TOP_RIGHT);
    auto bot_left_sprite = create_sprite("dirt_2.png", Vec2::ANCHOR_BOTTOM_LEFT);
    auto bot_right_sprite = create_sprite("dirt_1.png", Vec2::ANCHOR_BOTTOM_RIGHT);



    auto rt = RenderTexture::create(this->getContentSize().width, this->getContentSize().height);
    rt->retain();
    rt->begin();
    top_left_sprite->visit();
    top_right_sprite->visit();
    bot_left_sprite->visit();
    bot_right_sprite->visit();
    rt->end();

    this->sprite = Sprite::createWithTexture(rt->getSprite()->getTexture());

    this->sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->sprite->setPosition(get_relative(this->getContentSize()));
    this->clip->addChild(this->sprite);
    
};

void MiningHarvestable::animate_harvest()
{
    auto building = BUILDUP->target_building;
    building->create_resources(Resource::Ingredient, 1, building->punched_ingredient_type);

    float click_ratio = static_cast<float>(this->current_clicks) / this->click_limit;

    auto sprite_size = Size(
            this->sprite->getContentSize().width*this->sprite->getScaleX(),
            this->sprite->getContentSize().height*this->sprite->getScaleY()
            );

    auto size = 20.0f;
    Vec2 origin = Vec2(
        MIN(sprite_size.width, sprite_size.width - size)*CCRANDOM_0_1(), //random along the width, dont go so far right
        MIN(sprite_size.height-(click_ratio*sprite_size.height), sprite_size.height - size) //from the top, go a percentage of the way down
    );

    Vec2 destination = origin + Vec2(size, size);
    this->stencil->drawSolidRect(origin, destination, Color4F::MAGENTA);

    this->runAction(FShake::actionWithDuration(0.075f, 2.5f));


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
