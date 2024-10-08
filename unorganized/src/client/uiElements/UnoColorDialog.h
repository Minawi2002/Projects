//
// Created by UNOrganized on 11.12.2023.
//

#ifndef UNOCOLORDIALOG_H
#define UNOCOLORDIALOG_H

#include <wx/wx.h>

class UnoColorDialog : public wxDialog
{
public:
    /**
    * \brief Constructor of UnoColorDialog
    * \param: title of the dialog
    * \result UnoColorDialog is constructed according to parameters
    */
    UnoColorDialog(const wxString& title);

    //** accessors **//
    /**
    * \brief gets the color that was selected
    * \result returns the color that was selected
    */
    wxString GetSelectedColor() const;

    /**
    * \brief gets the index of the color that was selected
    * \result returns the index of the color that was selected
    */
    int GetSelectedColorIndex() const;

private:
    wxString _selectedColor;
    int _selectedColorIndex;

    /**
    * \brief overrides the onclose method, should not be able to close before color was chosen
    * \param: an event
    * \result cannot close dialog before a color was chosen
    */
    void OnClose(wxCloseEvent& event);

    /**
    * \brief event handler for color selection
    * \param: an event
    * \result guides through the color selection precess
    */
    void OnColorSelection(wxMouseEvent& event);

    /**
    * \brief finds the color that was selected in the process
    * \param: a bitmap
    * \result returns the color index that was selected in the process
    */
    int FindColorIndex(wxStaticBitmap* colorSquare);

    /**
    * \brief transles index to string
    * \param: the index of the color
    * \result returns the name of the color in a string
    */
    wxString GetColorNameByIndex(int index);

    wxDECLARE_EVENT_TABLE();
};

#endif // UNOCOLORDIALOG_H