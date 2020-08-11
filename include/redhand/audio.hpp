#pragma once

#include "AL/al.h"
#include "AL/alc.h"

#include <stdio.h>
#include <iostream>

namespace redhand {
    class audio{
        private:
            int InitAL();
            void CloseAL(void);
        public:
            void play();
    };
}