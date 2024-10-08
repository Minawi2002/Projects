//
// Created by Software Engineering TA-Team in 2021.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNOUI_GAMECONTROLLER_H
#define UNOUI_GAMECONTROLLER_H

#include "windows/GameWindow.h"
#include "panels/ConnectionPanel.h"
#include "panels/MainGamePanel.h"
#include "network/ResponseListenerThread.h"
#include "../common/game_state/game_state.h"
#include "uiElements/UnoColorDialog.h"

#include <mutex>

class GameController {

public:
    /**
    * \brief initializes GameWindow and all GamePanels
    * \param: pointer to the GameWindow that should be shown
    * \result shows the ConnectionPanel
    */
    static void init(GameWindow* gameWindow);

    /**
    * \brief connects client and therefore player to the server
    * \result client and player are connected, join game request is sent
    */
    static void connectToServer();

    /**
    * \brief updates the current game state on the clients
    * \param: the new game state that should be displayed
    * \result the new game state is displayed and, if necessary, messages are shown
    */
    static void updateGameState(game_state* newGameState);

    /**
    * \brief waits for all player to join the new round
    * \param: game state with the infromation of how many players joined the new round
    * \result shows waiting interface and when all players joined the new round is started
    */
    static void WaitForNewRound(game_state* state);

    /**
    * \brief tries to start the game
    * \param: a boolean that is true if the game should played with the questionmarkcard
    * \result a start game request is sent
    */
    static void startGame(bool questionmarkbool);

    /**
    * \brief tries to start a new round
    * \result a new round request is sent
    */
    static void newRound();

    /**
    * \brief tries to deal with a player leaving the game
    * \result a player left request is sent
    */
    static void playerLeft();

    /**
    * \brief tries to draw a card from the draw pile
    * \result a draw card is sent
    */
    static void drawCard();

    /**
    * \brief tries to call uno for the player
    * \result a call uno request ist sent
    */
    static void callUno();

    /**
    * \brief tries to play a card on the discard pile 
    * \param: a pointer to the card that should be played
    * \result a play card request is sent
    */
    static void playCard(card* cardToPlay);

    /**
    * \brief tries to pick a color that is chosen for a wildcard
    * \param: the color that was chosen by the player
    * \result a pick a color request is sent
    */
    static void pickColor(int color);

    /**
    * \brief returns the MainThreadEventHandler
    * \result returns the MainThreadEventHandler
    */
    static wxEvtHandler* getMainThreadEventHandler();

    /**
    * \brief displays the error that was received
    * \result displays the error that was received
    */
    static void showError(const std::string& title, const std::string& message);

    /**
    * \brief displays the connection status in the clients window
    * \param: the message that should be displayed
    * \result displays the connection status in the clients window
    */
    static void showStatus(const std::string& message);

    /**
    * \brief displays a dialog box to choose color 
    * \result the player chooses a card, PickColor is called
    */
    static void showPickColor();

    /**
    * \brief displays a dialog box with the rounds results
    * \param: the old and new game state such that points can be compared
    * \result when closing the dialogbox, newRound is called
    */
    static void showNewRoundMessage(game_state* oldGameState, game_state* newGameState);
    
    /**
    * \brief displays a dialog box with the games results
    * \result either quits the client or makes a join game request
    */
    static void showGameOverMessage();

private:
    static GameWindow* _gameWindow;
    static ConnectionPanel* _connectionPanel;
    static MainGamePanel* _mainGamePanel;

    static player* _me;
    static game_state* _currentGameState;
    static bool _colorDialogOpen;
};


#endif //UNOUI_GAMECONTROLLER_H
