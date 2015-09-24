#include "FrameApp/FrameApp.h"

int main() {
    Application* app = new FrameApp();
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
