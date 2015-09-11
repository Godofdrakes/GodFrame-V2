#include "RenderApp/RenderApp.h"

int main() {
    Application* app = new RenderApp( "Test" );
    if ( app->Init() == ApplicationFail::NONE ) {
        while ( app->Tick() ) {
            app->Update();
            app->Render();
        }
        app->Shutdown();
    }
    return 0;
}