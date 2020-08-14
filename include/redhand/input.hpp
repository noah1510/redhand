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
#include <stdexcept>

namespace redhand {

    class engine;

    /**
     * @brief The input_system is used to handle user inputs for all kinds of devices.
     * 
     */
    class input_system {
        friend class engine;
      private:
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
         * @brief a pointer to the attached engine
         * 
         */
        engine* m_engine;

        /**
         * @brief Get a reference to the attached engine, will throw an execption if m_engine is not set.
         * 
         * @return engine& A reference to the atteched engine.
         */
        engine& getEngineReference();

        /**
         * @brief This function sets m_engine
         * 
         * @param main_engine the engine that will be attached
         */
        void registerEngine(engine* main_engine);

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

        /**
         * @brief Get the active engine, this will throw an error if no engine is attached
         * 
         * @return engine& A reference to the active engine
         */
        static engine& getEngine(){
            return getInstance().getEngineReference();
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
