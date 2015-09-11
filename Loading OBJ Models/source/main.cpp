#include "ObjApp/ObjApp.h"

int main() {
    Application* app = new ObjApp( "ObjApp" );

    if( app->Init() == ApplicationFail::NONE ) {
        while( app->Tick() ) {
            app->FixedUpdate();
            app->Update();
            app->Render();
        }
        app->Shutdown();
    }

    return 0;
}