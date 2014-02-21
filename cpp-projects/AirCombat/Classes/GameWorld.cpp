//
//  GameWorld.cpp
//  onfly
//
//  Created by cocos2dx on 14-2-20.
//
//

#include "GameWorld.h"
#include "Const.h"
#include "AudioManager.h"
#include "MiniLayer.h"
#include "storage/local-storage/LocalStorage.h"

USING_NS_CC;

Scene* GameWorld::scene()
{
    CCLOG("will gameworld scene.");
    auto scene = Scene::create();
    auto layer = GameWorld::create();
    scene->addChild(layer);
    CCLOG("gameworld scene.");
    return scene;
}

enum TAG {
    TAG_PAUSE = 0,
    TAG_CLOSE
};

bool GameWorld::init()
{
    wSize = Director::getInstance()->getVisibleSize();
    initWithColor(Color4B(240,240,240,240));
    auto spBack = Sprite::create("mainBack.png");
    spBack->setPosition(wSize.width/2, wSize.height/2);
    addChild(spBack);
    setOneStep();
    //pause button
    auto pauseSp = Sprite::create("plane.png", Rect(175, 148, 22, 23));
    auto btnPause = MenuItemSprite::create(pauseSp,
                                           pauseSp,
                                           CC_CALLBACK_1(GameWorld::onClick, this));
    btnPause->setTag(TAG_PAUSE);
    auto sizePause = btnPause->getContentSize();
    btnPause->setPosition(Point(sizePause.width, wSize.height - sizePause.height));
    //close button
    auto btnClose = MenuItemImage::create("CloseNormal.png",
                                          "CloseSelected.png",
                                          CC_CALLBACK_1(GameWorld::onClick, this));
    btnClose->setTag(TAG_CLOSE);
    
    auto abelttf=LabelTTF::create("Resume Game","Thonburi", 48);
    auto color = Color3B(126, 126, 126);
    abelttf->setColor(color);
    itemResume = MenuItemLabel::create(abelttf, CC_CALLBACK_1(GameWorld::onResume, this));
    itemResume->setPosition(wSize.width/2, wSize.height/2);
    itemResume->setVisible(false);
    auto menuResume = Menu::create(itemResume, NULL);
    addChild(menuResume);
    menuResume->setPosition(Point::ZERO);
    
    menu = Menu::create(btnPause, btnClose, NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu);
    
    //score label
    scoreLabel = LabelBMFont::create("0", "konqa32.fnt");
    addChild(scoreLabel);
    scoreLabel->setPosition(btnPause->getPosition().x + btnPause->getContentSize().width + scoreLabel->getContentSize().width/2, btnPause->getPosition().y);
    _myPlane = Sprite::create("plane.png", Rect(2, 168, 62, 75));
    _myPlane->setPosition(Point(wSize.width/2, _myPlane->getContentSize().height/2));
    addChild(_myPlane);
    
    //
    CCLOG("schedule logic.");
    this->schedule(schedule_selector(GameWorld::gamelogic), 0.4);
    this->schedule(schedule_selector(GameWorld::addBullet),0.2);
    this->schedule(schedule_selector(GameWorld::addPackage),rand()%10+10);
    this->schedule(schedule_selector(GameWorld::updateSpeedRate), 5);
    this->schedule(schedule_selector(GameWorld::updategame));
    //
    //_targets = Array::create();
    //_bullets = new Array;
    this->speedRate = 1.0;
    doubleBulletFlag = false;
    onClickFlag = false;
    score = 0;
    
    //srand((unsigned)time(NULL));
    
    //add touch event.
    listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


bool GameWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (isInResume) {
        return false;
    }
    auto location = touch->getLocation();
    auto posPlane = _myPlane->getPosition();
    if(_myPlane->boundingBox().containsPoint(location)){
        onClickFlag = true;
    }
    else{
        CCLOG("touch begin. (%f, %f), (%f, %f).", location.x, location.y, posPlane.x, posPlane.y);
        if( location.x > posPlane.x ){
            moveRight();
        }
        if( location.x < posPlane.x ){
            moveLeft();
        }
        if( location.y > posPlane.y ){
            moveUp();
        }
        if( location.y < posPlane.y ){
            moveDown();
        }
    }
    return true;
}

