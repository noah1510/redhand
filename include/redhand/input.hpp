/**
 * @file input.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref input_system
 * @version 0.13
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

#include "redhand/types.hpp"

#include <memory>
#include <functional>
#include <string>

namespace redhand {

    class engine;

    /**
     * @brief The input_system is used to handle user inputs for all kinds of devices.
     * @todo finish documentation of members
     * 
     */
    class input_system {

      private:
        input_system(void) {
        }

        input_system(input_system const &);

        void operator=(input_system const &);

        engine *m_engine;

      public:
        static input_system &getInstance() {
            static input_system instance;
            return instance;
        }

        static key_actions static_getKeyAction(keyboard_keys wantedKey) {
            return getInstance().getKeyAction(wantedKey);
        };

        static bool static_isKeyPressed(keyboard_keys wantedKey) {
            return getInstance().isKeyPressed(wantedKey);
        };

        static engine *getEngine() {
            return getInstance().m_engine;
        };

        void registerEngine(engine *main_engine);

        key_actions getKeyAction(keyboard_keys wantedKey);

        bool isKeyPressed(keyboard_keys wantedKey);
    };

} // namespace redhand
