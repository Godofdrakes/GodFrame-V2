#include "Camera.h"

void Camera::SetAspectRatio( CameraAspectRatio aspect ) {
    switch ( aspect ) {
        case CameraAspectRatio::FOUR_THREE:
        aspectRatio_width_ = 4.f;
        aspectRatio_height_ = 3.f;
        break;

        case CameraAspectRatio::SIXTEEN_NINE:
        aspectRatio_width_ = 16.f;
        aspectRatio_height_ = 9.f;
        break;

        case CameraAspectRatio::SIXTEEN_TEN:
        aspectRatio_width_ = 16.f;
        aspectRatio_height_ = 10.f;
        break;
    }
}


Camera::Camera() {
    world_transform_mat4_ = mat4( 1 );
    view_transform_mat4_ = mat4( 1 );
    camera_transform_mat4_ = mat4( 1 );
    camera_view_transform_mat4_ = mat4( 1 );
    position_vec3_ = vec3( 0 );
    lookAt_vec3_ = vec3( 0 );
    up_vec3_ = vec3( 0, 1, 0 );
    aspectRatio_width_ = 16.f;
    aspectRatio_height_ = 9.f;
}

Camera::~Camera() {}


void Camera::SetPerspective( const float set_fov, CameraAspectRatio set_aspectRatio, const float set_near, const float set_far ) {
    SetAspectRatio( set_aspectRatio );
    camera_transform_mat4_ = glm::perspective( set_fov, aspectRatio_width_ / aspectRatio_height_, set_near, set_far );
}

void Camera::SetOrthographic( CameraAspectRatio set_aspectRatio, const float set_near, const float set_far ) {
    SetAspectRatio( set_aspectRatio );
    camera_transform_mat4_ = glm::ortho( aspectRatio_width_ / -2.f, aspectRatio_height_ / 2.f, set_near / -2.f, set_near / 2.f, set_near, set_far );
}

void Camera::SetLookAt( const vec3 set_from, const vec3 set_to, const vec3 set_up ) {
    position_vec3_ = set_from;
    lookAt_vec3_ = set_to;
    up_vec3_ = set_up;
    view_transform_mat4_ = glm::lookAt( position_vec3_, lookAt_vec3_, up_vec3_ );
    world_transform_mat4_ = glm::inverse( view_transform_mat4_ );
}

void Camera::SetPosition( const vec3 set_position ) {
    position_vec3_ = set_position;
    view_transform_mat4_ = glm::lookAt( position_vec3_, lookAt_vec3_, up_vec3_ );
    world_transform_mat4_ = glm::inverse( view_transform_mat4_ );
}

void Camera::UpdateProjectionViewTransform() {
    camera_view_transform_mat4_ = camera_transform_mat4_ * view_transform_mat4_;
}