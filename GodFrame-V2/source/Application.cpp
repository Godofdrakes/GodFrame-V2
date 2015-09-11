#include "Application.h"

#include <GLCORE/gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include "Input/InputDevice.h"

Application::Application() : Application( "TEST" ) {}

Application::Application( const std::string set_name ) : Application( set_name, 1280, 720 ) {}

Application::Application( const std::string set_name, const int set_width, const int set_height ) :
    timeKeeper( nullptr ) {
    name_string_ = set_name;
    width_i_ = set_width;
    height_i_ = set_height;
    window_glfw_ = nullptr;
}

Application::~Application() {}

ApplicationFail Application::Init() {
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

    InputDevice::Init( window_glfw_ );

    Init_Logic();

    timeKeeper = new ApplicationTimer( this );

    return ApplicationFail::NONE;
}

void Application::Shutdown() {
    Shutdown_Logic();
    delete( timeKeeper );
    glfwDestroyWindow( window_glfw_ );
    glfwTerminate();
}

bool Application::Tick() {
    InputState escape_key = InputDevice::GetKeyboardKey( GLFW_KEY_ESCAPE );
    if ( glfwWindowShouldClose( window_glfw_ ) ||
         escape_key == INPUT_DOWN || escape_key == INPUT_PRESS ) { return false; }
    glfwSwapBuffers( window_glfw_ );
    glfwPollEvents();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    Tick_Logic();
    return true;
}

void Application::FixedUpdate() { timeKeeper->CatchUp(); }

void Application::Update() { Update_Logic(); }

void Application::Render() { Render_Logic(); }