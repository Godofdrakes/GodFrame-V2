#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#pragma comment( lib, "year2-Rendering.lib" )

using std::string;

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
            app->Update();
            app->Render();
        }
        app->Shutdown();
    }
    return 0;
}

*/

class Application {
    string name_string_;
    int width_i_, height_i_;
    GLFWwindow* window_glfw_;

    const double TICK_PER_SEC_D_;
    double time_current_d_, time_previous_d_, time_elapsed_d_, time_lag_d_;

    void updateTime();

protected:
    virtual void BeforeInit() {};
    virtual void AfterInit() {};

    virtual void BeforeShutdown() {};
    virtual void AfterShutdown() {};

    virtual void BeforeTick() {};
    virtual void AfterTick() {};

    virtual void BeforeUpdate() {};
    virtual void AfterUpdate() {};

    virtual void BeforeRender() {};
    virtual void AfterRender() {};

public:
    Application();
    Application( const string set_name );
    Application( const string set_name, const int set_width, const int set_height );
    virtual ~Application();

    ApplicationFail Init();
    void Shutdown();

    bool Tick();
    void Update();
    void Render();

};

#endif
