//
// Created by Software Engineering TA-Team in 2021.
//

#ifndef TEST_INPUTFIELD_H
#define TEST_INPUTFIELD_H

#include <wx/wx.h>

class InputField : public wxPanel {

public:
    /**
    * \brief Constructor of InputField
    * \param: parent to inherit from, label and field informations
    * \result InputField is constructed according to parameters
    */
    InputField(wxWindow* parent, const wxString& labelText, int labelWidth, const wxString& fieldValue, int fieldWidth);
    
    /**
    * \brief get value of the field
    * \result returns value of the field
    */
    wxString getValue();

private:
    wxStaticText* _label;
    wxTextCtrl* _field;

};

#endif //TEST_INPUTFIELD_H
