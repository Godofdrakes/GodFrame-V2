#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

enum CameraAspectRatio {
    SIXTEEN_NINE,
    SIXTEEN_TEN,
    FOUR_THREE
};

class Camera {
protected:
    mat4 world_transform_mat4_;
    mat4 view_transform_mat4_;
    mat4 camera_transform_mat4_;
    mat4 camera_view_transform_mat4_;
    vec3 position_vec3_, lookAt_vec3_, up_vec3_;
    float aspectRatio_width_, aspectRatio_height_;
    void SetAspectRatio( CameraAspectRatio aspect );

public:
    Camera();
    virtual ~Camera();

    virtual void Update() = 0;

    void SetPerspective( const float set_fov, CameraAspectRatio set_aspectRatio, const float set_near, const float set_far );
    void SetOrthographic( CameraAspectRatio set_aspectRatio, const float set_near, const float set_far );
    void SetLookAt( const vec3 set_from, const vec3 set_to, const vec3 set_up );
    void SetPosition( const vec3 set_position );

    void UpdateProjectionViewTransform();

    // Getter/Setter
    mat4 Get_World_Transform_Mat4() const {
        return world_transform_mat4_;
    }
    __declspec( property( get = Get_World_Transform_Mat4 ) ) mat4 World_Transform_Mat4;

    mat4 Get_View_Transform_Mat4() const {
        return view_transform_mat4_;
    }
    __declspec( property( get = Get_View_Transform_Mat4 ) ) mat4 View_Transform_Mat4;

    mat4 Get_Camera_Transform_Mat4() const {
        return camera_transform_mat4_;
    }
    __declspec( property( get = Get_Camera_Transform_Mat4 ) ) mat4 Camera_Transform_Mat4;

    mat4 Get_Camera_View_Transform_Mat4() const {
        return camera_view_transform_mat4_;
    }
    __declspec( property( get = Get_Camera_View_Transform_Mat4 ) ) mat4 Camera_View_Transform_Mat4;
};

#endif
