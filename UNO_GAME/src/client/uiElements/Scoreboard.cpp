//
// Created by UNOrganized on 11.12.2023.
//

#include "Scoreboard.h"

Scoreboard::Scoreboard(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style) {
    InitializeListCtrl();
}

void Scoreboard::InitializeListCtrl() {
    _listCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);

    // Add columns to the list control
    _listCtrl->InsertColumn(0, wxT("Player"), wxLIST_FORMAT_LEFT, 150);
    _listCtrl->InsertColumn(1, wxT("Score"), wxLIST_FORMAT_RIGHT, 100);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(_listCtrl, 1, wxEXPAND);
    SetSizer(sizer);
}

void Scoreboard::AddPlayer(const wxString& playerName) {
    _playerNames.push_back(playerName);

    // Add player to the list control
    long index = _listCtrl->InsertItem(_listCtrl->GetItemCount(), playerName);
    _listCtrl->SetItem(index, 1, wxT("0")); // Initialize score to 0
}

void Scoreboard::UpdateScore(const wxString& playerName, int score) {
    // Find the index of the player
    auto it = std::find(_playerNames.begin(), _playerNames.end(), playerName);
    if (it != _playerNames.end()) {
        long index = std::distance(_playerNames.begin(), it);

        // Update the score in the list control
        _listCtrl->SetItem(index, 1, wxString::Format(wxT("%d"), score));
    }
}

void Scoreboard::SelectItem(long index) {
    // Deselect all items
    long item = -1;
    while ((item = _listCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1) {
        _listCtrl->SetItemState(item, 0, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
    }

    // Select the specified item
    if (index >= 0 && index < _listCtrl->GetItemCount()) {
        _listCtrl->SetItemState(index, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
    }
}