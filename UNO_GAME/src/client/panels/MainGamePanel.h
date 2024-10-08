//
// Created by Software Engineering TA-Team in 2021.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNO_CLIENT_MAINGAMEPANEL_H
#define UNO_CLIENT_MAINGAMEPANEL_H

#include <wx/wx.h>
#include "../../common/game_state/game_state.h"
#include "../windows/GameWindow.h"


class MainGamePanel : public wxPanel {

public:
    /**
    * \brief constructs the MainGamePanel
    * \param: the parent game window that should be inherited
    * \result MainGamePanel is constructed
    */
    MainGamePanel(wxWindow* parent);

    /**
    * \brief builds the game state and all UI elements
    * \param: the game state that should be built
    * \result displays all UI elements according to the game state
    */
    void buildGameState(game_state* gameState, player* me);

    /**
    * \brief shows a waiting message that covers the whole panel
    * \result displays "waiting for all players to start new round"
    */
    void buildWaitForNewRound();

private:

    /**
    * \brief builds the hand of one other player 
    * \param: the game state, the other player and the angle where the player is located
    * \result displays the hand of the desired other player
    */
    void buildOtherPlayerHand(game_state* gameState, player* otherPlayer, double playerAngle);
    
    /**
    * \brief builds the label of one other player
    * \param: the game state, the other player and the angle where the player is located
    * \result displays the label of the desired other player
    */
    void buildOtherPlayerLabels(game_state* gameState, player* otherPlayer, double playerAngle, int side);
    
    /**
    * \brief builds the draw and discard pile
    * \param: the game state and the pointer to the player corresponding to the client
    * \result displays the draw and discard pile in its current state
    */
    void buildCardPiles(game_state* gameState, player *me);

    /**
    * \brief builds a text that shows whos turn it is
    * \param: the game state and the pointer to the player corresponding to the client
    * \result displays a text that shows whos turn it is
    */
    void buildTurnIndicator(game_state* gameState, player* me);

    /**
    * \brief builds the own player, messages and maybe the own cards
    * \param: the game state and the pointer to the player corresponding to the client
    * \result displays game state messages and if the game is started the own players cards
    */
    void buildThisPlayer(game_state* gameState, player* me);

    /**
    * \brief builds the uno button
    * \param: the game state and the pointer to the player corresponding to the client
    * \result displays the uno button
    */
    void buildUnoButton(game_state* gameState, player *me);

    /**
    * \brief builds the scoreboard
    * \param: the game state and the pointer to the player corresponding to the client and the position of the scoreboard
    * \result displays the scoreboard corresponding to the game state
    */
    void buildScoreboard(game_state* gameState, player *me, int myPosition);

    /**
    * \brief builds a wxStaticText
    * \param: the content, position, size, alignment and boldness of the text
    * \result returns the static text built corresponding to the parameters
    */
    wxStaticText* buildStaticText(std::string content, wxPoint position, wxSize size, long textAlignment, bool bold = false);

    /**
    * \brief returns the size of the new square
    * \param: the current edgeLength and rotationAngle
    * \result returns the size of the new square corresponding to the parameters
    */
    wxSize getBoundsOfRotatedSquare(double edgeLength, double rotationAngle);

    /**
    * \brief calculates the size of the new edgeLenth
    * \param: the current edgeLength and rotationAngle
    * \result returns new edgeLength
    */
    double getEdgeLengthOfRotatedSquare(double originalEdgeLength, double rotationAngle);

    /**
    * \brief calculates the poisition on the ellipse around the card piles
    * \param: the current horizontal and vertical radius and angle
    * \result returns the position on the ellipse around the card piles corresponding to radius and angle
    */
    wxPoint getPointOnEllipse(double horizontalRadius, double verticalRadius, double angle);

    // define key constant layout values
    wxSize const panelSize = wxSize(960, 680); // also set in the constructor implementation
    wxPoint const tableCenter = wxPoint(480, 300);
    wxSize const cardSize = wxSize(80, 124);
    wxSize const boxSize = wxSize(60, 60);
    wxSize const unoButtonSize = wxSize(60, 60);

    double const otherPlayerHandSize = 160.0;
    double const otherPlayerHandDistanceFromCenter = 180.0;
    double const otherPlayerLabelDistanceFromCenter = 275.0;

    wxPoint const discardPileOffset = wxPoint(-84, -42);
    wxPoint const drawPileOffset = wxPoint(4, -42);
    wxPoint const colorBoxOffset = wxPoint(88, -42);
    wxPoint const unoButtonOffset = wxPoint(88, 20);
    wxPoint const turnIndicatorOffset = wxPoint(-100, 98);
    wxPoint const darkmodeButtonOffset = wxPoint(100, 0);

    double const twoPi = 6.28318530718;

    wxCheckBox* _playWithQuestionMarkCheckBox;

};


#endif //UNO_CLIENT_MAINGAMEPANEL_H
