//
// Created by Software Engineering TA-Team in 2021.
//

#ifndef TEST_CONNECTIONPANEL_H
#define TEST_CONNECTIONPANEL_H

#include <wx/wx.h>
#include "../uiElements/InputField.h"


class ConnectionPanel : public wxPanel {

public:
    /**
    * \brief constructs the ConnectionPanel
    * \param: the parent game window that should be inherited
    * \result ConnectionPanel is constructed
    */
    ConnectionPanel(wxWindow* parent);

    /**
    * \brief get the server adress
    * \result returns the server adress
    */
    wxString getServerAddress();

    /**
    * \brief get the server port
    * \result returns the server port
    */
    wxString getServerPort();

    /**
    * \brief get the player name
    * \result return the player name
    */
    wxString getPlayerName();

private:
    InputField* _serverAddressField;
    InputField* _serverPortField;
    InputField* _playerNameField;

};


#endif //TEST_CONNECTIONPANEL_H
