/**
 * @file helper.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Auxillary functions
 * @version 0.1.1
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

#include <cmath>
#include <string>
#include <vector>

namespace redhand {
    class engine;
    
    /**
     * @brief A singlton with a lot of helper functions available to all parts of redhand
     * 
     */
    class helper{
        friend class engine;
      private:
        /**
         * @brief Construct a new helper object
         * @warning this must not be called by anything except getInstatnce()
         * 
         */
        helper(void) {

        };

        ///disable compy constructor
        helper(helper const &);

        ///disable copy assignment
        void operator=(helper const &);

        /**
         * @brief This function returns an instance of the helper
         * 
         * @return input_system& a reference to the helper
         */
        static helper &getInstance() {
            static helper instance;
            return instance;
        }

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
        * @brief the implementation of split_string
        * 
        * @param s the string to split
        * @param delimiter the character where the string will be split
        * @return std::vector<std::string> a vector conatining all the substrings
        */
        std::vector<std::string> impl_split(const std::string& s, char delimiter);

      public:

        /**
        * @brief split a string into multiple substrings
        * 
        * @param s the string to split
        * @param delimiter the character where the string will be split
        * @return std::vector<std::string> a vector conatining all the substrings
        */
        static std::vector<std::string> split_string(const std::string& s, char delimiter){
            return getInstance().impl_split(s,delimiter);
        }

        /**
         * @brief Get a reference to the attached engine, will throw an execption if m_engine is not set.
         * 
         * @return engine& A reference to the atteched engine.
         */
        static engine& getEngine(){
            return getInstance().getEngineReference();
        }

    };

    

} // namespace redhand