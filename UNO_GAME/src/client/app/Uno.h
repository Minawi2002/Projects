//
// Created by Software Engineering TA-Team in 2021.
//

#ifndef UNOUI_UNO_H
#define UNOUI_UNO_H

#include <wx/wx.h>
#include "../windows/GameWindow.h"
#include "../GameController.h"


// Main app class
class Uno : public wxApp
{
public:
    /**
    * \brief starts a game window
    * \result starts and shows a game window
    */
    virtual bool OnInit();
};


#endif //UNOUI_UNO_H
