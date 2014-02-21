//
//  GameWorld.h
//  onfly
//
//  Created by cocos2dx on 14-2-20.
//
//

#ifndef __onfly__GameWorld__
#define __onfly__GameWorld__

#include <iostream>
#include "cocos2d.h"
#include "Plane.h"

class GameWorld : public cocos2d::LayerColor
{
private:
    float speedRate;
    int score;
    bool onClickFlag;
    bool doubleBulletFlag;
    cocos2d::Menu* menu;
    cocos2d::Size wSize;
    cocos2d::Sprite* _myPlane;
    cocos2d::LabelBMFont* scoreLabel;
    cocos2d::Layer* pauseLayer;
    cocos2d::MenuItemLabel* itemResume;
    cocos2d::EventListenerTouchOneByOne* listener;
    
    cocos2d::Vector<Plane*> _targets;
    cocos2d::Vector<cocos2d::Sprite*> _bullets;
    
    void gamelogic(float dt);
    void addTarget();
    void addBullet(float dt);
    void updategame(float dt);
    void addPackage(float dt);
    
    void updateSpeedRate(float dt);
    void setDoubleBulletFlagF(float dt);
    void spriteMoveFinish(cocos2d::Node * sender);
    
    void onPause();
    void onResume(cocos2d::Object* pSender);
    CC_SYNTHESIZE(int, isInResume, inResume);
    
    cocos2d::Point _oneStep;
public:
    static cocos2d::Scene* scene();
    virtual bool init();
    
    void onClick(cocos2d::Object* pSender);
    
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void moveUp(bool oneStep=true);
    void moveDown(bool oneStep=true);
    void moveLeft(bool oneStep=true);
    void moveRight(bool oneStep=true);
    
    void setOneStep(cocos2d::Point step=cocos2d::Point(30, 30)){_oneStep = step;};
    cocos2d::Point getOneStep(){return _oneStep;};
    
    CREATE_FUNC(GameWorld);
};

#endif /* defined(__onfly__GameWorld__) */
