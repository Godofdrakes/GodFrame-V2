#include "FlyCamera.h"
#include <GLFW/glfw3.h>
#include "../Input/InputDevice.h"
#include <glm/ext.hpp>

using glm::mat4;

FlyCamera::FlyCamera( const float speed_f )
    : up_key_( GLFW_KEY_W ), down_key_( GLFW_KEY_S ),
    left_key_( GLFW_KEY_A ), right_key_( GLFW_KEY_D ),
    speed_f_( speed_f ) {}

FlyCamera::~FlyCamera() {}


void FlyCamera::Update() {
    // Conflicting keys should just cancel eachother out.
    if ( InputDevice::GetKeyboardKey( up_key_ ) == INPUT_DOWN || InputDevice::GetKeyboardKey( up_key_ ) == INPUT_PRESS ) {
        position_vec3_.y += speed_f_;
    }
    if ( InputDevice::GetKeyboardKey( down_key_ ) == INPUT_DOWN || InputDevice::GetKeyboardKey( down_key_ ) == INPUT_PRESS ) {
        position_vec3_.y -= speed_f_;
    }
    if ( InputDevice::GetKeyboardKey( left_key_ ) == INPUT_DOWN || InputDevice::GetKeyboardKey( left_key_ ) == INPUT_PRESS ) {
        position_vec3_.x -= speed_f_;
    }
    if ( InputDevice::GetKeyboardKey( right_key_ ) == INPUT_DOWN || InputDevice::GetKeyboardKey( right_key_ ) == INPUT_PRESS ) {
        position_vec3_.x += speed_f_;
    }

    /*vec2 mouse_new = Input::GetMousePosition();
    vec2 mouse_delta = mouse_new - mouse_old_vec2_;
    mat4 rotation = mat4( 1 );*/

    SetLookAt( position_vec3_, lookAt_vec3_, up_vec3_ );
}