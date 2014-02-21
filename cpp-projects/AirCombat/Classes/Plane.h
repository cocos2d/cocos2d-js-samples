//
//  Plane.h
//  onfly
//
//  Created by cocos2dx on 14-2-20.
//
//

#ifndef __onfly__Plane__
#define __onfly__Plane__

#include <iostream>
#include "cocos2d.h"

class Plane : public cocos2d::Sprite {
public:
    static Plane* create(int type);
    int duration;
    int attacked;
    int attackedCount;
};

class Bullet : public cocos2d::Sprite{
public:
    static Bullet* create(int type);
    int attack;
    
};

#endif /* defined(__onfly__Plane__) */
