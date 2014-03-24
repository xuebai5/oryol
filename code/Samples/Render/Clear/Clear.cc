//------------------------------------------------------------------------------
//  Clear.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "Application/App.h"
#include "Core/Module.h"
#include "IO/Module.h"
#include "Render/Module.h"

OryolApp("Clear", "1.0");

using namespace Oryol;
using namespace Oryol::Application;
using namespace Oryol::Render;
using namespace Oryol::Core;

AppState::Code OryolInit();
AppState::Code OryolRunning();
AppState::Code OryolCleanup();

//------------------------------------------------------------------------------
void
OryolMain() {
    App app;
    app.AddCallback(AppState::Init, &OryolInit);
    app.AddCallback(AppState::Running, &OryolRunning);
    app.AddCallback(AppState::Cleanup, &OryolCleanup);
    app.StartMainLoop();
}

//------------------------------------------------------------------------------
AppState::Code
OryolInit() {
    // setup subsystems
    Core::Module::Setup();
    IO::Module::Setup();
    Render::Module::Setup();
    
    // setup render system
    RenderFacade::Instance()->Setup(RenderSetup::Windowed(400, 300, "Oryol Clear Sample"));
    
    return AppState::Running;
}

//------------------------------------------------------------------------------
AppState::Code
OryolRunning() {
    CoreFacade* coreFacade = CoreFacade::Instance();
    RenderFacade* renderFacade = RenderFacade::Instance();
    
    coreFacade->RunLoop()->Run();
    if (renderFacade->BeginFrame()) {
        // FIXME
        renderFacade->EndFrame();
    }
    if (renderFacade->QuitRequested()) {
        return AppState::Cleanup;
    }
    else {
        return AppState::Running;
    }
}

//------------------------------------------------------------------------------
AppState::Code
OryolCleanup() {
    RenderFacade::Instance()->Discard();

    Render::Module::Discard();
    IO::Module::Discard();
    Core::Module::Discard();
    return AppState::Destroy;
}