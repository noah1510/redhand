#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include <audio/wave.h>

namespace redhand
{
    class AudioHandler
    {
        private:
            void checkErrors();
            static inline ALenum to_al_format(short channels, short samples);
            void createDefaultDevice(ALCdevice *device);
            
            ALCdevice *device;
            ALCenum error;

        public:
            AudioHandler();
            //~AudioHandler();
            static void listAudioDevices(const ALCchar *devices);
            void playSound(const char*);
    };
}