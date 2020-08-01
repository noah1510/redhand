#include "redhand/input.hpp"

#include "redhand/engine.hpp"

void redhand::input_system::registerEngine(engine *main_engine) {
    m_engine = main_engine;
}

redhand::key_actions redhand::input_system::getKeyAction(redhand::keyboard_keys wantedKey) {
    if (this->m_engine == nullptr || this->m_engine->getWindow() == nullptr) {
        return redhand::UNKNOWN;
    }

    return (redhand::key_actions)glfwGetKey(this->m_engine->getWindow(), wantedKey);
}

bool redhand::input_system::isKeyPressed(redhand::keyboard_keys wantedKey) {
    return getKeyAction(wantedKey) == redhand::PRESS;
}