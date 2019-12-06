
#include "AudioHandler.hpp"

namespace audio{
    AudioHandler::AudioHandler(){

    }
    AudioHandler::~AudioHandler(){

    }
    sf::Sound AudioHandler::loadSound(std::string audioFilePath){
        sf::SoundBuffer buffer;
        if(!buffer.loadFromFile(audioFilePath))
        {
            std::cout << "LMAO" << std::endl;
        }
        sf::Sound sound;
        sound.setBuffer(buffer);
        return sound;
    }
}