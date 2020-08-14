#pragma once

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "sndfile.h"

#include "al.h"
#include "alc.h"


namespace redhand {
    class audio{
        private:
            ALuint buffer;
            int InitAL();
            void CloseAL(void);
            void al_nssleep(unsigned long nsec);
        public:
            audio();
            bool isInitialized;
            void play();
            ALuint LoadSound(const char *filename);
    };
}