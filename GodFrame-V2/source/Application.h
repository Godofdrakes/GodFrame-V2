#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#include "Timer/Timer.h"
#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glfw3.lib" )
#pragma comment( lib, "GodFrame-V2.lib" )

struct GLFWwindow;

enum ApplicationFail {
    NONE,
    GLFW_INIT,
    GLFW_CREATE_WINDOW,
    OGL_LOAD_FUNCTIONS,
};

/* Example loop

int main() {
    Application* app = new Application();
    if ( app->Init() == ApplicationFail::NONE ) {
        while ( app->Tick() ) {
            app->FixedUpdate();
            app->Update();
            app->Render();
        }
        app->Shutdown();
    }
    return 0;
}

*/

class ApplicationTimer;

class Application {
    friend class ApplicationTimer;
    std::string name_string_;
    int width_i_, height_i_;
    GLFWwindow* window_glfw_;

    ApplicationTimer* timeKeeper;

protected:
    virtual void Init_Logic() {}
    virtual void Shutdown_Logic() {}
    virtual void Tick_Logic() {}
    virtual void FixedUpdate_Logic() {}
    virtual void Update_Logic() {}
    virtual void Render_Logic() {}

public:
    Application();
    Application( const std::string set_name );
    Application( const std::string set_name, const int set_width, const int set_height );
    virtual ~Application();

    ApplicationFail Init();
    void Shutdown();

    bool Tick();
    void FixedUpdate();
    void Update();
    void Render();

};

class ApplicationTimer : public Timer {
    Application* target;
    virtual void Logic() override { target->FixedUpdate_Logic(); }
public:
    ApplicationTimer( Application* target_app ) : Timer( 60.0, 1.0 / 15.0 ), target( target_app ) {}
    ~ApplicationTimer() {}
};

#endif
