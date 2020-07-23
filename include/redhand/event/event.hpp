/**
 * @file event.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief Class @ref event
 * @version 0.13
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

#include <memory>

namespace redhand {

    /**
     * @brief A general class for raising events, specific events types should be a subclass of this general event.
     * 
     * @tparam T The type of the object that raised the event.
     */
    template <class T>
    class event {
      protected:
        ///A pointer to the object that raised the event
        T *m_raiser;

      public:
        /**
        * @brief Construct a new event object as an event raiser
        * 
        * @param raiser 
        */
        event(T *raiser) {
            m_raiser = raiser;
        };

        /**
        * @brief Get the Raiser of the event
        * 
        * @return T* a pointer to the raiser of the event
        */
        T *getRaiser() {
            return m_raiser;
        };
    };

} // namespace redhand
