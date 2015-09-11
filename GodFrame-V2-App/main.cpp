#include <Application.h>

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