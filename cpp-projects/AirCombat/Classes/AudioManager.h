//
//  AudioManager.h
//  JoGame
//
//  Created by 3q on 13-6-21.
//
//

#ifndef __JoGame__AudioManager__
#define __JoGame__AudioManager__

#include <iostream>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace std;
USING_NS_CC;

enum AUDIO {
    AUDIO_LOSE = 0,
    AUDIO_WIN = 1,
    AUDIO_FLIP = 2,
    AUDIO_FLAG = 3,
    AUDIO_BOOM = 4
};

class AudioManager {
private:
    string arrAudios[10];
public:
    
    void init();
    void playAudioById(int idx);
    
    static AudioManager* sharedAudioManager();
    
    AudioManager();
    ~AudioManager();
};

#endif /* defined(__JoGame__AudioManager__) */
