#include <stdio.h>
#include <string>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include <audio/wave.h>

namespace redhand
{
    class AudioHandler
    {
        private:
            void CheckErrors();
            static inline ALenum to_al_format(short channels, short samples);
        public:
            AudioHandler();
            //~AudioHandler();
            void PlaySound(const char*);
    };
}