void GameWorld::onTouchMoved(Touch *touch, Event *unused_event)
{
    //CCLOG("touch move.");
    Point location = touch->getLocation();
    if(onClickFlag){
        _myPlane->setPosition(location);
    }
}

void GameWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
    //CCLOG("touch end.");
    onClickFlag=false;
}

void GameWorld::moveUp(bool oneStep)
{
    CCLOG("moveUp");
    auto pos = _myPlane->getPosition();
    _myPlane->setPositionY(pos.y + _oneStep.y);
}
void GameWorld::moveDown(bool oneStep)
{
    CCLOG("moveDown");
    auto pos = _myPlane->getPosition();
    _myPlane->setPositionY(pos.y - _oneStep.y);
}
void GameWorld::moveLeft(bool oneStep)
{
    CCLOG("moveLeft");
    auto pos = _myPlane->getPosition();
    _myPlane->setPositionX(pos.x - _oneStep.x);
}
void GameWorld::moveRight(bool oneStep)
{
    CCLOG("moveRight");
    LOG_POS(_oneStep);
    auto pos = _myPlane->getPosition();
    _myPlane->setPositionX(pos.x + _oneStep.x);
}

void GameWorld::onPause(){
    Director::getInstance()->pause();
    itemResume->setVisible(true);
    menu->setEnabled(false);
    isInResume = false;
}

void GameWorld::onResume(Object* pSender){
    Director::getInstance()->resume();
    itemResume->setVisible(false);
    menu->setEnabled(true);
    isInResume = true;
}

void GameWorld::gamelogic(float  dt)
{
    addTarget();
}
/* add target */
void GameWorld::addTarget(){
    //CCLOG("addTarget.");
    Plane* target = NULL;
    int index = (rand()%100);
    if(index>=30){
        // adds small plane
        target = Plane::create(PLANE_SMALL);
        target->setTag(1);
    }else if(index>=5){
        // adds mid plane
        target = Plane::create(PLANE_MID);
        target->setTag(2);
    }else{
        // adds big plane
        target = Plane::create(PLANE_BIG);
        target->setTag(3);
    }
    
    //set the target position
    float minX = target->getContentSize().width/2;
    float maxX = wSize.width-target->getContentSize().width/2;
    int rangeX = (int)(maxX-minX);
    int actualX = (rand()%rangeX)+(int)minX;//random X
    auto posT = Point(Director::getInstance()->getVisibleOrigin().x+actualX,wSize.height+target->getContentSize().height/2);
    target->setPosition(posT);
    this->addChild(target,100);
    // set the target action
    auto fDt = target->duration * speedRate;
    auto posTar = Point(Director::getInstance()->getVisibleOrigin().x+actualX,0-target->getContentSize().height);
    auto actionMove = MoveTo::create(fDt, posTar);
    auto actionMoveDone = CallFuncN::create( CC_CALLBACK_1(GameWorld::spriteMoveFinish, this) );
    target->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
    _targets.pushBack(target);
}
/* add bullet */
void GameWorld::addBullet(float dt){
    //CCLOG("add bullet");
    Point origin = Director::getInstance()->getVisibleOrigin();
    Point planePosition = _myPlane->getPosition();
    if(doubleBulletFlag){
        //CCLOG("double bullet");
        // initializes bullet
        auto bulletLeft = Sprite::create(PNG_PLANE, RECT_BULLET);
        auto bulletRight = Sprite::create(PNG_PLANE, RECT_BULLET);
        int bulletDuration = 1;
        
        //sets the left bullet
        bulletLeft->setPosition(Point(planePosition.x-bulletLeft->getContentSize().width,planePosition.y+bulletLeft->getContentSize().height));
        auto actionMoveLeft = CCMoveTo::create(bulletDuration * ((wSize.height - planePosition.y - bulletLeft->getContentSize().height/2)/wSize.height), Point(planePosition.x-bulletLeft->getContentSize().width,origin.y+wSize.height+bulletLeft->getContentSize().height/2));
        auto actionMoveDoneLeft = CallFuncN::create( CC_CALLBACK_1(GameWorld::spriteMoveFinish, this) );
        bulletLeft->runAction(CCSequence::create(actionMoveLeft,actionMoveDoneLeft,NULL));
        bulletLeft->setTag(6);
        
        // sets the right bullet
        bulletRight->setPosition(Point(planePosition.x+bulletRight->getContentSize().width,planePosition.y+bulletRight->getContentSize().height));
        auto actionMoveRight = MoveTo::create(bulletDuration * ((wSize.height - planePosition.y - bulletRight->getContentSize().height/2)/wSize.height), Point(planePosition.x+bulletRight->getContentSize().width,origin.y+wSize.height+bulletRight->getContentSize().height/2));
        auto actionMoveDoneRight = CallFuncN::create( CC_CALLBACK_1(GameWorld::spriteMoveFinish, this) );
        bulletRight->runAction(CCSequence::create(actionMoveRight,actionMoveDoneRight,NULL));
        bulletRight->setTag(6);
        
        _bullets.pushBack(bulletRight);
        _bullets.pushBack(bulletLeft);
        AudioManager::sharedAudioManager()->playAudioById(1);//"shoot.mp3");
        this->addChild(bulletLeft,0);
        this->addChild(bulletRight,0);
    }else {
        //CCLOG("single bullet");
        auto bullet = CCSprite::create(PNG_PLANE, RECT_BULLET);
        
        bullet->setPosition(Point(planePosition.x,planePosition.y+bullet->getContentSize().height));
        
        int bulletDuration = 1;
        auto actionMove = MoveTo::create(bulletDuration * ((wSize.height - planePosition.y - bullet->getContentSize().height/2)/wSize.height), Point(planePosition.x,origin.y+wSize.height+bullet->getContentSize().height/2));
        auto actionMoveDone = CallFuncN::create( CC_CALLBACK_1(GameWorld::spriteMoveFinish, this));
        bullet->runAction(Sequence::create(actionMove,actionMoveDone,NULL));
        bullet->setTag(6);
        _bullets.pushBack(bullet);
        this->addChild(bullet,0);
    }
}

