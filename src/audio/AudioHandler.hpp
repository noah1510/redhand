#pragma once

#include <iostream>
#include <SFML/Audio.hpp>
#include <string>

namespace audio
{
    class AudioHandler
    {
        public:
            AudioHandler();
            ~AudioHandler();
            static sf::Sound loadSound(const std::string);
    };
}