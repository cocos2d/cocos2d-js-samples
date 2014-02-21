//
//  MiniLayer.cpp
//  onfly
//
//  Created by cocos2dx on 14-2-20.
//
//

#include "MiniLayer.h"
#include "HelloWorldScene.h"
#include "GameWorld.h"
#include "storage/local-storage/LocalStorage.h"

USING_NS_CC;

//------------------game over layer and scene--------------------//
bool GameOverScene::init()
{
    if (Scene::init()) {
        this->_layer = GameOver::create();
        this->_layer->retain();
        this->addChild(_layer);
        return true;
    }else{
        return false;
    }
}

GameOverScene::~GameOverScene(){
    if(_layer){
        _layer->release();
        _layer = NULL;
    }
}

bool GameOver::init()
{
    if(GameOver::initWithColor(Color4B(126, 126, 126, 126))){
        auto winSize = Director::getInstance()->getVisibleSize();
        auto spBack = Sprite::create("gameBack.png");
        spBack->setPosition(winSize.width/2, winSize.height/2);
        addChild(spBack);
        auto spScore = Sprite::create("yourScore.png");
        spScore->setPosition(winSize.width/2, winSize.height/2);
        addChild(spScore);
        //score
        this->_score = LabelTTF::create("0", "Thonburi", 55);
        _score->retain();
        _score->setPosition(Point(winSize.width/2, winSize.height/2));
        //restart
        auto _share = CCLabelTTF::create("Start", "Thonburi", 40);
        _share->setColor(Color3B::WHITE);
        auto shareMenuItem = MenuItemLabel::create(_share, CC_CALLBACK_1(GameOver::start, this));
        _share->retain();
        //adds
        this->_label = CCLabelTTF::create("0", "Thonburi", 40);
        _label->retain();
        _label->setPosition(Point(winSize.width/2, winSize.height/2+_label->getContentSize().height/2+_score->getContentSize().height/2));
        //mainmenu
        LabelTTF* _mainmenu = CCLabelTTF::create("MainMenu", "Thonburi", 40);
        _mainmenu->setColor(Color3B::WHITE);
        auto mainmenuMenuItem = MenuItemLabel::create(_mainmenu, CC_CALLBACK_1(GameOver::mainmenu, this));
        _mainmenu->retain();
        Menu* menu = Menu::create(mainmenuMenuItem, shareMenuItem, NULL);
        menu->setPosition(Point(_score->getPositionX(),_score->getPositionY()-winSize.height/4));
        menu->alignItemsHorizontallyWithPadding(40);
        this->addChild(menu);
        this->addChild(_label);
        this->addChild(_score);
        
        auto callGameOver = CallFuncN::create( CC_CALLBACK_1(GameOver::gameOverDone, this));;
        this->runAction(Sequence::create(DelayTime::create(5), callGameOver, NULL));
        return true;
    }
    else{
        return false;
    }
}

void GameOver::gameOverDone(Object* sender)
{
    CCLOG("gameOverDone.");
}

void GameOver::start(cocos2d::Object* sender)
{
    CCLOG("start to a new game.");
    Director::getInstance()->replaceScene( GameWorld::scene() );
}

void GameOver::mainmenu(Object* sender)
{
    Director::getInstance()->replaceScene( HelloWorld::scene() );
}

GameOver::~GameOver(){
    if(_label){
        _label->release();
        _label = NULL;
    }
    if(_score){
        _score->release();
        _score = NULL;
    }
}


//------------------high score layer and scene--------------------//
bool HighScoreScene::init(){
    bool flag = false;
    scoreLayer = HighScoreLayer::create();
    this->addChild(scoreLayer);
    flag = true;
    return flag;
}

bool HighScoreLayer::init(){
    if(HighScoreLayer::initWithColor(Color4B(255, 255, 255, 255))){
        auto winSize = Director::getInstance()->getVisibleSize();
        auto spBack = Sprite::create("scoreBack.png");
        spBack->setPosition(winSize.width/2, winSize.height/2);
        addChild(spBack);
        const char* highScore = localStorageGetItem("high_score").c_str();
        if(highScore!= NULL){
            highestLabel = LabelTTF::create(highScore, "Thonburi", 48);
        }else{
            highestLabel = LabelTTF::create("0", "Thonburi", 48);
        }
        
        //
        this->_label = LabelTTF::create("Your Highest Score", "Thonburi", 40);
        _label->retain();
        _label->setColor(Color3B::WHITE);
        _label->setPosition(Point(winSize.width/2, winSize.height/2+_label->getContentSize().height/2+highestLabel->getContentSize().height/2+20));
        
        //mainmenu
        auto _mainmenu = LabelTTF::create("MainMenu", "Thonburi", 40);
        auto mainmenuMenuItem = MenuItemLabel::create(_mainmenu, CC_CALLBACK_1(HighScoreLayer::mainmenu, this));
        _mainmenu->setColor(Color3B::WHITE);
        
        auto _startmenu = LabelTTF::create("Start", "Thonburi", 40);
        auto startmenuMenuItem = MenuItemLabel::create(_startmenu, CC_CALLBACK_1(HighScoreLayer::start, this));
        _startmenu->setColor(Color3B::WHITE);
        startmenuMenuItem->setPosition(0, _mainmenu->getContentSize().height + 20);
        
        Menu* menu = Menu::create(mainmenuMenuItem, startmenuMenuItem, NULL);
        menu->setPosition(Point(winSize.width/2, winSize.height/4));
        menu->alignItemsHorizontallyWithPadding(40);
        this->addChild(_label);
        this->addChild(menu);
        
        //highestLabel->setColor(Color3B::WHITE);
        highestLabel->setPosition(Point(winSize.width/2, winSize.height/2));
        this->addChild(highestLabel);
        return true;
    }else
        return false;
}

void HighScoreLayer::mainmenu(Object* sender){
    auto scene = HelloWorld::scene();
    Director::getInstance()->replaceScene(scene);
}
void HighScoreLayer::start(Object* sender){
    auto scene = GameWorld::scene();
    Director::getInstance()->replaceScene(scene);
}

//------------------data manager--------------------//

static DataManager* _dataInstance = NULL;

DataManager* DataManager::getInstance()
{
    if (_dataInstance == NULL) {
        _dataInstance = new DataManager();
        _dataInstance->init();
    }
    return _dataInstance;
}

DataManager::DataManager()
{
    bInit = false;
}
DataManager::~DataManager()
{
    
}

void DataManager::init()
{
    localStorageInit();
    
    bInit = true;
}
void DataManager::setItem(const char *item, const char *content)
{
    localStorageSetItem(item, content);
}
const char* DataManager::getItem(const char *item)
{
    return localStorageGetItem(item).c_str();
}
