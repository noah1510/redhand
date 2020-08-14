#include "redhand/input.hpp"
#include "redhand/engine.hpp"
#include "redhand/helper.hpp"

#include <exception>



redhand::key_actions redhand::input_system::impl_getKeyAction(redhand::keyboard_keys wantedKey) {
    if (helper::getEngine().getWindow() == nullptr) {
        return redhand::UNKNOWN;
    }

    return (redhand::key_actions)glfwGetKey(helper::getEngine().getWindow(), wantedKey);
}

bool redhand::input_system::impl_isKeyPressed(redhand::keyboard_keys wantedKey) {
    return impl_getKeyAction(wantedKey) == redhand::PRESS;
}

