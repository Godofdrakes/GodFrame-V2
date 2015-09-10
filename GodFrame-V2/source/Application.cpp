#include "Application.h"

#include <GLM/glm.hpp>
#include <GLM/ext.hpp>
#include <GLCORE/gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include "Input/InputDevice.h"

void Application::updateTime() {
    time_current_d_ = glfwGetTime();
    time_elapsed_d_ = time_current_d_ - time_previous_d_;
    time_previous_d_ = time_current_d_;
    time_lag_d_ += time_elapsed_d_;
}

Application::Application() : Application( "TEST" ) {}

Application::Application( const string set_name ) : Application( set_name, 1280, 720 ) {}

Application::Application( const string set_name, const int set_width, const int set_height ) : TICK_PER_SEC_D_( 1.0 / 60.0 ) {
    name_string_ = set_name;
    width_i_ = set_width;
    height_i_ = set_height;
    window_glfw_ = nullptr;

    time_current_d_ = 0.0;
    time_elapsed_d_ = 0.0;
    time_lag_d_ = 0.0;
    time_previous_d_ = 0.0;
}

Application::~Application() {}

ApplicationFail Application::Init() {
    BeforeInit();

    if ( glfwInit() == false ) { return ApplicationFail::GLFW_INIT; }

    window_glfw_ = glfwCreateWindow( 1280, 720, name_string_.c_str(), nullptr, nullptr );
    if ( window_glfw_ == nullptr ) {
        glfwTerminate();
        return ApplicationFail::GLFW_CREATE_WINDOW;
    }
    glfwMakeContextCurrent( window_glfw_ );

    if ( ogl_LoadFunctions() == ogl_LOAD_FAILED ) {
        glfwDestroyWindow( window_glfw_ );
        glfwTerminate();
        return ApplicationFail::OGL_LOAD_FUNCTIONS;
    }

    const int OGL_MAJOR = ogl_GetMajorVersion();
    const int OGL_MINOR = ogl_GetMinorVersion();
    printf( "OpenGL Version: %i.%i\n", OGL_MAJOR, OGL_MINOR );

    glClearColor( 0.25f, 0.25f, 0.25f, 1.0f );
    glEnable( GL_DEPTH_TEST );

    time_previous_d_ = glfwGetTime();

    InputDevice::Init( window_glfw_ );

    AfterInit();

    return ApplicationFail::NONE;
}

void Application::Shutdown() {
    BeforeShutdown();
    glfwDestroyWindow( window_glfw_ );
    glfwTerminate();
    AfterShutdown();
}

bool Application::Tick() {
    InputState escape_key = InputDevice::GetKeyboardKey( GLFW_KEY_ESCAPE );
    if ( glfwWindowShouldClose( window_glfw_ ) ||
         escape_key == INPUT_DOWN || escape_key == INPUT_PRESS ) { return false; }
    BeforeTick();
    glfwSwapBuffers( window_glfw_ );
    glfwPollEvents();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    updateTime();
    AfterTick();
    return true;
}

void Application::Update() {
    while ( time_lag_d_ >= TICK_PER_SEC_D_ ) {
        BeforeUpdate();
        AfterUpdate();
        time_lag_d_ -= TICK_PER_SEC_D_;
    }
}

void Application::Render() {
    BeforeRender();
    AfterRender();
}