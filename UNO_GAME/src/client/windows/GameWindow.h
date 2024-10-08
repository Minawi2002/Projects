//
// Created by Software Engineering TA-Team in 2021.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNOUI_GAMEWINDOW_H
#define UNOUI_GAMEWINDOW_H

#include "../panels/ConnectionPanel.h"


class GameWindow : public wxFrame
{
public:
/**
    * \brief constructs GameWindow and sets original layout
    * \param: the title, position and size of the window
    * \result well formated Gamewindow with status bar and colormode button
    */
    GameWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

    /**
    * \brief shows the panel we want to the client to show
    * \param: the panel we want to show
    * \result shows the panel we want the client to show
    */
    void showPanel(wxPanel* panel);

    /**
    * \brief updates the message in the status bar
    * \param: the message that should be shown
    * \result shows the desired message in the status bar
    */
    void setStatus(const std::string& message);

    /**
    * \brief switches between lightmode and darkmode on the game window
    * \result changed the colormode displayed, changed the darkModeEnabled variable
    */
    void toggleDarkMode();

private:
    wxBoxSizer* _mainLayout;
    wxStatusBar* _statusBar;

    wxPanel* _currentPanel;

    bool _darkModeEnabled;

    /**
    * \brief changes back- and foregroundcolor
    * \result changes back- and foregroundcolor
    */
    void updateUIForDarkMode();

};


#endif //UNOUI_GAMEWINDOW_H
