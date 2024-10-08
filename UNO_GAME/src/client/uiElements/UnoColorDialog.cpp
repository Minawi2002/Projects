//
// Created by UNOrganized on 11.12.2023.
//

#include "UnoColorDialog.h"

UnoColorDialog::UnoColorDialog(const wxString& title)
    : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300, 150)),
      _selectedColorIndex(-1) // Initialize the selectedColorIndex
{
    // Create a panel to hold the controls
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    // Create a sizer for the color squares
    wxBoxSizer* colorSizer = new wxBoxSizer(wxHORIZONTAL);

    // Define Uno colors
    wxColour unoColors[4] = { *wxBLUE, *wxGREEN, *wxRED, *wxYELLOW };

    // Create colored squares using wxStaticBitmap
    for (int i = 0; i < 4; i++) {
        wxBitmap colorBitmap(50, 50);
        wxMemoryDC dc(colorBitmap);
        dc.SetBackground(wxBrush(wxColour(255, 255, 255)));
        dc.Clear();
        dc.SetBrush(wxBrush(unoColors[i]));
        dc.DrawRectangle(5, 5, 40, 40);
        dc.SelectObject(wxNullBitmap);

        wxStaticBitmap* colorSquare = new wxStaticBitmap(panel, wxID_ANY, colorBitmap);
        colorSquare->Bind(wxEVT_LEFT_DOWN, &UnoColorDialog::OnColorSelection, this);
        colorSizer->Add(colorSquare, 0, wxALL, 5);
    }

    // Create a top-level sizer to arrange the color squares
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(colorSizer, 0, wxALIGN_CENTER | wxALL, 10);

    panel->SetSizer(topSizer);
}

// Event handler for color selection
void UnoColorDialog::OnColorSelection(wxMouseEvent& event)
{
    wxStaticBitmap* colorSquare = dynamic_cast<wxStaticBitmap*>(event.GetEventObject());
    if (colorSquare) {
        _selectedColorIndex = FindColorIndex(colorSquare); // Update the selectedColorIndex
        int colorIndex = _selectedColorIndex;
        if (colorIndex != -1) {
            _selectedColor = GetColorNameByIndex(colorIndex);
            EndModal(wxID_OK);
        }
    }
}

// Getter function for the selected color
wxString UnoColorDialog::GetSelectedColor() const
{
    return _selectedColor;
}

// Getter function for the selected color index
int UnoColorDialog::GetSelectedColorIndex() const
{
    return _selectedColorIndex;
}

//Cannot close the Dialog if no color is chosen
void UnoColorDialog::OnClose(wxCloseEvent& event)
{
    if (_selectedColorIndex == -1) {
        wxMessageBox("Please choose a color before closing the dialog.", "No Color Selected", wxOK | wxICON_WARNING, this);
        event.Veto(); // Veto the close event
    } else {
        event.Skip(); // Allow the close event
    }
}

// Utility function to find the index of the selected color square
int UnoColorDialog::FindColorIndex(wxStaticBitmap* colorSquare)
{
    wxWindowList children = colorSquare->GetParent()->GetChildren();
    int index = 0;
    for (wxWindowList::const_iterator it = children.begin(); it != children.end(); ++it, ++index) {
        if (*it == colorSquare) {
            return index;
        }
    }
    return -1;
}

// Utility function to get the color name by index
wxString UnoColorDialog::GetColorNameByIndex(int index)
{
    switch (index) {
        case 0: return "Blue";
        case 1: return "Green";
        case 2: return "Red";
        case 3: return "Yellow";
        default: return "Unknown";
    }
}

wxBEGIN_EVENT_TABLE(UnoColorDialog, wxDialog)
    EVT_LEFT_DOWN(UnoColorDialog::OnColorSelection)
    EVT_CLOSE(UnoColorDialog::OnClose)
wxEND_EVENT_TABLE()
