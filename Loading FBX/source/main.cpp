#include "FbxApp/FbxApp.h"

int main() {
    Application* app = new FbxApp();
    if ( app->Init() == ApplicationFail::NONE ) {
        while ( app->Tick() ) {
            app->Update();
            app->Render();
        }
        app->Shutdown();
    }
    return 0;
}
