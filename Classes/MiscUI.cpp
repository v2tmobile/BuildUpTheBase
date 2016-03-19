#include "MiscUI.h"
#include "Util.h"

USING_NS_CC;

FloatingLabel::FloatingLabel(TextHAlignment hAlignment, TextVAlignment vAlignment)
    : Label(hAlignment, vAlignment)
{
    
};

FloatingLabel* FloatingLabel::createWithTTF(const std::string& text, const std::string& fontFile, float fontSize, const Size& dimensions /* = Size::ZERO */, TextHAlignment hAlignment /* = TextHAlignment::LEFT */, TextVAlignment vAlignment /* = TextVAlignment::TOP */)
{
    auto ret = new (std::nothrow) FloatingLabel(hAlignment,vAlignment);

    if (ret && ret->initWithTTF(text, fontFile, fontSize, dimensions, hAlignment, vAlignment))
    {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

FloatingLabel* FloatingLabel::createWithTTF(const TTFConfig& ttfConfig, const std::string& text, TextHAlignment hAlignment /* = TextHAlignment::CENTER */, int maxLineWidth /* = 0 */)
{
    auto ret = new (std::nothrow) FloatingLabel(hAlignment);

    if (ret && ret->initWithTTF(ttfConfig, text, hAlignment, maxLineWidth))
    {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}


void FloatingLabel::do_float()
{
    float duration = 2.0f;

    float x_scale = sx(100*CCRANDOM_0_1());
    float y_scale = sy(100 + (10 * CCRANDOM_0_1()));

    ccBezierConfig config = {
        Vec2(x_scale, y_scale),
        Vec2(x_scale, 1),
        Vec2(1, y_scale)
    };


    this->runAction(TintTo::create(duration, Color3B::RED));
    this->runAction(ScaleBy::create(duration, 0.25f));
    this->runAction(FadeOut::create(duration));
    this->runAction(
        RepeatForever::create(
            Sequence::create(
                BezierBy::create(duration, config),
                BezierBy::create(duration, config),
                NULL
            )
        )
    );

};
