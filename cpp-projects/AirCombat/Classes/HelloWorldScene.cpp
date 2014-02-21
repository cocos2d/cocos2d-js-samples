#include "HelloWorldScene.h"
#include "GameWorld.h"
#include "MiniLayer.h"
#include "Const.h"
#include "storage/local-storage/LocalStorage.h"

USING_NS_CC;

Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //Point origin = Director::getInstance()->getVisibleOrigin();
    //this->initWithColor(Color4B(126, 126, 126, 126)); //back color to back image.
    auto spBack = Sprite::create("mainBack.png");
    spBack->setPosition(visibleSize.width/2, visibleSize.height/2);
    addChild(spBack);
    
    std::string rPath = FileUtils::getInstance()->getWritablePath();
    filePath = rPath + "onFly.dat";
    localStorageInit(filePath);

    float btn_x = visibleSize.width/2;
    float btn_y = visibleSize.height*0.65;
    // create menu, it's an autorelease object
    auto menu = Menu::create();
    menu->setPosition(Point::ZERO);
    std::string strName[] = {"Start Game", "High Scores", "Quit"};
    for (int i=0; i<3; i++) {
        auto lb = LabelTTF::create(strName[i].c_str(), "Thonburi", 48);
        auto btn = MenuItemLabel::create(lb, CC_CALLBACK_1(HelloWorld::onClick, this));
        btn->setTag(i);
        btn->setPosition(btn_x, btn_y - (lb->getContentSize().height  + 20)* i);
        menu->addChild(btn);
    }
    addChild(menu);
    
    return true;
}

void HelloWorld::onClick(Object* pSender)
{
    //CCLOG("on btn click1111");
    int tag = ((Node*)pSender)->getTag();
    switch (tag) {
        case TAG_START:
            startGame();
            break;
        case TAG_SCORE:
            highScore();
            break;
        case TAG_QUIT:
            exitGame();
            break;
            
        default:
            break;
    }
}

void HelloWorld::startGame(){
    auto scene = GameWorld::scene();
    Director::getInstance()->replaceScene(scene);
}

void HelloWorld::highScore(){
    auto scene = HighScoreScene::create();
    Director::getInstance()->replaceScene(scene);
}

void HelloWorld::exitGame(){
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
