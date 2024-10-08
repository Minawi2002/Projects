//
// Created by Software Engineering TA-Team in 2021.
// Updates by UNOrganized on 11.12.2023.
//

#include "GameWindow.h"


GameWindow::GameWindow(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    // Set up layout that will contain and center all content
    this->_mainLayout = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    outerLayout->Add(this->_mainLayout, 1, wxCENTER);
    this->SetSizerAndFit(outerLayout);
    this->_currentPanel = nullptr;

    // Set up status bar
    this->_statusBar = this->CreateStatusBar(1);

    // Set background
    wxColor cream = wxColour(254,251,234);
    this->SetBackgroundColour(cream);

    // Set the minimum size of the window. The user won't be able to resize the window to a size smaller than this
    this->SetMinSize(wxSize(1050, 780));

    //build Darkmode Button
    wxButton* darkModeSwitchButton = new wxButton(this, wxID_ANY, "colormode", wxDefaultPosition, wxSize(80, 20));
    _mainLayout->Add(darkModeSwitchButton, 0, wxALIGN_TOP | wxALIGN_RIGHT | wxTOP | wxRIGHT, 20);
    darkModeSwitchButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
        this->toggleDarkMode();
    });
    darkModeSwitchButton->SetToolTip("Switch color mode");

}


void GameWindow::showPanel(wxPanel* panel) {

    // if we are already showing the panel, we don't need to do anything
    if(this->_currentPanel == panel) {
        return;
    }

    // remove previous panel
    if(this->_currentPanel != nullptr) {
        this->_mainLayout->Detach(this->_currentPanel);
        this->_currentPanel->Show(false);
        this->_currentPanel = nullptr;
    }

    // add new panel
    this->_mainLayout->Add(panel, 0, wxALIGN_CENTER | wxRIGHT | wxBOTTOM | wxLEFT, 20); // 20 pixel spacing
    panel->Show(true);
    this->_currentPanel = panel;

    // update layout
    this->_mainLayout->Layout();

    // update window size
    this->Layout();
}


void GameWindow::setStatus(const std::string& message) {
    this->_statusBar->SetStatusText(message, 0);
}

void GameWindow::toggleDarkMode(){
    if(!_darkModeEnabled){
        _darkModeEnabled = true;
    }else{
        _darkModeEnabled = false;
    }
    updateUIForDarkMode();
}

void GameWindow::updateUIForDarkMode(){
    wxColor darkgrey = wxColour(37, 40, 42);
    wxColor cream = wxColour(254,251,234);
    wxColor white = wxColour(255,255,255);
    wxColor black = wxColour(0,0,0);

    if(_darkModeEnabled){
        this->SetBackgroundColour(darkgrey);
        this->SetForegroundColour(white);
    }else{
        this->SetBackgroundColour(cream);
        this->SetForegroundColour(black);
    }
}