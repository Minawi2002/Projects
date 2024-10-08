//
// Created by UNOrganized on 11.12.2023.
//

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>

class Scoreboard : public wxPanel {
public:
    /**
    * \brief Constructor of Scorboard
    * \param: parent to inherit from, position and style information
    * \result Scoreboard is constructed according to parameters
    */
    Scoreboard(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);

    /**
    * \brief tries to add a player to the scoreboard
    * \param: name of the player
    * \result adds the player and his name to the scoreboard
    */
    void AddPlayer(const wxString& playerName);

    /**
    * \brief updates the score of the player
    * \param: name of the player and his new score
    * \result updates the score of the given player to the given value
    */
    void UpdateScore(const wxString& playerName, int score);

    /**
    * \brief should highlight one player
    * \param: player index
    * \result highlights the player with the given index
    */
    void SelectItem(long index);

private:
    wxListCtrl* _listCtrl;
    std::vector<wxString> _playerNames;

    /**
    * \brief Initializes the List of the Scoreboard
    * \result List of the scoreboard is initialized
    */
    void InitializeListCtrl();
};

#endif // SCOREBOARD_H
