//
//  Plane.cpp
//  onfly
//
//  Created by cocos2dx on 14-2-20.
//
//

#include "Plane.h"
#include "Const.h"

USING_NS_CC;

/***---------------------Plane----------------------***/
Plane* Plane::create(int type){
    Plane *plane = new Plane();
    Rect rect;
    int minDuration = 2.5;
    int maxDuration = 4;
    switch (type) {
        case PLANE_SMALL:{
            rect = RECT_SMALL;
            plane->attackedCount = 1;
        }break;
        case PLANE_MID:{
            minDuration = 3.5;
            maxDuration = 4.5;
            rect = RECT_MID;
            plane->attackedCount = 3;
        }break;
        case PLANE_BIG:{
            minDuration = 5;
            maxDuration = 5;
            rect = RECT_BIG;
            plane->attackedCount = 10;
        }break;
        case PLANE_DOUBLE:{
            minDuration = 3.5;
            maxDuration = 3.5;
            rect = RECT_DOUBLE;
        }break;
        case PLANE_BULLET:{
            rect = RECT_BULLET;
        }break;
            
        default:
            break;
    }
    int rangeDuration = maxDuration - minDuration;
    int actualDuration = minDuration;
    if(rangeDuration != 0)
        actualDuration += (rand()%rangeDuration);
    plane->duration = actualDuration;
    plane->attacked = 0;
    if (plane && plane->initWithFile(PNG_PLANE, rect))
    {
        plane->autorelease();
        return plane;
    }
    CC_SAFE_DELETE(plane);
    return NULL;
}


/***---------------------Bullet----------------------***/
const char* bullets[] = {
    "smallBullet",
    "midBullet",
    "bigBullet",
    "flyBullet",
};
Bullet* Bullet::create(int type){
    auto sp = new Bullet();
    int attack;
    switch (type) {
        case BULLET_SMALL:
            attack = 1;
            break;
        case BULLET_MID:
            attack = 2;
            break;
        case BULLET_BIG:
            attack = 3;
            break;
        case BULLET_FLY:
            attack = 6;
            break;
            
        default:
            break;
    }
    sp->attack = attack;
    if (sp && sp->initWithSpriteFrameName(bullets[type]))
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return NULL;
}