void GameWorld::addPackage(float dt){
    CCLOG("add package");
    
    auto package = Plane::create(PLANE_DOUBLE);
    Point origin = Director::getInstance()->getVisibleOrigin();
    float minX = package->getContentSize().width/2;
    float maxX = wSize.width-package->getContentSize().width/2;
    int rangeX = (int)(maxX-minX);
    int actualX = (rand()%rangeX)+(int)minX;//random X
    package->setPosition(Point(Director::getInstance()->getVisibleOrigin().x+actualX, wSize.height+package->getContentSize().height/2));
    // set the target action
    auto actionMove = CCMoveTo::create(package->duration, Point(origin.x+actualX,0-package->getContentSize().height/2));
    auto actionMoveDone = CCCallFuncN::create( CC_CALLBACK_1(GameWorld::spriteMoveFinish, this) );
    package->runAction(CCSequence::create(actionMove,actionMoveDone,NULL));
    package->setTag(4);
    this->addChild(package,2);
    _targets.pushBack(package);
}
//


void GameWorld::updateSpeedRate(float dt){
    //CCLOG("rate:%f",speedRate);
    this->speedRate *= 0.90;
}

//
void GameWorld::updategame(float dt){
    //CCLOG("update game");
    
    Vector<Plane*> targets2Delete;
    //CCLOG("111, _targets: %zd.", _targets.size());
    Vector<Plane*>::iterator iter;//;
    for (iter = _targets.begin(); iter != _targets.end(); iter++) {
        // target的碰撞体积
        Plane* target = dynamic_cast<Plane*>(*iter);
        Rect targetRect = target->boundingBox();
        // plane的碰撞矩形
        Rect planeRect = _myPlane->boundingBox();
        // plane与target的碰撞检测
        if(targetRect.intersectsRect(planeRect)){
            if(target->getTag() == 4){
                CCLOG("package\n");
                // if package and plane collide
                //doubleBulletFlag = true;
                this->scheduleOnce(schedule_selector(GameWorld::setDoubleBulletFlagF), 5);
                targets2Delete.pushBack(target);
            } else {
                CCLOG("game end.");
                // if enemy and plane collide
                auto gameOverScene = GameOverScene::create();
                gameOverScene->getLayer()->getLabel()->setString(" ");
                gameOverScene->getLayer()->setCurrentScore(score);
                // converts 'int' to 'string'
                std::stringstream ss;
                std::string str;
                ss<<score;
                ss>>str;
                const char *pHighScore = str.c_str();
                // converts 'const char *' to 'int'
                const char *highScore = localStorageGetItem("high_score").c_str();
                if(highScore != NULL ){
                    int highScoreInt = std::atoi(highScore);
                    // If higher than the highest score ,your score will replace it;
                    if(highScoreInt<score)
                        CCLOG("high_score: %s.", pHighScore);
                        localStorageSetItem("high_score", pHighScore);
                }else{
                    localStorageSetItem("high_score", pHighScore);
                    CCLOG("high_score: %s.", pHighScore);
                }
                gameOverScene->getLayer()->getScore()->setString(pHighScore);
                Director::getInstance()->replaceScene(gameOverScene);
            }
        }
        
        //CCLOG("222, bullet: %zd.", _bullets.size());
        Vector<Sprite*> bullets2Delete;
        Vector<Sprite*>::iterator iterB;
        //bullet与target的碰撞
        for (iterB = _bullets.begin(); iterB != _bullets.end(); iterB++) {
        //CCARRAY_FOREACH(_bullets, bulletsIterator){
            auto bullet = dynamic_cast<Sprite*>(*iterB);
            Rect bulletRect = bullet->boundingBox();
            if(targetRect.intersectsRect(bulletRect)){
                target->attacked++;
                //CCLOG("target attacked: %d.", target->attacked);
                bullets2Delete.pushBack(bullet);
            }
        }
        
        /*
         CCLOG("333");
        for (iterB = bullets2Delete.begin(); iterB != bullets2Delete.end(); iter++) {
        //CCARRAY_FOREACH(bullets2Delete, bulletsIterator){
            auto bullet = dynamic_cast<Sprite*>(*iterB);
            _bullets.eraseObject(bullet);
            this->removeChild(bullet);
        }*/
        
        if(target->attacked >= target->attackedCount){
            targets2Delete.pushBack(target);
        }
        bullets2Delete.clear();
    }
    
    //CCLOG("444, targets2Delete: %zd.", targets2Delete.size());
    for (iter = targets2Delete.begin(); iter != targets2Delete.end(); iter++) {
    //CCARRAY_FOREACH(targets2Delete, targetIterator){
        auto target = dynamic_cast<Plane*>(*iter);
        _targets.eraseObject(target);
        if(target->getTag() == 1){
            score+=10;
        }else if(target->getTag() == 2){
            score+=20;
        }else if(target->getTag() == 3){
            score+=50;
            //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("explosion.mp3");
        }else if(target->getTag() == 4){
            CCLOG("target is the package");
            this->scheduleOnce(schedule_selector(GameWorld::setDoubleBulletFlagF), 5);
            doubleBulletFlag = true;
        }
        this->removeChild(target);
        std::stringstream ss;
        std::string str;
        ss<<score;
        ss>>str;
        const char *p = str.c_str();
        scoreLabel->setString(p);
        //CCLOG("888");
    }
    targets2Delete.clear();
    //CCLOG("555");
}

void GameWorld::setDoubleBulletFlagF(float dt)
{
    doubleBulletFlag = false;
}

void GameWorld::spriteMoveFinish(Node * sender)
{
    //CCLOG("move finish");
    auto sprite =  (Plane*)sender;
    if(sprite->getTag()==1||sprite->getTag()==2||sprite->getTag()==3||sprite->getTag()==4||sprite->getTag()==5){//target
        //tag 1 is smallplane
        //tag 2 is midplane
        //tag 3 is bigplane
        //tag 4 is package
        _targets.eraseObject(sprite);
    }else if(sprite->getTag()==6){//bullet
        _bullets.eraseObject(sprite);
    }
    this->removeChild(sprite, true);
}

void GameWorld::onClick(Object* pSender)
{
    CCLOG("on btn click");
    onPause();
}
