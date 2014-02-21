//
//  MiniLayer.h
//  onfly
//
//  Created by cocos2dx on 14-2-20.
//
//

#ifndef __onfly__MiniLayer__
#define __onfly__MiniLayer__

#include <iostream>
#include "cocos2d.h"

class GameOver : public cocos2d::LayerColor
{
public:
    GameOver():_label(NULL){}
    virtual ~GameOver();
    bool init();
    void gameOverDone(cocos2d::Object* sender);
    void start(cocos2d::Object* sender);
    void mainmenu(cocos2d::Object* sender);
    CREATE_FUNC(GameOver);
    
    CC_SYNTHESIZE_READONLY(cocos2d::LabelTTF*, _label, Label);
    CC_SYNTHESIZE_READONLY(cocos2d::LabelTTF*, _score, Score);
    CC_SYNTHESIZE(int, score, CurrentScore);
};

class GameOverScene : public cocos2d::Scene{
public:
    GameOverScene():_layer(NULL){}
    ~GameOverScene();
    CREATE_FUNC(GameOverScene);
    bool init();
    CC_SYNTHESIZE_READONLY(GameOver*, _layer , Layer);
};


class HighScoreLayer:public cocos2d::LayerColor{
public:
    bool init();
    void mainmenu(cocos2d::Object* sender);
    void start(cocos2d::Object* sender);
    CREATE_FUNC(HighScoreLayer);
    CC_SYNTHESIZE(cocos2d::LabelTTF*, highestLabel, HighestLabel);
    CC_SYNTHESIZE(cocos2d::LabelTTF*, _label, Label);
};

class HighScoreScene:public cocos2d::Scene{
public:
    bool init();
    CREATE_FUNC(HighScoreScene);
    CC_SYNTHESIZE(HighScoreLayer*, scoreLayer, ScoreLayer);
};

class DataManager{
private:
    bool bInit;
    void init();
public:
    static DataManager* getInstance();
    DataManager();
    ~DataManager();
    
    const char* getItem(const char* item);
    void setItem(const char* item, const char* content);
};

#endif /* defined(__onfly__MiniLayer__) */
