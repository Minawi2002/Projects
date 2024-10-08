//
// Created by Software Engineering TA-Team in 2021.
//

#include "Uno.h"


// Application entry point
bool Uno::OnInit()
{
    // Allow loading of JPEG  and PNG image files
    wxImage::AddHandler(new wxJPEGHandler());
    wxImage::AddHandler(new wxPNGHandler());

    // Open main game window
    GameWindow* gameWindow = new GameWindow(
            "UNO", // title of window,
            wxDefaultPosition, // position of the window
            wxDefaultSize // size of the window
    );
    gameWindow->Show(true);

    // Initialize game controller
    GameController::init(gameWindow);
    gameWindow->Layout();

    return true;
}