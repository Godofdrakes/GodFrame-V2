#include "LightApp/LightApp.h"

int main() {
    Application* app = new LightApp();
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
