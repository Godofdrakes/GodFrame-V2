#include "InputDevice.h"
#include <GLFW/glfw3.h>

std::array<InputState, 384> InputDevice::keys;
std::array<InputState, 10> InputDevice::buttons;
double InputDevice::xpos_old_, InputDevice::ypos_old_, InputDevice::xpos_new_, InputDevice::ypos_new_;

void InputDevice::keyboard_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if ( keys[key] == INPUT_UP ) {
        if ( action == GLFW_PRESS || action == GLFW_REPEAT ) {
            keys[key] = INPUT_PRESS;
        } else if ( action == GLFW_RELEASE ) {
            // Do nothing
        }
    } else if ( keys[key] == INPUT_PRESS ) {
        if ( action == GLFW_PRESS || action == GLFW_REPEAT ) {
            keys[key] = INPUT_DOWN;
        } else if ( action == GLFW_RELEASE ) {
            keys[key] = INPUT_RELEASE;
        }
    } else if ( keys[key] == INPUT_DOWN ) {
        if ( action == GLFW_PRESS || action == GLFW_REPEAT ) {
            // Do nothing
        } else if ( action == GLFW_RELEASE ) {
            keys[key] = INPUT_RELEASE;
        }
    } else if ( keys[key] == INPUT_RELEASE ) {
        if ( action == GLFW_PRESS || action == GLFW_REPEAT ) {
            keys[key] = INPUT_PRESS;
        } else if ( action == GLFW_RELEASE ) {
            keys[key] = INPUT_UP;
        }
    }
}

void InputDevice::mouse_pos_callback( GLFWwindow* window, double xpos, double ypos ) {
    xpos_old_ = xpos_new_;
    ypos_old_ = ypos_new_;
    xpos_new_ = xpos;
    ypos_new_ = ypos;
}

void InputDevice::mouse_button_callback( GLFWwindow* window, int button, int action, int mods ) {
    if ( buttons[button] == INPUT_UP ) {
        if ( action == GLFW_PRESS ) {
            buttons[button] = INPUT_PRESS;
        } else if ( action == GLFW_RELEASE ) {
            // Do nothing
        }
    } else if ( buttons[button] == INPUT_PRESS ) {
        if ( action == GLFW_PRESS ) {
            buttons[button] = INPUT_DOWN;
        } else if ( action == GLFW_RELEASE ) {
            buttons[button] = INPUT_RELEASE;
        }
    } else if ( buttons[button] == INPUT_DOWN ) {
        if ( action == GLFW_PRESS ) {
            // Do nothing
        } else if ( action == GLFW_RELEASE ) {
            buttons[button] = INPUT_RELEASE;
        }
    } else if ( buttons[button] == INPUT_RELEASE ) {
        if ( action == GLFW_PRESS ) {
            buttons[button] = INPUT_PRESS;
        } else if ( action == GLFW_RELEASE ) {
            buttons[button] = INPUT_UP;
        }
    }
}

void InputDevice::Init( GLFWwindow* window ) {
    xpos_old_ = xpos_new_ = 0.0;
    ypos_old_ = ypos_new_ = 0.0;
    for ( int n = 0; n < keys.size(); ++n ) {
        keys[n] = INPUT_UP;
    }
    for ( int n = 0; n < buttons.size(); ++n ) {
        buttons[n] = INPUT_UP;
    }
    glfwSetKeyCallback( window, keyboard_key_callback );
    glfwSetCursorPosCallback( window, mouse_pos_callback );
    glfwSetMouseButtonCallback( window, mouse_button_callback );
}

InputState InputDevice::GetKeyboardKey( int key ) { return keys[key]; }

InputState InputDevice::GetMouseButton( int button ) { return buttons[button]; }

void InputDevice::GetMousePos( double& xpos, double& ypos ) {
    xpos = xpos_new_;
    ypos = ypos_new_;
}

void InputDevice::GetMousePosDelta( double& xpos, double& ypos ) {
    xpos = xpos_new_ - xpos_old_;
    ypos = ypos_old_ - ypos_new_;
}