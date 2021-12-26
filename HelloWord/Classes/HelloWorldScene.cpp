/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
// in thông báo lỗi khi file không tìm thấy
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
// bạn cần init để khởi tạo các yêu cầu của bạn
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    // lấy size của màn hình 
    // lấy kích thước màn hình. trả về height và width
    // kiểu dữ liệu trả về cocos2d::size
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // lấy gốc tọa độ của màn hình
    // ở một số thiết bị điện thoại, gốc tọa độ không phải 0 0
    // nên chúng ta cần lấy gốc tọa độ để tính chính xác vị trí
    // kiểu dữ liệu trả về cocos2d::Vec2 x = 0; y = 0
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // thêm background ở đây
    // ? ảnh background lấy position là điểm góc bên trái trên cùng hay là ở trọng tâm của ảnh
    auto background = Sprite::create("background.png");
    background->setPosition(origin + visibleSize / 2);
    this->addChild(background);

    // thêm 1 nhân vật lên trên màn hình và tạo animation cho nhân vật
    // tọa độ 104, 390.0f
    character = Sprite::create("character/character_01.png");
    character->setPosition({104.f, 390.0f});
    this->addChild(character);

    // tạo anime cho nhân vật
    auto characterAnimation = Animation::create();
    // hình ảnh trước và hình ảnh sau 0.15 giây
    characterAnimation->setDelayPerUnit(0.15f);
    // -1 nghiax laf animation chạy xong sẽ được lặp lại
    characterAnimation->setLoops(-1);
    characterAnimation->addSpriteFrame(Sprite::create("character/character_01.png")->getSpriteFrame());
    characterAnimation->addSpriteFrame(Sprite::create("character/character_02.png")->getSpriteFrame());
    characterAnimation->addSpriteFrame(Sprite::create("character/character_03.png")->getSpriteFrame());
    characterAnimation->addSpriteFrame(Sprite::create("character/character_04.png")->getSpriteFrame());
    characterAnimation->addSpriteFrame(Sprite::create("character/character_05.png")->getSpriteFrame());
    characterAnimation->addSpriteFrame(Sprite::create("character/character_06.png")->getSpriteFrame());

    // khởi tạo hoạt động lên anime
    Animate* animate = Animate::create(characterAnimation);
    // cho đối tượng character này paly action animate
    character->runAction(animate);

    // tạo bóng cho nhân vật
    auto shadow = Sprite::create("character/shadow.png");
    // x = vị trí giữa nhân vật, y = 10.5f
    shadow->setPosition(Vec2(character->getContentSize().width / 2, 10.5f));
    // xOrder = -1 là bên duois nhân vật
    character->addChild(shadow, -1);

    // tạo sự kiện touch và keyboqrd cho game
    // B0 chuẩn bị cho một animationCache để tạo nanimation có thể sử dụng lại
    // B1 lấy vị trí nhấp chuột
    // B2 vẽ một hình ảnh tại điểm chuột

    // -> B0
    // tạo hành động như lai thần chưởng
    auto skillAnimation = Animation::create();  
    skillAnimation->setDelayPerUnit(0.15f);     // cho delay 0.15s
    skillAnimation->setLoops(1);                // cho animation loop 1 lần
    
    for (int i = 1; i < 10; i++)                
    {
        skillAnimation->addSpriteFrame(Sprite::create("skill/attack" + std::to_string(i) + ".png")->getSpriteFrame());
    }
    const std::string k_skillAnimationName = "skillAnimation";      // đặt tên gọi skill
    // add vaof cache khi naof cần gọi tên nó ra dùng, không tạo lại
    AnimationCache::getInstance()->addAnimation(skillAnimation, k_skillAnimationName);

    // tạo hành động 3 đường sấm sét
    auto skill2Animation = Animation::create();
    skill2Animation->setDelayPerUnit(0.15f);
    skill2Animation->setLoops(1);
    for (int i = 0; i < 18; i++)
    {
        skill2Animation->addSpriteFrame(Sprite::create("skill_02/skill_02_" + std::to_string(i) + ".png")->getSpriteFrame());
    }
    const std::string k_skill2AnimationName = "skill2Animation";
    AnimationCache::getInstance()->addAnimation(skill2Animation, k_skill2AnimationName);
    // end B1

    
    // tạo sự kiện tourch event
    // cách 1 dùng lamda function 
    // khởi tạo touch event của người chơi tại một điểm
    auto touchListenner = EventListenerTouchOneByOne::create();
    // khi bạn nhấn xuông
    touchListenner->onTouchBegan = [](Touch* touch, Event* event)
    {
        // nơi chứa đoạn code của bạn
        // có thể thêm hiệu ứng gì đấy khi nhấn xuông
        return true;
    };
    // khi bạn vừa nhấn xuống vừa di chuyển
    touchListenner->onTouchMoved = [](Touch* touch, Event* event)
    {
        // code cho hiệu ứng đoạn này
    };
    // khi bạn nhả tay ra
    touchListenner->onTouchEnded = [=](Touch* touch, Event* event)
    {
        // nơi chứa đoạn code của bạn
        auto skill = Sprite::create("skill/attack1.png");
        skill->setPosition(touch->getLocation());
        this->addChild(skill);

        Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation("skillAnimation"));

        // play xong animation, ta sẽ xóa skill đi khỏi màn hình bằng action
        skill->runAction(Sequence::createWithTwoActions(animate, RemoveSelf::create()));
    };
    // thêm Touch event vào cơ chế Event Dispatch
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListenner, this);


    // tạo keyboard event.
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    // Thêm keyboar event vào cơ chế event dispatcher.
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);

    // Di chuyển nhân vật từ vị trí hiện tại, đến vị trí(1020, 390) trong thời gian 5 giây
    character->runAction(MoveTo::create(5.0f, Point(1020.f, 390.0f)));


    /////////////////////////////
    // 2. thêm một item menu với x. khi click vào nó sẽ tắt chương trình
    // thêm một close icon để tắt chương trình. nó là một đối tượng autorelease
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    // cách để xem một giá trị trong quá trình debug
    CCLOG("thu can hien thi: ");

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_A) 
    {
       //auto ss = character->getPosition();
       //character->runAction(MoveTo::create(0.1f, ss + Point(-30, 0)));

       // nơi chứa đoạn code của bạn
       auto skill1 = Sprite::create("skill_02/skill_02_0.png");
       auto skill2 = Sprite::create("skill_02/skill_02_0.png");
       auto skill3 = Sprite::create("skill_02/skill_02_0.png");

       skill1->setPosition(character->getPosition() + Vec2(+10, 0));
       skill1->setPosition(character->getPosition() + Vec2(+10, +30));
       skill1->setPosition(character->getPosition() + Vec2(+20, +10));

       this->addChild(skill1);
       this->addChild(skill2);
       this->addChild(skill3);

       Animate* animate1 = Animate::create(AnimationCache::getInstance()->getAnimation("skill2Animation"));
       Animate* animate2 = Animate::create(AnimationCache::getInstance()->getAnimation("skill2Animation"));
       Animate* animate3 = Animate::create(AnimationCache::getInstance()->getAnimation("skill2Animation"));

       // play xong animation, ta sẽ xóa skill đi khỏi màn hình bằng action
       skill1->runAction(Sequence::createWithTwoActions(animate1, RemoveSelf::create()));
       skill2->runAction(Sequence::createWithTwoActions(animate2, RemoveSelf::create()));
       skill3->runAction(Sequence::createWithTwoActions(animate3, RemoveSelf::create()));
    }
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}


