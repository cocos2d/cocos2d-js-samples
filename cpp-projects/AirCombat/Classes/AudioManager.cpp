//
//  AudioManager.cpp
//  JoGame
//
//  Created by 3q on 13-6-21.
//
//

#include "AudioManager.h"

void AudioManager::init()
{
    const char* arrAudios[] = {"explosion.mp3",
        "shoot.mp3",
        "flip.wav",
        "flag.wav",
        "lose.wav"};
    
	string temp_str;
    int len = sizeof(arrAudios)/ sizeof(arrAudios[0]);
    CCLOG("audios number: %d.", len);
    for (int i=0; i<len; i++) {
        SimpleAudioEngine::getInstance()->preloadEffect(arrAudios[i]);
    }
}

void AudioManager::playAudioById(int idx)
{
	SimpleAudioEngine::getInstance()->playEffect(arrAudios[idx].c_str());
}



////////////////
static AudioManager* _audioManager;
AudioManager* AudioManager::sharedAudioManager()
{
    if (_audioManager == NULL) {
        _audioManager = new AudioManager();
        _audioManager->init();
    }
    
    return _audioManager;
}

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
    
}