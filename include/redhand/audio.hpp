#pragma once

#include "al.h"
#include "alc.h"

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