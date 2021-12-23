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
    auto character = Sprite::create("character/character_01.png");
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

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");
    //if (sprite == nullptr)
    //{
    //    problemLoading("'HelloWorld.png'");
    //}
    //else
    //{
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    //}
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
