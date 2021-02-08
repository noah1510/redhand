/**
 * @file input.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref input_system
 * @version 0.1.1
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
#include <stdexcept>

namespace redhand {

    class engine;

    /**
     * @brief The input_system is used to handle user inputs for all kinds of devices.
     * 
     */
    class input_system {
      private:
        friend class redhand::engine;
        
        /**
         * @brief Construct a new input system object
         * @warning this must not be called by anything except getInstatnce()
         * 
         */
        input_system(void) {

        };

        ///disable compy constructor
        input_system(input_system const &);

        ///disable copy assignment
        void operator=(input_system const &);

        /**
         * @brief This function implements the getKeyAction function
         * 
         * @param wantedKey 
         * @return key_actions 
         */
        key_actions impl_getKeyAction(keyboard_keys wantedKey);

        /**
         * @brief inplements the isKeyPressed function
         * 
         * @param wantedKey the key you want to know about
         * @return true the key is pressed
         * @return false the key is not pressed
         */
        bool impl_isKeyPressed(keyboard_keys wantedKey);
        
        double scroll_x = 0.0;
        double scroll_y = 0.0;
        
        static void registerScrollingEvent(double xoffset, double yoffset){
            getInstance().scroll_x += xoffset;
            getInstance().scroll_y += yoffset;
        }
        
        double impl_getScrollX();
        double impl_getScrollY();

        /**
         * @brief This function returns an instance of the input_system
         * 
         * @return input_system& a reference to the input_system
         */
        static input_system &getInstance() {
            static input_system instance;
            return instance;
        }

      public:

        /**
         * @brief returns the state of a given key
         * 
         * @param wantedKey the key you want to know about
         * @return key_actions the current state of that key
         */
        static key_actions getKeyAction(keyboard_keys wantedKey) {
            return getInstance().impl_getKeyAction(wantedKey);
        };

        /**
         * @brief check if a given key is pressed
         * 
         * @param wantedKey the key you want to know about
         * @return true the key is pressed
         * @return false the key is not pressed
         */
        static bool isKeyPressed(keyboard_keys wantedKey) {
            return getInstance().impl_isKeyPressed(wantedKey);
        };
        
        static double getScrollX(){
            return getInstance().impl_getScrollX();
        }
        
        static double getScrollY(){
            return getInstance().impl_getScrollY();
        }
        
        /**
         * @warning will be removed in 0.2.0
         * @todo remove
         */
        static key_actions static_getKeyAction(keyboard_keys wantedKey) {
            return input_system::getKeyAction(wantedKey);
        };

        /**
         * @warning will be removed in 0.2.0
         * @todo remove
         */
        static bool static_isKeyPressed(keyboard_keys wantedKey) {
            return input_system::isKeyPressed(wantedKey);
        };
        
    };

} // namespace redhand
