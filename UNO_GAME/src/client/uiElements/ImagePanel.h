//
// Created by Software Engineering TA-Team in 2021.
//

#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <wx/wx.h>
#include <wx/sizer.h>

// This class can be used to display an image. It can be scaled with parameter <size> and rotated with <rotation> (in radian)
class ImagePanel : public wxPanel
{
    wxImage _image;
    wxBitmap _bitmap;

    double _rotation;

    int _width;
    int _height;

public:
    /**
    * \brief Constructor of ImagePanel
    * \param: parent to inherit from, file, format position, size and rotation
    * \result ImagePanel is constructed according to parameters
    */
    ImagePanel(wxWindow* parent, wxString file, wxBitmapType format, wxPoint position = wxDefaultPosition, wxSize size = wxDefaultSize, double rotation = 0.0);

    /**
    * \brief redraws the panel
    * \param: an event
    * \result redraws the panel according to the event
    */
    void paintEvent(wxPaintEvent& event);

    /**
    * \brief redraws itself when the user resizes the panel
    * \param: an event
    * \result redraws if the event is a resizing, does nothing otherwise
    */
    void onSize(wxSizeEvent& event);
};

#endif // IMAGEPANEL_H
