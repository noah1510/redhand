#include "redhand/input.hpp"

#include "redhand/engine.hpp"
#include <exception>

void redhand::input_system::registerEngine(engine* main_engine) {
    m_engine = main_engine;
}

redhand::key_actions redhand::input_system::impl_getKeyAction(redhand::keyboard_keys wantedKey) {
    if (this->m_engine->getWindow() == nullptr) {
        return redhand::UNKNOWN;
    }

    return (redhand::key_actions)glfwGetKey(this->m_engine->getWindow(), wantedKey);
}

bool redhand::input_system::impl_isKeyPressed(redhand::keyboard_keys wantedKey) {
    return impl_getKeyAction(wantedKey) == redhand::PRESS;
}

redhand::engine& redhand::input_system::getEngineReference(){
    if (m_engine == nullptr){
        throw new std::invalid_argument("engine not set");
    }else{
        return m_engine->getReference();
    }
}