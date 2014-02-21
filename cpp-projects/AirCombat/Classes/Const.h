//
//  Const.h
//  onfly
//
//  Created by cocos2dx on 14-2-20.
//
//

#ifndef onfly_Const_h
#define onfly_Const_h

//type of plane
#define RECT_SMALL Rect(201, 88, 39, 27)
#define RECT_MID Rect(130, 2, 69, 89)
#define RECT_BIG Rect(2, 2, 108, 164)
#define RECT_DOUBLE Rect(144, 134, 29, 46)
#define RECT_BULLET Rect(66, 237, 7, 20)

enum PLANE_TYPE {
    PLANE_SMALL,
    PLANE_MID,
    PLANE_BIG,
    PLANE_DOUBLE,
    PLANE_BULLET
};

enum BUTTON_TAG{
    TAG_START,
    TAG_SCORE,
    TAG_QUIT
};

enum BULLET_TYPE{
    BULLET_SMALL,   //小型子弹
    BULLET_MID,     //中型子弹
    BULLET_BIG,     //大型子弹
    BULLET_FLY      //导弹
};


//plane png
#define PNG_PLANE "plane.png"

#define LOG_RECT(rect) CCLOG("rect(%f, %f, %f, %f).", rect.size.width, rect.size.height, rect.origin.x, rect.origin.y)
#define LOG_POS(pos) CCLOG("pos(%f, %f).", pos.x, pos.y)

#endif
