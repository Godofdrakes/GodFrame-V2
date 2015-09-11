#include "TextureApp/TextureApp.h"

int main() {
    Application* app = new TextureApp();
    if ( app->Init() == ApplicationFail::NONE ) {
        while ( app->Tick() ) {
            app->Update();
            app->Render();
        }
        app->Shutdown();
    }
    return 0;
}
