#pragma once
#ifndef _FLYCAMERA_H_
#define _FLYCAMERA_H_

#include <glm/glm.hpp>
#include "Camera.h"

using glm::vec2;

class FlyCamera : public Camera {
    int up_key_, down_key_, left_key_, right_key_, front_key_, back_key_;
    float speed_f_;

public:
    FlyCamera( const float speed_f );
    ~FlyCamera();

    virtual void Update() override;

    int up_key() const { return up_key_; }
    void set_up_key( const int up_key ) { up_key_ = up_key; }
    __declspec( property( get = up_key, put = set_up_key ) ) int UpKey;

    int down_key() const { return down_key_; }
    void set_down_key( const int down_key ) { down_key_ = down_key; }
    __declspec( property( get = down_key, put = set_down_key ) ) int DownKey;

    int left_key() const { return left_key_; }
    void set_left_key( const int left_key ) { left_key_ = left_key; }
    __declspec( property( get = left_key, put = set_left_key ) ) int LeftKey;

    int right_key() const { return right_key_; }
    void set_right_key( const int right_key ) { right_key_ = right_key; }
    __declspec( property( get = right_key, put = set_right_key ) ) int RightKey;

    float speed_f() const { return speed_f_; }
    void set_speed_f( const float speed_f ) { speed_f_ = speed_f; }
    __declspec( property( get = speed_f, put = set_speed_f ) ) float SpeedF;
};

#endif
