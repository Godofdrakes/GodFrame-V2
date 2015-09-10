#pragma once
#ifndef _INPUTDEVICE_H_
#define _INPUTDEVICE_H_

#include <array>

enum InputState {
    INPUT_UP,
    INPUT_PRESS,
    INPUT_DOWN,
    INPUT_RELEASE
};

struct GLFWwindow;

class InputDevice {
    static std::array<InputState, 384> keys;
    static std::array<InputState, 10> buttons;
    static double xpos_old_, ypos_old_, xpos_new_, ypos_new_;
    static void keyboard_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods );
    static void mouse_pos_callback( GLFWwindow* window, double xpos, double ypos );
    static void mouse_button_callback( GLFWwindow* window, int button, int action, int mods );

public:
    static void Init( GLFWwindow* window );
    static InputState GetKeyboardKey( int key );
    static InputState GetMouseButton( int button );
    static void GetMousePos( double& xpos, double& ypos );
    static void GetMousePosDelta( double& xpos, double& ypos );
};

#endif
